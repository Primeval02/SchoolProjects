//Revised by: Bradley Tate
//Date: 2/20/24
//
//program: background.cpp
//author:  Gordon Griesel
//date:    2017 - 2018
//
//The position of the background QUAD does not change.
//Just the texture coordinates change.
//In this example, only the x coordinates change.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

//24-bit color:  8 + 8 + 8 = 24
//               R   G   B
//how many colors?  256*256*256 = 16-million+
//
//32-bit color:  8 + 8 + 8     = 24
//               R   G   B   A
//               R   G   B     = 24
//
//char data[1000][3]  aaabbbcccdddeeefff
//char data[1000][4]  aaa bbb ccc ddd eee fff
//
//
class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		name[slen-4] = '\0';
		//printf("name **%s**\n", name);
		char ppmname[80];
		sprintf(ppmname,"%s.ppm", name);
		//printf("ppmname **%s**\n", ppmname);
		char ts[100];
		//system("convert eball.jpg eball.ppm");
		sprintf(ts, "convert %s %s", fname, ppmname);
		system(ts);
		//sprintf(ts, "%s", name);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			while (line[0] == '#')
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		unlink(ppmname);
	}
};
Image img[4] = {"seamless_back.jpg", "bobcat.png", "bobcat.jpeg", "bobcat_t.png"};

class Texture {
public:
	Image *backImage;
	GLuint backTexture;
	float xc[2];
	float yc[2];
};

class Global {
public:
	int xres, yres;
	Texture tex;
    Texture bobcat;
    Texture bobcat2;
    Texture bobcat3;
	Global() {
		xres=640, yres=480;
	}
} g;

class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		setup_screen_res(640, 480);
		dpy = XOpenDisplay(NULL);
		if(dpy == NULL) {
			printf("\n\tcannot connect to X server\n\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if(vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		XSetWindowAttributes swa;
		swa.colormap = cmap;
		swa.event_mask =
			ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
			ButtonPressMask | ButtonReleaseMask |
			StructureNotifyMask | SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
								vi->depth, InputOutput, vi->visual,
								CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
	}
	void cleanupXWindows() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void setup_screen_res(const int w, const int h) {
		g.xres = w;
		g.yres = h;
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		set_title();
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "scrolling background (seamless)");
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	void check_resize(XEvent *e) {
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
} x11;

void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);


