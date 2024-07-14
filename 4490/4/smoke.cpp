//
//modified by: Bradley Tate
//date: Feb 2024
//
//program: smoke.cpp
//author:  Gordon Griesel
//date written: 2018
//date modified: Jan 2020
//date modified: Feb 2022
//
//An OpenGL 3D framework for students.
//
//Goal:
//     Make the rising transparent panels look like smoke.
//     You must...
//       1. sort the panels by distance from the camera
//       2. billboard each panel
//       3. render the panels in order using the painter's algorithm
//          furthest panel is drawn first
//          closest panel is drawn last 
//
//After getting this to work, we will try to improve the look
//by applying a technique so that none of the panels intersect
//each other. Any intersection will ruin the look of the smoke.
//
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "fonts.h"

typedef float Flt;
typedef Flt Vec[3];
typedef Flt	Matrix[4][4];
//some macros
#define MakeVector(x, y, z, v) (v)[0]=(x);(v)[1]=(y);(v)[2]=(z)
#define VecCross(a,b,c) (c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1];\
							   (c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2];\
(c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]
#define rnd() (Flt)rand() / (Flt)RAND_MAX
//some constants
const Flt PI = 3.141592653589793;
const Vec upv = {0.0, 1.0, 0.0};
const int MAX_SMOKES = 400;

//
class Smoke {
	public:
		Vec pos;
		Vec vert[16];
		Flt radius;
		int n;
        Flt distance;
		struct timespec tstart;
		Flt maxtime;
		Flt alpha;
		Smoke() { }
};

//-----------------------------------------------------------------------------
//Setup timers
const double OOBILLION = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------

class Global {
	public:
		int xres, yres;
		Flt aspectRatio;
		Vec cameraPos;
		Flt cameraAngle;
        Matrix cameraMat;
		int circling;
		int sorting;
		int billboarding;
		GLfloat lightPosition[4];
		struct timespec smokeStart, smokeTime;
		Smoke *smoke;
		int nsmokes;
		~Global() {
			if (smoke)
				delete [] smoke;
		}
		Global() {
			//constructor
			xres = 640;
			yres = 480;
			aspectRatio = (GLfloat)xres / (GLfloat)yres;
			MakeVector(0.0, 15.0, 75.0, cameraPos);
			cameraAngle = 0.0;
			circling = 0;
			sorting = 0;
			billboarding = 0;
			//light is high, right, toward
			MakeVector(100.0f, 1240.0f, 40.0f, lightPosition);
			lightPosition[3] = 1.0f;
			clock_gettime(CLOCK_REALTIME, &smokeStart);
			nsmokes = 0;
			smoke = new Smoke[MAX_SMOKES];
		}
} g;

//X Windows wrapper class
class X11_wrapper {
	private:
		Display *dpy;
		Window win;
		GLXContext glc;
	public:
		X11_wrapper() {
			//Look here for information on XVisualInfo parameters.
			//http://www.talisman.org/opengl-1.1/Reference/glXChooseVisual.html
			//
			GLint att[] = { GLX_RGBA,
				GLX_STENCIL_SIZE, 2,
				GLX_DEPTH_SIZE, 24,
				GLX_DOUBLEBUFFER, None };
			XSetWindowAttributes swa;
			setup_screen_res(g.xres, g.yres);
			dpy = XOpenDisplay(NULL);
			if (dpy == NULL) {
				printf("\ncannot connect to X server\n\n");
				exit(EXIT_FAILURE);
			}
			Window root = DefaultRootWindow(dpy);
			XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
			if (vi == NULL) {
				printf("\nno appropriate visual found\n\n");
				exit(EXIT_FAILURE);
			}
			Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
			swa.colormap = cmap;
			swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
				StructureNotifyMask | SubstructureNotifyMask;
			win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
					vi->depth, InputOutput, vi->visual,
					CWColormap | CWEventMask, &swa);
			set_title("4490 OpenGL");
			glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
			glXMakeCurrent(dpy, win, glc);
		}
		~X11_wrapper() {
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
		}
		void set_title(const char *str) {
			//Set the window title bar.
			XMapWindow(dpy, win);
			XStoreName(dpy, win, str);
		}
		void setup_screen_res(const int w, const int h) {
			g.xres = w;
			g.yres = h;
			g.aspectRatio = (GLfloat)g.xres / (GLfloat)g.yres;
		}
		void reshape_window(int width, int height) {
			//window has been resized.
			setup_screen_res(width, height);
			glViewport(0, 0, (GLint)width, (GLint)height);
		}
		void check_resize(XEvent *e) {
			//ConfigureNotify is sent by the server if the window is resized.
			if (e->type != ConfigureNotify)
				return;
			XConfigureEvent xce = e->xconfigure;
			if (xce.width != g.xres || xce.height != g.yres) {
				//Window size did change.
				reshape_window(xce.width, xce.height);
			}
		}
		bool getXPending() {
			return XPending(dpy);
		}
		void swapBuffers() {
			glXSwapBuffers(dpy, win);
		}
		XEvent getXNextEvent() {
			XEvent e;
			XNextEvent(dpy, &e);
			return e;
		}
} x11;


