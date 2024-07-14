//
//author: Gordon Griesel
//date: Feb 20, 2018
//simulation of orbital physics
//
//Assignment is to put the satellite in orbit around the planet.
//Do not let the satellite crash into the planet.
//Show a trail indicating the satellite's path.
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
//
#define PI 3.14159265358979
#define SWAP(x,y) (x)^=(y);(y)^=(x);(x)^=(y)
#define rnd() (Flt)rand() / (Flt)RAND_MAX
//
class Circle {
public:
	Flt pos[2];
	Flt vel[2];
	Flt radius, radius2;
	Flt mass;
	Flt vert[40][2];
	int nverts;
	Circle() { }
	void init(int n, int rad, int x, int y) {
		float ang = 0.0;
		float inc = (PI*2.0)/(float)n;
		radius = rad;
		pos[0] = x;
		pos[1] = y;
		nverts = 0;
		for (int i=0; i<n; i++) {
			vert[nverts][0] = cos(ang);
			vert[nverts][1] = sin(ang);
			nverts++;
			ang += inc;
		}
		radius2 = radius * radius;
		vel[0] = vel[1] = 0.0f;
	}
	void setVel(Flt x, Flt y) {
		vel[0] = x;
		vel[1] = y;
	}
	void move() {
		pos[0] += vel[0];
		pos[1] += vel[1];
	}
};

class Global {
public:
	int xres, yres;
	Circle planet;
	Circle satellite;
	Global() {
		xres = 800, yres = 600;
		planet.init(32, 80, 400, 300);
		satellite.init(10, 6, 100, 100);
		satellite.setVel(0.05, 0.0);
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
		sprintf(ts, "Put the satellite in orbit.");
		XStoreName(dpy, win, ts);
	}
	void check_resize(XEvent *e) {
		//ConfigureNotify is sent when the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		g.xres = xce.width;
		g.yres = xce.height;
		set_window_title();
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
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//left button is down
		}
		if (e->xbutton.button==3) {
		}
	}
	if (e->type == MotionNotify) {
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
			//mouse moved
			savex = e->xbutton.x;
			savey = e->xbutton.y;
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
		case XK_Up:
			break;
		case XK_Down:
			break;
		case XK_Left:
			break;
		case XK_Right:
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

void show_circle(Circle *c)
{
	x11.set_color_3i(100,255,100);
	for (int i=0; i<c->nverts; i++) {
		int j = i+1;
		if (j >= c->nverts)
			j -= c->nverts;
		myBresenhamLine(
			c->pos[0] + c->vert[i][0] * c->radius,
			c->pos[1] + c->vert[i][1] * c->radius,
			c->pos[0] + c->vert[j][0] * c->radius,
			c->pos[1] + c->vert[j][1] * c->radius);
	}
}

void physics()
{
	g.satellite.move();
}

void render()
{
	x11.clear_screen();
	show_circle(&g.planet);
	show_circle(&g.satellite);
}



















