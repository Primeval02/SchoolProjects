//Modified by: Bradley Tate
//Date: February 2024
//
//author: Gordon Griesel
//date: Feb 2018
//line-line intersection
//
//Draw an indication when and where the lines intersect.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/Xdbe.h>
#include <bits/stdc++.h>
//#define DEBUG_ON
using namespace std;
typedef float Flt;
typedef Flt Vec[2];
#define PI 3.14159265358979
#define SWAP(x,y) (x)^=(y);(y)^=(x);(x)^=(y)
#define rnd() (Flt)rand() / (Flt)RAND_MAX
#define pdd pair<double, double>

struct Point {
	Flt pos[2];
	Flt vel[2];
	Point() { }
};

class Global {
public:
	int xres, yres;
	Flt fxres, fyres;
	int lbuttonDown;
	Point p[4];
	int overlap;
	Global() {
		xres = 640, yres = 640;
		fxres=(Flt)xres, fyres=(Flt)yres;
		lbuttonDown = 0;
		#ifdef DEBUG_ON
		srand(7);
		#else
		srand((unsigned)time(NULL));
		#endif
		for (int i=0; i<4; i++) {
			p[i].pos[0] = rand() % xres;
			p[i].pos[1] = rand() % yres;
			p[i].vel[0] = rnd() * 0.2 - 0.1;
			p[i].vel[1] = rnd() * 0.2 - 0.1;
		}
		overlap = 0;
	}
} g;

class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GC gc;
	XdbeBackBuffer backBuffer;
	XdbeSwapInfo swapInfo;
public:
	~X11_wrapper() {
		//Deallocate back buffer
		if (!XdbeDeallocateBackBufferName(dpy, backBuffer)) {
			fprintf(stderr,"Error : unable to deallocate back buffer.\n");
		}
		XFreeGC(dpy, gc);
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	X11_wrapper() {
		int major, minor;
		XSetWindowAttributes attributes;
		XdbeBackBufferAttributes *backAttr;
		dpy = XOpenDisplay(NULL);
	    //List of events we want to handle
		attributes.event_mask = ExposureMask | StructureNotifyMask |
			PointerMotionMask | ButtonPressMask |
			ButtonReleaseMask | KeyPressMask | KeyReleaseMask;
		//Various window attributes
		attributes.backing_store = Always;
		attributes.save_under = True;
		attributes.override_redirect = False;
		attributes.background_pixel = 0x00000000;
		//Get default root window
		Window root = DefaultRootWindow(dpy);
		//Create a window
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
		    CopyFromParent, InputOutput, CopyFromParent,
		    CWBackingStore | CWOverrideRedirect | CWEventMask |
			CWSaveUnder | CWBackPixel, &attributes);
		//Create gc
		gc = XCreateGC(dpy, win, 0, NULL);
		//Get DBE version
		if (!XdbeQueryExtension(dpy, &major, &minor)) {
			fprintf(stderr, "Error : unable to fetch Xdbe Version.\n");
			XFreeGC(dpy, gc);
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
		}
		printf("Xdbe version %d.%d\n",major,minor);
		//Get back buffer and attributes (used for swapping)
		backBuffer = XdbeAllocateBackBufferName(dpy, win, XdbeUndefined);
		backAttr = XdbeGetBackBufferAttributes(dpy, backBuffer);
	    swapInfo.swap_window = backAttr->window;
	    swapInfo.swap_action = XdbeUndefined;
		XFree(backAttr);
		//Map and raise window
		set_window_title();
		XMapWindow(dpy, win);
		XRaiseWindow(dpy, win);
	}
	void set_window_title() {
		char ts[256];
		sprintf(ts, "line/line intersection");
		XStoreName(dpy, win, ts);
	}
	void check_resize(XEvent *e) {
		//ConfigureNotify is sent when the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		g.xres = xce.width;
		g.yres = xce.height;
		g.fxres = (Flt)g.xres;
		g.fyres = (Flt)g.yres;
		//init();
		set_window_title();
		//g.circle.pos[0] = 0.0;
		//g.circle.pos[1] = 0.0;
	}
	void clear_screen()	{
		XSetForeground(dpy, gc, 0x00050505);
		XFillRectangle(dpy, backBuffer, gc, 0, 0, g.xres, g.yres);
	}
	void set_color_3i(int r, int g, int b) {
		unsigned long cref = 0L;
		cref += r;
		cref <<= 8;
		cref += g;
		cref <<= 8;
		cref += b;
		XSetForeground(dpy, gc, cref);
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void drawPoint(int x, int y) {
		XDrawPoint(dpy, backBuffer, gc, x, y);
	}
	void swapBuffers() { XdbeSwapBuffers(dpy, &swapInfo, 1); }
} x11;