void init_opengl();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

int main()
{
	srand((unsigned)time(NULL));
	init_opengl();
	int done=0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		physics();
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	return 0;
}

void init()
{

}

void init_opengl()
{
	//OpenGL initialization
	glClearColor(0.5f, 0.6f, 1.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, g.aspectRatio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	//Enable this so material colors are the same as vert colors.
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	//Turn on a light
	glLightfv(GL_LIGHT0, GL_POSITION, g.lightPosition);
	glEnable(GL_LIGHT0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//Test the stencil buffer on this computer.
	//
	// https://www.opengl.org/discussion_boards/showthread.php/
	// 138452-stencil-buffer-works-on-one-machine-but-not-on-another
	//
	// Before you try using stencil buffer try this:
	// Code :
	// GLint stencilBits = 0;
	// glGetIntegerv(GL_STENCIL_BITS, &amp;stencilBits);
	// if (stencilBits < 1)
	//    MessageBox(NULL,"no stencil buffer.\n","Stencil test", MB_OK);
	GLint stencilBits = 0;
	glGetIntegerv(GL_STENCIL_BITS, &stencilBits);
	if (stencilBits < 1) {
		printf("No stencil buffer on this computer.\n");
		printf("Exiting program.\n");
		exit(0);
	}
}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

int check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		switch(key) {
			case XK_r:
				break;
			case XK_1:
				//camera front
				MakeVector(0.0, 15.0, 75.0, g.cameraPos);
				g.circling = 0;
				break;
			case XK_2:
				//camera high
				MakeVector(0.0, 65.0, 15.0, g.cameraPos);
				g.circling = 0;
				break;
			case XK_3:
				//camera right
				MakeVector(50.0, 15.0, 3.0, g.cameraPos);
				g.circling = 0;
				break;
			case XK_4:
				//camera is circling
				g.cameraAngle = PI * 1.5;
				g.circling = 1;
				break;
			case XK_s:
				g.sorting ^= 1;
				break;
			case XK_b:
				g.billboarding ^= 1;
				break;
			case XK_Escape:
				return 1;
		}
	}
	return 0;
}

void drawBox(float w1, float h1, float d1)
{
	//
	//   1================2
	//   |\              /|
	//   | \            / |
	//   |  \          /  |
	//   |   \        /   |
	//   |    5------6    |
	//   |    |      |    |
	//   |    |      |    |
	//   |    |      |    |
	//   |    4------7    |
	//   |   /        \   |
	//   |  /          \  |
	//   | /            \ |
	//   |/              \|
	//   0================3
	//
	const Flt vert[][3] = {
		-1.0, -1.0,  1.0,
		-1.0,  1.0,  1.0,
		1.0,  1.0,  1.0,
		1.0, -1.0,  1.0,
		-1.0, -1.0, -1.0,
		-1.0,  1.0, -1.0,
		1.0,  1.0, -1.0,
		1.0, -1.0, -1.0 };
	//left,top,right,bottom,front,back.
	const int face[][4] = {
		0,1,5,4,
		1,2,6,5,
		2,3,7,6,
		0,4,7,3,
		2,1,0,3,
		4,5,6,7 };
	const Flt norm[][3] = {
		-1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0,
		0.0,-1.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 0.0,-1.0 };
	//half the width from center.
	Flt w = w1 * 0.5;
	Flt d = d1 * 0.5;
	Flt h = h1 * 0.5;
	//Normals are required for any lighting.
	glBegin(GL_QUADS);
	for (int i=0; i<6; i++) {
		glNormal3fv(norm[i]);
		for (int j=0; j<4; j++) {
			int k = face[i][j];
			glVertex3f(vert[k][0]*w, vert[k][1]*h, vert[k][2]*d);
		}
	}
	glEnd();
}

void drawGround()
{
	glColor3f(0.9f, 0.9f, 0.1f);
	Flt w = 40.0 * 0.5;
	Flt d = 40.0 * 0.5;
	Flt h = 0.0;
	glBegin(GL_QUADS);
	//top
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glVertex3f(-w, h,-d);
	glVertex3f( w, h,-d);
	glVertex3f( w, h, d);
	glVertex3f(-w, h, d);
	glEnd();
	//Draw colored corner posts.
	unsigned char c[] = {255,0,0, 0,255,0, 0,0,255, 0,0,0};
	for (int i=0; i<4; i++) {
		glPushMatrix();
		glColor3ubv(c+(i*3));
		glRotatef(90.0*(Flt)i, 0.0, 1.0, 0.0);
		glTranslatef(20.0, -0.9, 20.0);
		drawBox(2, 2, 2);
		glPopMatrix();
	}
}

