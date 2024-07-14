//modified by: Bradley Tate
//date: February 2024
//
//pointline.cpp
//author: Gordon Griesel
//date: February 2022
//purpose: collision detection exercise
//
//Determine if, when, and where a moving point intersects a line segment.
//
//Press spacebar to start.
//
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
//for text on the screen
#include "fonts.h"

typedef float Flt;
typedef Flt Vec[3];

class Point {
public:
	Vec pos;
	Vec vel;
	Point() {
		vel[0] = vel[1] = vel[2] = 0.0f;
	}
};

class Global {
public:
	int xres, yres;
	Point line[2];   //a line is 2 end-points
	Point point;     //a point is one point
	bool animation;
	Global() {
		xres = 800;
		yres = 600;
		animation = false;
	}
} g;

class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	~X11_wrapper();
	X11_wrapper();
	void set_title();
	bool getXPending();
	XEvent getXNextEvent();
	void swapBuffers();
	void reshape_window(int width, int height);
	void check_resize(XEvent *e);
	void check_mouse(XEvent *e);
	int check_keys(XEvent *e);
} x11;

//Function prototypes
void init_opengl(void);
void physics(void);
void render(void);

int main()
{
	init_opengl();
	//main game loop
	int done = 0;
	while (!done) {
		//process events...
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			x11.check_mouse(&e);
			done = x11.check_keys(&e);
		}
		physics();           //move things
		render();            //draw things
		x11.swapBuffers();   //make video memory visible
		usleep(1000);        //pause to let X11 work better
	}
	cleanup_fonts();
	return 0;
}

X11_wrapper::~X11_wrapper()
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

X11_wrapper::X11_wrapper()
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	int w = g.xres, h = g.yres;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		cout << "\n\tcannot connect to X server\n" << endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		cout << "\n\tno appropriate visual found\n" << endl;
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask =
		ExposureMask | KeyPressMask | KeyReleaseMask |
		ButtonPress | ButtonReleaseMask |
		PointerMotionMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
		InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void X11_wrapper::set_title()
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "4490 Lab1");
}

bool X11_wrapper::getXPending()
{
	//See if there are pending events.
	return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
	//Get a pending event.
	XEvent e;
	XNextEvent(dpy, &e);
	return e;
}

void X11_wrapper::swapBuffers()
{
	glXSwapBuffers(dpy, win);
}

void X11_wrapper::reshape_window(int width, int height)
{
	//window has been resized.
	g.xres = width;
	g.yres = height;
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
}

void X11_wrapper::check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != g.xres || xce.height != g.yres) {
		//Window size did change.
		reshape_window(xce.width, xce.height);
	}
}
//-----------------------------------------------------------------------------

void X11_wrapper::check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;

	//Weed out non-mouse events
	if (e->type != ButtonRelease &&
		e->type != ButtonPress &&
		e->type != MotionNotify) {
		//This is not a mouse event that we care about.
		return;
	}
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button was pressed.
			//int y = g.yres - e->xbutton.y;
			//int x = e->xbutton.x;
			return;
		}
		if (e->xbutton.button==3) {
			//Right button was pressed.
			return;
		}
	}
	if (e->type == MotionNotify) {
		//The mouse moved!
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
			savex = e->xbutton.x;
			savey = e->xbutton.y;
			//Code placed here will execute whenever the mouse moves.


		}
	}
}

int X11_wrapper::check_keys(XEvent *e)
{
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyPress) {
		switch (key) {
			case XK_space:
				g.animation = !g.animation;
				if (g.animation) {
					//define a line segment...
					g.line[0].pos[0] = rand() % g.xres;				   
					g.line[0].pos[1] = g.yres - 1;				   
					g.line[1].pos[0] = rand() % g.xres;				   
					g.line[1].pos[1] = 0;				   
					//define the moving point...
					g.point.pos[0] = 5;				   
					g.point.pos[1] = rand() % g.yres;				   
					g.point.vel[1] = 0.0f;				   
					g.point.vel[0] = 4.0f;				   
				}
				break;
			case XK_Escape:
				//Escape key was pressed
				return 1;
		}
	}
	return 0;
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	//Set 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//Set the screen background color
	glClearColor(0.1, 0.1, 0.1, 1.0);
	//allow 2D texture maps
    glEnable(GL_TEXTURE_2D);
	initialize_fonts();
}

void physics()
{
    static int firsttime = 1;
    static int last_sign;
	if (g.animation) {
		g.point.pos[0] += g.point.vel[0];
		g.point.pos[1] += g.point.vel[1];
        //check for collision
        Vec v;
        v[0] = g.line[0].pos[0] - g.line[1].pos[0];
        v[1] = g.line[0].pos[1] - g.line[1].pos[1];
        Vec perp = {v[1], -v[0]};
        Vec v1;
        v1[0] = g.point.pos[0] - g.line[0].pos[0];
        v1[1] = g.point.pos[1] - g.line[0].pos[1];
        Flt dot = v1[0] * perp[0] + v1[1] * perp[1];
        int sign = (dot < 0.0) ? -1 : 1;
        if (firsttime) {
            firsttime = 0;
            last_sign = sign;
        }
        else {
            //not first time
            if (sign != last_sign) {
                //point crossed line
                g.point.vel[0] = 0.0;
            }
        }
	}
}


void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Rect r;
	r.bot = g.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 0, 0x00ffffff, "Point intersect line");
	//
	if (g.animation) {
		glPushMatrix();
		//Draw line.
		glColor3ub(155, 155, 255);
		glBegin(GL_LINES);
			glVertex2f(g.line[0].pos[0], g.line[0].pos[1]);
			glVertex2f(g.line[1].pos[0], g.line[1].pos[1]);
		glEnd();
		//Draw point.
		glColor3ub(100, 255, 100);
		glBegin(GL_POINTS);
			glVertex2f(g.point.pos[0], g.point.pos[1]);
		glEnd();
		//make point more visible...
		glColor3ub(90, 200, 60);
		glBegin(GL_POINTS);
			glVertex2f(g.point.pos[0]-1, g.point.pos[1]);
			glVertex2f(g.point.pos[0]+1, g.point.pos[1]);
			glVertex2f(g.point.pos[0]  , g.point.pos[1]-1);
			glVertex2f(g.point.pos[0]  , g.point.pos[1]+1);
		glEnd();
		glPopMatrix();
	}
}