//prototypes
void init();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

int main()
{
	init();
	int done = 0;
	while (!done) {
		//Handle all events in queue...
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		//Process physics and rendering every frame
		physics();
		render();
		x11.swapBuffers();
		usleep(512);
	}
	return 0;
}

void init()
{

}

void check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type != ButtonRelease &&
			e->type != ButtonPress &&
			e->type != MotionNotify)
		return;
	if (e->type == ButtonRelease) {
		g.lbuttonDown = 0;
		return;
	}
	if (e->type == ButtonPress) {
		//Log("ButtonPress %i %i\n", e->xbutton.x, e->xbutton.y);
		if (e->xbutton.button==1) {
			//left button is down
			g.lbuttonDown = 1;
		}
		if (e->xbutton.button==3) {
			//Adding a new control point...
		}
	}
	if (e->type == MotionNotify) {
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
			//mouse moved
			savex = e->xbutton.x;
			savey = e->xbutton.y;
			if (g.lbuttonDown) {

			}
		}
	}
}

int check_keys(XEvent *e)
{
	static int shift = 0;
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyRelease) {
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift = 0;
			return 0;
		}
	}
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift = 1;
		return 0;
	}
	(void)shift;
	//a key was pressed
	switch (key) {
		case XK_s:
			break;
		case XK_Up: break;
		case XK_Down: break;
		case XK_Left: break;
		case XK_Right: break;
		case XK_Escape:
			return 1;
	}
	return 0;
}

void myBresenhamLine(int x0, int y0, int x1, int y1)
{
	//Bresenham line algorithm, integers only.
	int steep = (abs(y1-y0) > abs(x1-x0));
	if (steep) {
		SWAP(x0, y0);
		SWAP(x1, y1);
	}
	if (x0 > x1) {
		SWAP(x0, x1);
		SWAP(y0, y1);
	}
	int ystep = (y1>y0) ? 1 : -1;
	int yDiff = abs(y1-y0);
	int xDiff = x1 - x0;
	int err=xDiff>>1;
	int x, y=y0;
	for	(x=x0; x<=x1; x++) {
		if (steep)
			x11.drawPoint(y, x);
		else
			x11.drawPoint(x, y);
		err -= yDiff;
		if (err <= 0) {
			y += ystep;
			err += xDiff;
		}
	}
}

void show_anchor(int x, int y, int size)
{
	//draw a small rectangle...
	int i,j;
	for (i=-size; i<=size; i++) {
		for (j=-size; j<=size; j++) {
			x11.drawPoint(x+j, y+i);
		}
	}
}

void show_line(Point p0, Point p1)
{
	x11.set_color_3i(100,255,100);
	if (g.overlap) {
		x11.set_color_3i(255,0,0);
    }
	myBresenhamLine( p0.pos[0], p0.pos[1], p1.pos[0], p1.pos[1]);
}

Flt dotProduct(Vec v0, Vec v1) {
	return (v0[0]*v1[0] + v0[1]*v1[1]);
}