void make_view_matrix(const Vec p1, const Vec p2, Matrix m)
{
	//Line between p1 and p2 form a LOS Line-of-sight.
	//A rotation matrix is built to transform objects to this LOS.
	//Diana Gruber  http://www.makegames.com/3Drotation/
	m[0][0]=m[1][1]=m[2][2]=1.0f;
	m[0][1]=m[0][2]=m[1][0]=m[1][2]=m[2][0]=m[2][1]=0.0f;
	Vec out = { p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2] };
	//
	Flt l1, len = out[0]*out[0] + out[1]*out[1] + out[2]*out[2];
	if (len == 0.0f) {
		MakeVector(0.0f,0.0f,1.0f,out);
	} else {
		l1 = 1.0f / sqrtf(len);
		out[0] *= l1;
		out[1] *= l1;
		out[2] *= l1;
	}
	m[2][0] = out[0];
	m[2][1] = out[1];
	m[2][2] = out[2];
	Vec up = { -out[1] * out[0], upv[1] - out[1] * out[1], -out[1] * out[2] };
	//
	len = up[0]*up[0] + up[1]*up[1] + up[2]*up[2];
	if (len == 0.0f) {
		MakeVector(0.0f,0.0f,1.0f,up);
	}
	else {
		l1 = 1.0f / sqrtf(len);
		up[0] *= l1;
		up[1] *= l1;
		up[2] *= l1;
	}
	m[1][0] = up[0];
	m[1][1] = up[1];
	m[1][2] = up[2];
	//make left vector.
	VecCross(up, out, m[0]);
}


void vecNormalize(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	if (len == 0.0)
		return;
	len = 1.0 / sqrt(len);
	v[0] *= len;
	v[1] *= len;
	v[2] *= len;
}

void vecScale(Vec v, Flt s)
{
	v[0] *= s;
	v[1] *= s;
	v[2] *= s;
}

void VecSub(Vec v0, Vec v1, Vec v2)
{
    v2[0] = v0[0] - v1[0];
    v2[1] = v0[1] - v1[1];
    v2[2] = v0[2] - v1[2];
}

void drawSmoke()
{
	if (g.sorting) { 
        for(int i = 0; i < g.nsmokes; i++) {
            Flt dx = g.smoke[i].pos[0] - g.cameraPos[0];
            Flt dy = g.smoke[i].pos[1] - g.cameraPos[1];
            Flt dz = g.smoke[i].pos[2] - g.cameraPos[2];
            g.smoke[i].distance = dx * dx + dy * dy + dz * dz;
        }
        for(int i = 0; i < g.nsmokes - 1; i++) {
            for(int j = 0; j < g.nsmokes - 1 - i; j++) {
                if (g.smoke[j].distance < g.smoke[j + 1].distance) {
                    Smoke tmp = g.smoke[j];
                    g.smoke[j] = g.smoke[j + 1];
                    g.smoke[j + 1] = tmp;
                }
            }
        }
	}

	//
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i=0; i<g.nsmokes; i++) {
		glPushMatrix();
		glTranslatef(g.smoke[i].pos[0],g.smoke[i].pos[1],g.smoke[i].pos[2]);
		if (g.billboarding) {
            //billboarding code segment here
            Vec v;
            VecSub(g.smoke[i].pos, g.cameraPos, v);
            Vec z = {0.0f, 0.0f, 0.0f};
            Matrix m;
            make_view_matrix(z, v, m);
            float mat[16];
            mat[ 0] = m[0][0];
            mat[ 1] = m[0][1];
            mat[ 2] = m[0][2];
            mat[ 4] = m[1][0];
            mat[ 5] = m[1][1];
            mat[ 6] = m[1][2];
            mat[ 8] = m[2][0];
            mat[ 9] = m[2][1];
            mat[10] = m[2][2];
            mat[ 3] = mat[ 7] = mat[11] = mat[12] = mat[13] = mat[14] = 0.0f;
            mat[15] = 1.0f;
            glMultMatrixf(mat);
		}
		//glTranslatef(g.smoke[i].pos[0],g.smoke[i].pos[1],g.smoke[i].pos[2]);
		glColor4ub(255, 0, 0, (unsigned char)g.smoke[i].alpha);
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0.0, 0.0, 1.0);
		for (int j=0; j<g.smoke[i].n; j++) {
			//each vertex of the smoke
			//glVertex3fv(g.smoke[i].vert[j]);
			vecNormalize(g.smoke[i].vert[j]);
			vecScale(g.smoke[i].vert[j], g.smoke[i].radius);
			glVertex3fv(g.smoke[i].vert[j]);
		}
		glEnd();
		glPopMatrix();
	}
	glDisable(GL_BLEND);
}

