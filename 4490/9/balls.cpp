//modified by: Bradley Tate
//date: March 2024
//
//author: Gordon Griesel
//date: Spring 2022
//purpose: get openGL working on your personal computer
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
#include "fonts.h"

class Global {
    public:
        int xres, yres;
        int n;
        float bounds[2];
        float radius, radius2;
        int nframes;
        int fps;
        int nphysics;

        Global() {
            xres = 400;
            yres = 400;
            n = 0;
            bounds[0] = xres / 2;
            bounds[1] = yres / 2;
            radius = (xres / 2.1);
            radius2 = radius * radius;
            nframes = 0;
            fps = 0;
            nphysics = 1;
        }

} g;

class Ball {
    public:
        float radius, radius2;
        float pos[2];
        float last_pos[2];
        float color[3];
} balls[2000];


class Box {
    public:
        float w;
        float force;
        float pos[2];
        float last_pos[2];
        float verts[4][2];
        int nverts;
        union {
            unsigned int colorref;
            unsigned char rgb[4];
        } color;

        Box() {
            w = 20.0f;
            verts[0][0] = -w;
            verts[0][1] = -w;
            verts[1][0] = w;
            verts[1][1] = -w;
            verts[2][0] = w;
            verts[2][1] = w;
            verts[3][0] = -w;
            verts[3][1] = w;
            //pos[0] = 0.0f + w;
            //pos[1] = g.yres / 2.0f;
            //last_pos[0] = pos[0];
            //last_pos[1] = pos[1];
        }

} box[2];


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
    int seconds = time(NULL);
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
        //Extra physics calls resolves more overlaps; hinders performance
        for (int i = 0; i < g.nphysics; i++)
            physics();           //move things
        render();            //draw things
        ++g.nframes;
        int tmp = time(NULL);
        if (seconds != tmp) {
            g.fps = g.nframes;
            g.nframes = 0;
            seconds = tmp;
        }
        x11.swapBuffers();   //make video memory visible
        usleep(200);         //pause to let X11 work better
    }
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
    XStoreName(dpy, win, "4490 Lab 9");
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
    //resize boundary circle
    int a = min(g.xres, g.yres);
    g.radius = a * 0.5;
    g.bounds[0] = g.xres / 2;
    g.bounds[1] = g.yres / 2;
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

/*
int check_ball_overlap(int i, int j)
{
    if (i != j ) {
        float dx = ball[i].pos[0] - balls[j].pos[0];
        float dy = ball[i].pos[1] - balls[j].pos[1];
    }
}
*/