void physics()
{
	for (int i=0; i<4; i++) {
		#ifndef DEBUG_ON
		g.p[i].pos[0] += g.p[i].vel[0];
		g.p[i].pos[1] += g.p[i].vel[1];
		#endif
		//check for off screen
		if (g.p[i].pos[0] < 0.0) {
			g.p[i].pos[0] = 0.0;
			if (g.p[i].vel[0] < 0.0) {
				g.p[i].vel[0] = -g.p[i].vel[0];
				g.p[i].vel[0] += rnd() * 0.01 - 0.005;
			}
		}
		if (g.p[i].pos[1] < 0.0) {
			g.p[i].pos[1] = 0.0;
			if (g.p[i].vel[1] < 0.0) {
				g.p[i].vel[1] = -g.p[i].vel[1];
				g.p[i].vel[1] += rnd() * 0.01 - 0.005;
			}
		}
		if (g.p[i].pos[0] >= g.xres) {
			g.p[i].pos[0] = g.xres-1;
			if (g.p[i].vel[0] > 0.0) {
				g.p[i].vel[0] = -g.p[i].vel[0];
				g.p[i].vel[0] += rnd() * 0.01 - 0.005;
			}
		}
		if (g.p[i].pos[1] >= g.yres) {
			g.p[i].pos[1] = g.yres-1;
			if (g.p[i].vel[1] > 0.0) {
				g.p[i].vel[1] = -g.p[i].vel[1];
				g.p[i].vel[1] += rnd() * 0.01 - 0.005;
			}
		}
	}
	//set color of lines
	//check for lines overlapping...
	int over1, over2;
	{
		//1. use first line to define half spaces
		//make some vectors
		//get a perpendicular vector
		//are the dot products positive or negative?
		//different dot products means line end-points are on different sides
		//of the other line segment. If both line segments meet this condition,
		//the lines intersect.
		Vec v0 = {g.p[1].pos[0]-g.p[0].pos[0],g.p[1].pos[1]-g.p[0].pos[1]};
		Vec v1 = {g.p[2].pos[0]-g.p[0].pos[0],g.p[2].pos[1]-g.p[0].pos[1]};
		Vec v2 = {g.p[3].pos[0]-g.p[0].pos[0],g.p[3].pos[1]-g.p[0].pos[1]};
		Vec perp = { v0[1], -v0[0] };
		Flt dot1 = dotProduct(perp, v1);
		Flt dot2 = dotProduct(perp, v2);
		int sign1 = (dot1 < 0.0) ? -1 : 1;
		int sign2 = (dot2 < 0.0) ? -1 : 1;
		over1 = (sign1 != sign2);
	}
	{
		//2. use second line to define half spaces
		Vec v0 = {g.p[2].pos[0]-g.p[3].pos[0],g.p[2].pos[1]-g.p[3].pos[1]};
		Vec v1 = {g.p[0].pos[0]-g.p[3].pos[0],g.p[0].pos[1]-g.p[3].pos[1]};
		Vec v2 = {g.p[1].pos[0]-g.p[3].pos[0],g.p[1].pos[1]-g.p[3].pos[1]};
		Vec perp = { v0[1], -v0[0] };
		Flt dot1 = dotProduct(perp, v1);
		Flt dot2 = dotProduct(perp, v2);
		int sign1 = (dot1 < 0.0) ? -1 : 1;
		int sign2 = (dot2 < 0.0) ? -1 : 1;
		over2 = (sign1 != sign2);
	}
	g.overlap = (over1 && over2);

}

//Intersection Function Credit:
//https://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/
pdd lineIntersection(pdd A, pdd B, pdd C, pdd D) 
{
    double a1 = B.second - A.second;
    double b1 = A.first - B.first;
    double c1 = a1 * (A.first) + b1 * (A.second);

    double a2 = D.second - C.second;
    double b2 = C.first - D.first;
    double c2 = a2 * (C.first) + b2 * (C.second);

    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) {
        return make_pair(FLT_MAX, FLT_MAX);
    }
    else {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        return make_pair(x, y);
    }
}

void render()
{
	x11.clear_screen();
	show_line(g.p[0], g.p[1]);
	show_line(g.p[2], g.p[3]);

    if (g.overlap) {
        pdd A = make_pair(g.p[0].pos[0], g.p[0].pos[1]);
        pdd B = make_pair(g.p[1].pos[0], g.p[1].pos[1]);
        pdd C = make_pair(g.p[2].pos[0], g.p[2].pos[1]);
        pdd D = make_pair(g.p[3].pos[0], g.p[3].pos[1]);
        pdd intersection = lineIntersection(A, B, C, D);
        //printf("Intersection at: (%f, %f)", intersection.first, intersection.second);
        x11.set_color_3i(255, 255, 255); show_anchor(intersection.first, intersection.second, 2);
    }

	//show end-points
	x11.set_color_3i(255,255,255); show_anchor(g.p[0].pos[0],g.p[0].pos[1],2);
	x11.set_color_3i(255,  0,  0); show_anchor(g.p[1].pos[0],g.p[1].pos[1],2);
	x11.set_color_3i(  0,255,  0); show_anchor(g.p[2].pos[0],g.p[2].pos[1],2);
	x11.set_color_3i(255,255,  0); show_anchor(g.p[3].pos[0],g.p[3].pos[1],2);
}



