void make_a_smoke()
{
	if (g.nsmokes < MAX_SMOKES) {
		Smoke *s = &g.smoke[g.nsmokes];
		s->pos[0] = rnd() * 5.0 - 2.5;
		s->pos[2] = rnd() * 5.0 - 2.5;
		s->pos[1] = rnd() * 0.1 + 0.1;
		s->radius = rnd() * 1.0 + 0.5;
		s->n = rand() % 5 + 5;
		Flt angle = 0.0;
		Flt inc = (PI*2.0) / (Flt)s->n;
		for (int i=0; i<s->n; i++) {
			s->vert[i][0] = cos(angle) * s->radius;
			s->vert[i][1] = sin(angle) * s->radius;
			s->vert[i][2] = 0.0;
			angle += inc;
		}
		s->maxtime = 8.0;
		s->alpha = 100.0;
		clock_gettime(CLOCK_REALTIME, &s->tstart);
		++g.nsmokes;
	}
}

void physics()
{
	clock_gettime(CLOCK_REALTIME, &g.smokeTime);
	double d = timeDiff(&g.smokeStart, &g.smokeTime);
	if (d > 0.05) {
		//time to make another smoke particle
		make_a_smoke();
		timeCopy(&g.smokeStart, &g.smokeTime);
	}
	//move smoke particles
	for (int i=0; i<g.nsmokes; i++) {
		//smoke rising
		g.smoke[i].pos[1] += 0.015;
		g.smoke[i].pos[1] += ((g.smoke[i].pos[1]*0.24) * (rnd() * 0.075));
		//expand particle as it rises
		g.smoke[i].radius += g.smoke[i].pos[1]*0.002;
		//wind might blow particle
		if (g.smoke[i].pos[1] > 10.0) {
			//experiment here with different values
			g.smoke[i].pos[0] -= rnd() * 0.1;
		}
	}
	//this is where a smoke particle will fade away as it lingers
	int i=0;
	while (i < g.nsmokes) {
		struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		double d = timeDiff(&g.smoke[i].tstart, &bt);
		if (d > g.smoke[i].maxtime - 3.0) {
			g.smoke[i].alpha *= 0.95;
			if (g.smoke[i].alpha < 1.0)
				g.smoke[i].alpha = 1.0;
		}
		if (d > g.smoke[i].maxtime) {
			//delete this smoke
			--g.nsmokes;
			g.smoke[i] = g.smoke[g.nsmokes];
			continue;
		}
		++i;
	}
	//
	if (g.circling) {
		//here the camera is circling the smoke
		Flt rad = 80 + sin(g.cameraAngle) * 50.0;
		Flt x = cos(g.cameraAngle) * rad;
		Flt z = sin(g.cameraAngle) * rad;
		Flt y = 25.0;
		MakeVector(x, y, z, g.cameraPos);
		g.cameraAngle -= 0.01;
	}
}

void render()
{
	//Clear the depth buffer and screen.
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//
	//Render a 3D scene
	//
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	gluPerspective(45.0f, g.aspectRatio, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		g.cameraPos[0], g.cameraPos[1], g.cameraPos[2],
		0,12,0,
		0,1,0);
	glLightfv(GL_LIGHT0, GL_POSITION, g.lightPosition);
	//
	drawGround();
	drawSmoke();
	//
	//switch to 2D mode
	//
	Rect r;
	glViewport(0, 0, g.xres, g.yres);
	glMatrixMode(GL_MODELVIEW);   glLoadIdentity();
	gluOrtho2D(0, g.xres, 0, g.yres);
	glMatrixMode (GL_PROJECTION); glLoadIdentity();
	glDisable(GL_LIGHTING);
	r.bot = g.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00000000, "Smoke");
	ggprint8b(&r, 16, 0x00000000, "1 - Camera front");
	ggprint8b(&r, 16, 0x00000000, "2 - Camera high");
	ggprint8b(&r, 16, 0x00000000, "3 - Camera right");
	ggprint8b(&r, 16, 0x00000000, "4 - Camera circling");
	ggprint8b(&r, 16, 0x00000000, "S - Sorting");
	ggprint8b(&r, 16, 0x00000000, "B - Billboarding");
	ggprint8b(&r, 16, 0x00000000, "nsmokes: %i", g.nsmokes);
}



