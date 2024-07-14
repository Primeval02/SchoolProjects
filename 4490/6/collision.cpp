//revised by: Bradley Tate
//date: February 2024
//
//author: Gordon Griesel
//date: April 19, 2016
//      updated feb 2018
//circle-box collision
//circle-polygon collision
//
//Key presses:
//
//S - enlarge the sphere. (shift-S to reduce)
//Arrow keys - move sphere
//Mouse drag sphere with left button
//
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

typedef float Flt;

const int N_CIRCLE_SEGMENTS = 24;
const int N_POLYGON_SEGMENTS = 10;
//
#define PI 3.14159265358979
#define SWAP(x,y) (x)^=(y);(y)^=(x);(x)^=(y)
#define rnd() (Flt)rand() / (Flt)RAND_MAX
//
struct Circle {
	Flt pos[2];
	Flt vel[2];
	Flt radius, radius2;
	Flt mass;
	Flt vert[40][2];
	int nverts;
	Circle() {
		vel[0] = vel[1] = 0.0f;
	}
};

struct Polygon {
	Flt pos[2];
	Flt vel[2];
	Flt width, height;
	Flt vert[100][2];
	int nverts;
	Flt mass[200][2];
	int nmasses;
	Polygon() {
		vel[0] = vel[1] = 0.0f;
	}
};

