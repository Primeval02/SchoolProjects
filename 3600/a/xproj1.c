/*
 cs3600 Spring 2022
 original author:  Gordon Griesel
         Student:  Bradley Tate
   Original date:  Jan 24, 2022
    Updated Date:  Aug 25, 2022
         purpose:  C program for students to practice their C programming
		           over the Winter break. Also, introduce students to the
                   X Window System. We will use the X Window protocol or
                   API to generate output in some of our lab and homework
                   assignments.

 Instructions:

      1. If you make changes to this file, put your name at the top of
	     the file. Use one C style multi-line comment to hold your full
		 name. Do not remove the original author's name from this or 
		 other source files please.

      2. Build and run this program by using the provided Makefile.

	     At the command-line enter make.
		 Run the program by entering ./a.out
		 Quit the program by pressing Esc.

         The compile line will look like this:
            gcc xwin89.c -Wall -Wextra -Werror -pedantic -ansi -lX11

		 To run this program on the Odin server, you will have to log in
		 using the -YC option. Example: ssh myname@odin.cs.csub.edu -YC

      3. See the assignment page associated with this program for more
	     instructions.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <unistd.h>

struct Global {
	Display *dpy;
	Window win;
	GC gc;
	int xres, yres;
} g;

void x11_cleanup_xwindows(void);
void x11_init_xwindows(void);
void x11_clear_window(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void render(int pid);
int pid = 1;

char **my_argv;
char **my_envp;
int child = 0;
int pos[2];

int main(int argc, char *argv[], char *envp[])
{
    my_argv = argv;
    my_envp = envp;
    if (argc > 1) {
        printf("I am the child\n");
        child = 1;
    }

	XEvent e;
	int done = 0;
	x11_init_xwindows();
	while (!done) {
		/* Check the event queue */
		while (XPending(g.dpy)) {
			XNextEvent(g.dpy, &e);
			check_mouse(&e);
			done = check_keys(&e);
			render(pid);
		}
		usleep(4000);
	}
	x11_cleanup_xwindows();
	return 0;
}

void x11_cleanup_xwindows(void)
{
	XDestroyWindow(g.dpy, g.win);
	XCloseDisplay(g.dpy);
}

void x11_init_xwindows(void)
{
	int scr;

	if (!(g.dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: could not open display!\n");
		exit(EXIT_FAILURE);
	}
	scr = DefaultScreen(g.dpy);
	g.xres = 400;
	g.yres = 200;
	g.win = XCreateSimpleWindow(g.dpy, RootWindow(g.dpy, scr), 1, 1,
							g.xres, g.yres, 0, 0x00ffffff, 0x00000000);
	XStoreName(g.dpy, g.win, "cs3600 xwin sample");
	g.gc = XCreateGC(g.dpy, g.win, 0, NULL);
	XMapWindow(g.dpy, g.win);
	XSelectInput(g.dpy, g.win, ExposureMask | StructureNotifyMask |
								PointerMotionMask | ButtonPressMask |
								ButtonReleaseMask | KeyPressMask);
}

void drawRect(int x, int y, int w, int h) 
{
    XFillRectangle(g.dpy, g.win, g.gc, x, y, w, h);
}

void check_mouse(XEvent *e)
{
    static int count = 0;
	static int savex = 0;
	static int savey = 0;
	int mx = e->xbutton.x;
	int my = e->xbutton.y;

	if (e->type != ButtonPress
		&& e->type != ButtonRelease
		&& e->type != MotionNotify)
		return;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) { }
		if (e->xbutton.button==3) { }
	}
	if (e->type == MotionNotify) {
		if (savex != mx || savey != my) {
			/*mouse moved*/
			savex = mx;
			savey = my;
            pos[0] = mx;
            pos[1] = my;
            if (++count > 20) {
                (child) ? printf("c") : printf("m");
                /* write(1, "m", 1); prints m to the terminal and appears to flush and work as intended*/
                fflush(stdout);
                count = 0;
            }
            /*printf("m");
             *fflush(stdout);
             *Will work as well*/
		}
	}
}

void make_child_process()
{
    static int nchildren = 0;
    if (nchildren == 0) {
        printf("fork\n");
        fflush(stdout);
        nchildren++;
        pid = fork();
        if (pid == 0) {
            char *argv[3] = {my_argv[0], "C", NULL};
            execve(my_argv[0], argv, my_envp);
            //main();
        } else {

        }
    }
}

int check_keys(XEvent *e)
{
	int key;
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyPress) {
		switch (key) {
			case XK_1:
				break;
            case XK_c:
                make_child_process();
                break;
			case XK_Escape:
				return 1;
		}
	}
	return 0;
}

void render(int pid)
{
    if (child == 1) {
        XSetForeground(g.dpy, g.gc, 0x00ff9999);  /*Got from https://linux.die.net/man/3/xsetforeground*/
    }
    else {

        XSetForeground(g.dpy, g.gc, 0x00ff9900);  /*Got from https://linux.die.net/man/3/xsetforeground*/
    }
    //XFillRectangle(g.dpy, g.win, g.gc, 0, 0, g.xres, g.yres); /*Got from https://www.x.org/releases/X11R7.6/doc/man/man3/XFillRectangle.3.xhtml*/
    drawRect(0, 0, g.xres, g.yres);
    XSetForeground(g.dpy, g.gc, 0x000000ff);
    drawRect(pos[0] - 10, pos[1] - 10, 20, 20);
}