int X11_wrapper::check_keys(XEvent *e)
{
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
        switch (key) {
            case XK_p:
                ++g.nphysics;
                break;
            case XK_q:
                if (g.nphysics < 1)
                    g.nphysics = 1;
                --g.nphysics;
                break;
            case XK_1:
                //test collisions
                //set color
                balls[g.n].color[0] = rand() % 255;
                balls[g.n].color[1] = rand() % 255;
                balls[g.n].color[2] = rand() % 255;
                //check bounds for spawn location
                if (g.xres > 300)
                    balls[g.n].pos[0] = g.xres / 2 + rand() % 200 - 100;
                else
                    balls[g.n].pos[0] = g.xres / 2;
                //ball spawning and creation
                balls[g.n].pos[1] = g.yres / 2 + 50;
                balls[g.n].last_pos[0] = balls[g.n].pos[0] + rand() % 10 - 1;
                balls[g.n].last_pos[1] = balls[g.n].pos[1];
                balls[g.n].radius = 10;
                balls[g.n].radius2 = balls[g.n].radius * balls[g.n].radius;
                ++g.n;
                break;
            case XK_d:
                //Delete balls
                --g.n; 
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
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    //
    //initial position of balls
    g.n = 0;

    //allow text rendering
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

}

void physics()
{
    //Verlet integration
    //pos[0] += dir;
    float vel[2];
    for (int i = 0; i < g.n; i++) {
        vel[0] = balls[i].pos[0] - balls[i].last_pos[0];
        vel[1] = balls[i].pos[1] - balls[i].last_pos[1];
        //vel[0] *= 0.999;  //Gravity
        //vel[1] *= 0.999;
        vel[1] -= 0.2 * (1.0 / g.nphysics);
        balls[i].last_pos[0] = balls[i].pos[0];
        balls[i].last_pos[1] = balls[i].pos[1];
        balls[i].pos[0] += vel[0];
        balls[i].pos[1] += vel[1];

        /*testing boundaries of window
        if (balls[i].pos[0] > (g.xres - balls[i].radius)) {
            balls[i].pos[0] = (g.xres - balls[i].radius);
            balls[i].last_pos[0] = balls[i].pos[0] + fabs(vel[0]);
            //dir = -dir;
        }
        if (balls[i].pos[0] < balls[i].radius) {
            balls[i].pos[0] = balls[i].radius;
            balls[i].last_pos[0] = balls[i].pos[0] - fabs(vel[0]);;
            //dir = -dir;
        }
        if (balls[i].pos[1] < balls[i].radius) {
            balls[i].pos[1] = balls[i].radius;
            balls[i].last_pos[1] = balls[i].pos[1] - fabs(vel[1]);
        }*/

        //check if not in circle
        //x component
        float dx = balls[i].pos[0] - g.bounds[0];
        //y component
        float dy = balls[i].pos[1] - g.bounds[1];
	//optimized code no sqrt
        //float dist = sqrt(xx * xx + yy * yy); //sqrt is inefficient
	float dist = (dx * dx + dy * dy);
	float dist2 = g.radius - balls[i].radius;
	dist2 = dist2 * dist2;
        //float dist = xx * xx + yy * yy;
        if (dist > dist2) { 
            // outside the bounding circle
	    dist = sqrt(dist);
            dx /= dist; //normalize
            dy /= dist;
            balls[i].pos[0] = (dx * (g.radius - balls[i].radius));
            balls[i].pos[1] = (dy * (g.radius - balls[i].radius));
            balls[i].pos[0] += g.bounds[0];
            balls[i].pos[1] += g.bounds[1];
            //balls[i].last_pos[0] = balls[i].pos[0] - fabs(vel[0]);
            //balls[i].last_pos[1] = balls[i].pos[1] - fabs(vel[1]);
        }

        //test for collision with another object
        for (int j = 0; j < g.n; j++) {
            if (i == j) {
                continue;
            }
            float dx = balls[i].pos[0] - balls[j].pos[0];
            float dy = balls[i].pos[1] - balls[j].pos[1];

            float dist = sqrt(dx * dx + dy * dy); //sqrt is inefficient
            float diff = dist - (balls[i].radius + balls[j].radius);
	    //float dist = (dx * dx + dy * dy);
	    //float dist2 = dist - balls[i].radius - balls[j].radius;
	    //dist2 = dist2 * dist2;
	    //float diff = dist - dist2; 
            if (diff < 0.0) {
                //overlap
                //balls[i].last_pos[0] = balls[i].pos[0];
                //balls[i].last_pos[1] = balls[i].pos[1];
                //balls[j].last_pos[0] = balls[j].pos[0];
                //balls[j].last_pos[1] = balls[j].pos[1];


                //normalize the vector between them
		//dist = sqrt(dist);
                dx /= dist;
                dy /= dist;

                balls[i].pos[0] -= dx * (diff / 2);
                balls[i].pos[1] -= dy * (diff / 2);
                balls[j].pos[0] += dx * (diff / 2);
                balls[j].pos[1] += dy * (diff / 2);

            }
        }
    }
}

void render()
{
    static int firsttime = 1;
    const int NV = 20;
    //const int NV = 9;
    static float verts[NV][2];

    if (firsttime == 1) {
        //define the vertices of a ball
        double angle = 0.0;
        double inc = (3.14159 * 2.0) / NV;
        for (int i = 0; i < NV; i++) {
            verts[i][0] = cos(angle);
            verts[i][1] = sin(angle);
            angle += inc;
        }
        firsttime = 0;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    //draw the boundary
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(g.bounds[0], g.bounds[1], 0.0);
    glScalef(g.radius, g.radius, 0.0);
    glBegin(GL_LINE_LOOP);
    for (int j = 0; j < NV; j++) 
        glVertex2fv(verts[j]);
    glEnd();
    glPopMatrix();


    for (int i = 0; i < g.n; i++) {
        //Draw balls.
        glPushMatrix();
        float r = balls[i].color[0];
        float g = balls[i].color[1];
        float b = balls[i].color[2];
        glColor3ub(r, g, b);
        glTranslatef(balls[i].pos[0], balls[i].pos[1], 0.0);
        glScalef(balls[i].radius, balls[i].radius, 0.0);
        //glColor3ubv(balls[i].color.rgb);
        glBegin(GL_TRIANGLE_FAN);
        for (int j = 0; j < NV; j++) 
            glVertex2fv(verts[j]);
        glEnd();
        glPopMatrix();
    }

    Rect r;
    r.bot = g.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0x008877ff, "fps: %i", g.fps);
    ggprint8b(&r, 16, 0x008877ff, "n balls: %i", g.n);
    ggprint8b(&r, 16, 0x008877ff, "n physics: %i", g.nphysics);
}
