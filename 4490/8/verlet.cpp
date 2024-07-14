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

class Global {
    public:
        int xres, yres;
        int n;

        Global() {
            xres = 400;
            yres = 400;
            n = 0;
        }

} g;

class Ball {
    public:
        float radius;
        float pos[2];
        float last_pos[2];
} balls[1000];


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
    XStoreName(dpy, win, "4490 Lab8");
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
            case XK_1:
                //test collisions
                box[0].last_pos[0] = box[0].pos[0] - 5.0;
                box[1].last_pos[0] = box[1].pos[0] + 5.0;
                break;
            case XK_d:
                //Move box to right
                box[0].last_pos[0] = box[0].pos[0] - 5.0;
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
    // Initial position of boxes; left side first
    box[0].pos[0] = 0.0f + box[0].w;
    box[0].pos[1] = g.yres / 2.0f;
    box[0].last_pos[0] = box[0].pos[0];
    box[0].last_pos[1] = box[0].pos[1];
    box[0].color.colorref = 0x00ff0000;

    box[1].pos[0] = g.xres - box[1].w;
    box[1].pos[1] = g.yres / 2.0f;
    box[1].last_pos[0] = box[1].pos[0];
    box[1].last_pos[1] = box[1].pos[1];
    box[1].color.colorref = 0x000000ff;
    g.n = 2;

    //rotate
    float mat[2][2];
    float angle = 1.2;
    //build rotation matrix
    mat[0][0] = cos(angle);
    mat[0][1] = -sin(angle);
    mat[1][0] = sin(angle);
    mat[1][1] = cos(angle);
    //apply rotations
    float Rv[4][2];
    for (int i = 0; i < 4; i++) {
        Rv[i][0] = box[0].verts[i][0] * mat[0][0] + box[0].verts[i][1] * mat[0][1];
        Rv[i][1] = box[0].verts[i][0] * mat[1][0] + box[0].verts[i][1] * mat[1][1];
    }
    for (int i = 0; i < 4; i++) {
        box[0].verts[i][0] = Rv[i][0];
        box[0].verts[i][1] = Rv[i][1];
    }
}

#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define MAX(a, b) ((a) > (b)) ? (a) : (b)

void physics()
{
    //Verlet integration
    //pos[0] += dir;
    float vel[2];
    for (int i = 0; i < g.n; i++) {
        vel[0] = box[i].pos[0] - box[i].last_pos[0];
        vel[1] = box[i].pos[1] - box[i].last_pos[1];
        //vel[0] *= 0.999;  //Gravity
        //vel[1] *= 0.999;
        //vel[1] -= 0.1;
        box[i].last_pos[0] = box[i].pos[0];
        box[i].last_pos[1] = box[i].pos[1];
        box[i].pos[0] += vel[0];
        box[i].pos[1] += vel[1];
        if (box[i].pos[0] > (g.xres - box[i].w)) {
            box[i].pos[0] = (g.xres - box[i].w);
            box[i].last_pos[0] = box[i].pos[0] + fabs(vel[0]);
            //dir = -dir;
        }
        if (box[i].pos[0] < box[i].w) {
            box[i].pos[0] = box[i].w;
            box[i].last_pos[0] = box[i].pos[0] - fabs(vel[0]);;
            //dir = -dir;
        }
        if (box[i].pos[1] < box[i].w) {
            box[i].pos[1] = box[i].w;
            box[i].last_pos[1] = box[i].pos[1] - fabs(vel[1]);
        }
        //test for collision with another object
        for (int j = 0; j < g.n; j++) {
            if (i == j)
                continue;
            //separating axis theorem
            //check each axis
            float proj[2][4];
            float min[2] = {9e9, 9e9};
            float max[2] = {-9e9, -9e9};
            for (int k = 0; k < box[i].nverts; k++) {
                //create a vector
                float v[2] = {box[i].verts[k][0] - box[i].verts[(k + 1) % 4][0], box[i].verts[k][1] - box[i].verts[(k + 1) % 4][1]};
                //find the perpendicular
                float perp[2] = {v[1], -v[0]};
                //project each vertex
                proj[0][k] = perp[0]*box[i].verts[k][0] + perp[1]*box[i].verts[k][1];
                //find the min and max
                if (min[0] > proj[0][k])
                    min[0] = proj[0][k];
                if (max[0] < proj[0][k])
                    max[0] = proj[0][k];
            }
            for (int k = 0; k < box[j].nverts; k++) {
                //create a vector
                float v[2] = {box[j].verts[k][0] - box[j].verts[(k + 1) % 4][0], box[j].verts[k][1] - box[j].verts[(k + 1) % 4][1]};
                //find the perpendicular
                float perp[2] = {v[1], -v[0]};
                //project each vertex
                proj[1][k] = perp[0]*box[j].verts[k][0] + perp[1]*box[j].verts[k][1];
                //find the min and max
                if (min[1] > proj[1][k])
                    min[1] = proj[1][k];
                if (max[1] < proj[1][k])
                    max[1] = proj[1][k];
            }
            //do they overlap?
            if (MIN(max[0], max[1]) > MAX(min[0], min[1])) {
                //overlap!
                box[0].last_pos[0] = box[0].pos[0];
                box[0].last_pos[1] = box[0].pos[1];
                box[1].last_pos[0] = box[1].pos[0];
                box[1].last_pos[1] = box[1].pos[1];

            }  

        }
    }

}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < g.n; i++) {
        //Draw box.
        glPushMatrix();
        glColor3ubv(box[i].color.rgb);
        glTranslatef(box[i].pos[0], box[i].pos[1], 0.0f);
        glBegin(GL_QUADS);
        glVertex2fv(box[i].verts[0]);
        glVertex2fv(box[i].verts[1]);
        glVertex2fv(box[i].verts[2]);
        glVertex2fv(box[i].verts[3]);
        glEnd();
        glPopMatrix();
    }
}