class Global {
public:
	int xres, yres;
	Flt fxres, fyres;
	Flt gravity;
	int lbuttonDown;
	Circle circle;
	Polygon polygon[2];
	int ncircles;
	int npolygons;
	Global() {
		gravity = 0.0006;
		xres = 800, yres = 600;
		fxres=(Flt)xres, fyres=(Flt)yres;
		ncircles = 1;
		npolygons = 2;
		lbuttonDown = 0;
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
		sprintf(ts, "Collision Demo  drag circle w/mouse %i x %i",
			g.xres, g.yres);
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
	srand((unsigned)time(NULL));
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
	//build points on a circle
	int i, n = N_CIRCLE_SEGMENTS;
	float ang = 0.0;
	float inc = (PI*2.0)/(float)n;

	static int firsttime=1;
	if (firsttime) {
		firsttime=0;
		g.circle.radius = (Flt)(g.xres / 12);
		g.circle.pos[0] = 110.0;
		g.circle.pos[1] = g.yres - 450.0;
	}
	g.circle.nverts=0;
	for (i=0; i<n; i++) {
		g.circle.vert[g.circle.nverts][0] = cos(ang);
		g.circle.vert[g.circle.nverts][1] = sin(ang);
		g.circle.nverts++;
		ang += inc;
	}
	g.circle.radius2 = g.circle.radius * g.circle.radius;
	//
	//build polygons
	g.polygon[0].pos[0] = 200.0;
	g.polygon[0].pos[1] = g.fyres - 300.0;
	//
	g.polygon[0].width = 175.0;
	g.polygon[0].height = 50.0;
	g.polygon[0].vert[0][0] = -g.polygon[0].width;
	g.polygon[0].vert[1][0] = -g.polygon[0].width;
	g.polygon[0].vert[2][0] =  g.polygon[0].width + 50.0;
	g.polygon[0].vert[3][0] =  g.polygon[0].width + 50.0;
	g.polygon[0].vert[0][1] = -g.polygon[0].height;
	g.polygon[0].vert[1][1] =  g.polygon[0].height;
	g.polygon[0].vert[2][1] =  g.polygon[0].height;
	g.polygon[0].vert[3][1] = -g.polygon[0].height + 75.0;
	g.polygon[0].nverts = 4;
	//
    //
  	g.polygon[1].pos[0] = 350.0;
	g.polygon[1].pos[1] = g.fyres - 60.0;
	//
	g.polygon[1].width = 200.0;
	g.polygon[1].height = 32.0;
	g.polygon[1].vert[0][0] = -g.polygon[1].width + 50.0;
	g.polygon[1].vert[1][0] = -g.polygon[1].width + 50.0;
	g.polygon[1].vert[2][0] =  g.polygon[1].width + 100.0;
	g.polygon[1].vert[3][0] =  g.polygon[1].width + 100.0;
	g.polygon[1].vert[0][1] = -g.polygon[1].height - 25.0;
	g.polygon[1].vert[1][1] =  g.polygon[1].height;
	g.polygon[1].vert[2][1] =  g.polygon[1].height;
	g.polygon[1].vert[3][1] = -g.polygon[1].height - 100.0;
	g.polygon[1].nverts = 5;

	//
	//build masses
	g.polygon[0].nmasses = 0;
    g.polygon[1].nmasses = 0;
	//int nsteps = 4;
	int nsteps = N_POLYGON_SEGMENTS;
	Flt step[2];
	n = 0;
	//this steps between each vertex and adds masses.
	for (int i=0; i<g.polygon[0].nverts; i++) {
		int j = (i+1) % g.polygon[0].nverts;
		step[0] = (g.polygon[0].vert[j][0] - g.polygon[0].vert[i][0]) / (Flt)nsteps;
		step[1] = (g.polygon[0].vert[j][1] - g.polygon[0].vert[i][1]) / (Flt)nsteps;
		//step from vertex to vertex adding masses along the way.
		for (int j=0; j<nsteps; j++) {
			g.polygon[0].mass[n][0] = g.polygon[0].vert[i][0] + (step[0] * (Flt)j);
			g.polygon[0].mass[n][1] = g.polygon[0].vert[i][1] + (step[1] * (Flt)j);
			n++;
		}
	}
	for (int i=0; i<g.polygon[1].nverts; i++) {
		int j = (i+1) % g.polygon[1].nverts;
		step[0] = (g.polygon[1].vert[j][0] - g.polygon[1].vert[i][0]) / (Flt)nsteps;
		step[1] = (g.polygon[1].vert[j][1] - g.polygon[1].vert[i][1]) / (Flt)nsteps;
		//step from vertex to vertex adding masses along the way.
		for (int j=0; j<nsteps; j++) {
			g.polygon[1].mass[n][0] = g.polygon[1].vert[i][0] + (step[0] * (Flt)j);
			g.polygon[1].mass[n][1] = g.polygon[1].vert[i][1] + (step[1] * (Flt)j);
			n++;
		}
	}

	g.polygon[0].nmasses = n;
    g.polygon[1].nmasses = n;
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
				g.circle.vel[0] = (Flt)(e->xbutton.x - g.circle.pos[0])*0.1;
				g.circle.vel[1] = (Flt)(e->xbutton.y - g.circle.pos[1])*0.1;
				g.circle.pos[0] = e->xbutton.x;
				g.circle.pos[1] = e->xbutton.y;
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
	//a key was pressed
	switch (key) {
		case XK_s:
			if (shift) {
				g.circle.radius *= (1.0 / 1.01);
			} else {
				g.circle.radius *= 1.01;
			}
			g.circle.radius2 = g.circle.radius * g.circle.radius;
			break;
		case XK_Up:
			g.circle.pos[1] -= 4.0;
			break;
		case XK_Down:
			g.circle.pos[1] += 4.0;
			break;
		case XK_Left:
			g.circle.pos[0] -= 4.0;
			break;
		case XK_Right:
			g.circle.pos[0] += 4.0;
			break;
		case XK_Escape:
			//quitting the program
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

void show_circle()
{
	x11.set_color_3i(100,255,100);
	for (int i=0; i<g.circle.nverts; i++) {
		int j = i+1;
		if (j >= g.circle.nverts)
			j -= g.circle.nverts;
		myBresenhamLine(
			g.circle.pos[0] + g.circle.vert[i][0] * g.circle.radius,
			g.circle.pos[1] + g.circle.vert[i][1] * g.circle.radius,
			g.circle.pos[0] + g.circle.vert[j][0] * g.circle.radius,
			g.circle.pos[1] + g.circle.vert[j][1] * g.circle.radius);
	}
}

void show_polygons()
{
	x11.set_color_3i(240,240,0);
	for (int i=0; i<g.polygon[0].nverts; i++) {
		int j = i+1;
		if (j >= g.polygon[0].nverts) j -= g.polygon[0].nverts;
		myBresenhamLine(
			g.polygon[0].pos[0] + g.polygon[0].vert[i][0],
			g.polygon[0].pos[1] + g.polygon[0].vert[i][1],
			g.polygon[0].pos[0] + g.polygon[0].vert[j][0],
			g.polygon[0].pos[1] + g.polygon[0].vert[j][1]);
	}
	for (int i=0; i<g.polygon[0].nmasses; i++) {
		show_anchor(
			(int)(g.polygon[0].pos[0] + g.polygon[0].mass[i][0]),
			(int)(g.polygon[0].pos[1] + g.polygon[0].mass[i][1]), 2
		);
	}
	for (int i=0; i<g.polygon[1].nverts; i++) {
		int j = i+1;
		if (j >= g.polygon[1].nverts) j -= g.polygon[1].nverts;
		myBresenhamLine(
			g.polygon[1].pos[0] + g.polygon[1].vert[i][0],
			g.polygon[1].pos[1] + g.polygon[1].vert[i][1],
			g.polygon[1].pos[0] + g.polygon[1].vert[j][0],
			g.polygon[1].pos[1] + g.polygon[1].vert[j][1]);
	}
	for (int i=0; i<g.polygon[1].nmasses; i++) {
		show_anchor(
			(int)(g.polygon[1].pos[0] + g.polygon[1].mass[i][0]),
			(int)(g.polygon[1].pos[1] + g.polygon[1].mass[i][1]), 2
		);
	}


}

void physics()
{
	//move circle
	if (!g.lbuttonDown) {
		g.circle.pos[0] += g.circle.vel[0];
		g.circle.pos[1] += g.circle.vel[1];
		//gravity...
		g.circle.vel[1] += g.gravity;
	}
	//collision with floor
	Flt penalty;
	Flt diff = g.circle.radius - (g.fyres - g.circle.pos[1]);
	if (diff > g.gravity) {
		penalty = diff;
		g.circle.vel[1] -= penalty * 0.01;
		g.circle.vel[0] *= 0.99;
		g.circle.vel[1] *= 0.98;
	}
	//circle collision with object masses
	Flt d0, d1, dist;
	for (int i=0; i<g.polygon[0].nmasses; i++) {
		//distance from mass
		d0 = (g.polygon[0].mass[i][0]+g.polygon[0].pos[0]) - g.circle.pos[0];
		d1 = (g.polygon[0].mass[i][1]+g.polygon[0].pos[1]) - g.circle.pos[1];
		dist = (d0*d0 + d1*d1);
		d0 /= dist;
		d1 /= dist;
		diff = g.circle.radius2 - dist;
		if (diff > g.gravity) {
			penalty = diff;
			g.circle.vel[0] -= penalty * d0 * 0.01;
			g.circle.vel[1] -= penalty * d1 * 0.01;
			g.circle.vel[0] *= 0.994;
			g.circle.vel[1] *= 0.994;
		}
	}
	for (int i=0; i<g.polygon[1].nmasses; i++) {
		//distance from mass
		d0 = (g.polygon[1].mass[i][0]+g.polygon[1].pos[0]) - g.circle.pos[0];
		d1 = (g.polygon[1].mass[i][1]+g.polygon[1].pos[1]) - g.circle.pos[1];
		dist = (d0*d0 + d1*d1);
		d0 /= dist;
		d1 /= dist;
		diff = g.circle.radius2 - dist;
		if (diff > g.gravity) {
			penalty = diff;
			g.circle.vel[0] -= penalty * d0 * 0.01;
			g.circle.vel[1] -= penalty * d1 * 0.01;
			g.circle.vel[0] *= 0.994;
			g.circle.vel[1] *= 0.994;
		}
	}

	//constant friction
	g.circle.vel[0] *= 0.9996;
	g.circle.vel[1] *= 0.9996;
}

void render()
{
	x11.clear_screen();
	show_circle();
	show_polygons();
}



