//===========================================================================
//===========================================================================
int main()
{
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
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		//-----------------------------------------------
		//get largest color component...
		//*(ptr+3) = (unsigned char)((
		//		(int)*(ptr+0) +
		//		(int)*(ptr+1) +
		//		(int)*(ptr+2)) / 3);
		//d = a;
		//if (b >= a && b >= c) d = b;
		//if (c >= a && c >= b) d = c;
		//*(ptr+3) = d;
		//-----------------------------------------------
		//this code optimizes the commented code above.
        if (a == 255 && b == 255 && c == 255)
		    *(ptr+3) = 0;
        
        else
            *(ptr+3) = 1;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow texture maps
	glEnable(GL_TEXTURE_2D);
	//
	//load the images file into a ppm structure.
	//
	g.tex.backImage = &img[0];
	//create opengl texture elements
	glGenTextures(1, &g.tex.backTexture);
	int w = g.tex.backImage->width;
	int h = g.tex.backImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
							GL_RGB, GL_UNSIGNED_BYTE, g.tex.backImage->data);
	g.tex.xc[0] = 0.0;
	g.tex.xc[1] = 0.25;
	g.tex.yc[0] = 0.0;
	g.tex.yc[1] = 1.0;

    //bobcat
    g.bobcat.backImage = &img[1];
	glGenTextures(1, &g.bobcat.backTexture);
	w = g.bobcat.backImage->width;
	h = g.bobcat.backImage->height;
	glBindTexture(GL_TEXTURE_2D, g.bobcat.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
							GL_RGB, GL_UNSIGNED_BYTE, g.bobcat.backImage->data);
	g.bobcat.xc[0] = 0.0;
	g.bobcat.xc[1] = 1.0;
	g.bobcat.yc[0] = 0.0;
	g.bobcat.yc[1] = 1.0;
    
    //bobcat 2
    g.bobcat2.backImage = &img[2];
	glGenTextures(1, &g.bobcat2.backTexture);
	w = g.bobcat2.backImage->width;
	h = g.bobcat2.backImage->height;
	glBindTexture(GL_TEXTURE_2D, g.bobcat2.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
							GL_RGB, GL_UNSIGNED_BYTE, g.bobcat2.backImage->data);
	g.bobcat2.xc[0] = 0.0;
	g.bobcat2.xc[1] = 1.0;
	g.bobcat2.yc[0] = 0.0;
	g.bobcat2.yc[1] = 1.0;
    
    //bobcat 3
    g.bobcat3.backImage = &img[3];
	glGenTextures(1, &g.bobcat3.backTexture);
	w = g.bobcat3.backImage->width;
	h = g.bobcat3.backImage->height;
	glBindTexture(GL_TEXTURE_2D, g.bobcat3.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *data2 = buildAlphaData(&img[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, data2);
	g.bobcat3.xc[0] = 0.0;
	g.bobcat3.xc[1] = 1.0;
	g.bobcat3.yc[0] = 0.0;
	g.bobcat3.yc[1] = 1.0;


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
		if (key == XK_Escape) {
			return 1;
		}
	}
	return 0;
}

void physics()
{
	//move the background
	g.tex.xc[0] += 0.001;
	g.tex.xc[1] += 0.001;
}

void render()
{
    
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(g.tex.xc[0], g.tex.yc[1]); glVertex2i(0, 0);
		glTexCoord2f(g.tex.xc[0], g.tex.yc[0]); glVertex2i(0, g.yres);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
	glEnd();

    //draw bobcat
	glBindTexture(GL_TEXTURE_2D, g.bobcat.backTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(g.bobcat.xc[0], g.bobcat.yc[1]); glVertex2i(0, 0);
		glTexCoord2f(g.bobcat.xc[0], g.bobcat.yc[0]); glVertex2i(0, 100);
		glTexCoord2f(g.bobcat.xc[1], g.bobcat.yc[0]); glVertex2i(100, 100);
		glTexCoord2f(g.bobcat.xc[1], g.bobcat.yc[1]); glVertex2i(100, 0);
	glEnd();

    //draw bobcat 2
	glBindTexture(GL_TEXTURE_2D, g.bobcat2.backTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(g.bobcat2.xc[0], g.bobcat2.yc[1]); glVertex2i(100, 0);
		glTexCoord2f(g.bobcat2.xc[0], g.bobcat2.yc[0]); glVertex2i(100, 100);
		glTexCoord2f(g.bobcat2.xc[1], g.bobcat2.yc[0]); glVertex2i(200, 100);
		glTexCoord2f(g.bobcat2.xc[1], g.bobcat2.yc[1]); glVertex2i(200, 0);
	glEnd();

    //draw bobcat 3 transarent
	glBindTexture(GL_TEXTURE_2D, g.bobcat3.backTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255, 255, 255, 255);
	glBegin(GL_QUADS);
		glTexCoord2f(g.bobcat3.xc[0], g.bobcat3.yc[1]); glVertex2i(200, 0);
		glTexCoord2f(g.bobcat3.xc[0], g.bobcat3.yc[0]); glVertex2i(200, 100);
		glTexCoord2f(g.bobcat3.xc[1], g.bobcat3.yc[0]); glVertex2i(300, 100);
		glTexCoord2f(g.bobcat3.xc[1], g.bobcat3.yc[1]); glVertex2i(300, 0);
	glEnd();
    glDisable(GL_ALPHA_TEST);

}



