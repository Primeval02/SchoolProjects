//Name: Bradley Tate
//Date: March 2024
//Description: Lane based tower defense based on Popcap's PvZ

#include <iostream>
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
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <ctime>
#include "fonts.h"

using namespace std;

//CHANGE POINT TO PLANT / ZOMBIE STRUCT
struct Plant {
	//int 1-5 (Sunflower, Peashooter, Wallnut, Repeater, DoomShroom)
	int plantType;
	pair <int, int> location; //plant's location on grid space
	int n; //plant number
	time_t timer; //Plant actions
	Plant() { 
		timer = 0;
		n = 0;
	}
}; 

Plant p[45];

struct Zombie {
	int zombieType;
	pair <int, int> location; 
	Zombie() {
	}
};

Zombie z[45];

struct Sun {
	float x, y;
	bool collected;
	int n;
	Sun() {
		x = 0;
		y = 0;
		n = 0;
		collected = false;
	}
};

Sun suns[20];

struct Pea {
	float x, y;
	Pea() {
		x = 0;
		y = 0;
	}	
};

Pea peas[50];

//Classes for texturing
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
//Image img[1] = {"sunflower.png"};


class Texture {
	public:
		Image *backImage;
		GLuint backtexture;
		float xc[2];
		float yc[2];
};

class Global {
	public:
		int xres, yres;
		float fxres, fyres;
		int lbuttonDown;
		int selectedBox[2];
		int nPlants = 0;
		int nSunflowers = 0;
		int nPeashooters = 0;
		int nWalnuts = 0;
		int nRepeaters = 0;
		int nSun = 0;
		int nPeas = 0;
		int sun = 500;
		//Texture sunflower;
		//Texture peashooter;
		//Texture walnut;
		//Texture repeater;
		//Texture zombie;

		//2D array; each element is spot on map; int to determine plant type. [0][0] top left, [4][8] bottom right
		int mapView[5][9] = {{0}}; 
		Global() {
			xres = 1280, yres = 720;
			fxres=(float)xres, fyres=(float)yres;
			lbuttonDown = 0;
#ifdef DEBUG_ON
			srand(7);
#else
			srand((unsigned)time(NULL));
#endif
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

//prototypes
void init_opengl();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();
void introScreen();

bool introOn = true;

void introScreen() {
	glClear(GL_COLOR_BUFFER_BIT);
	Rect r;
	r.bot = g.yres - 50;
	r.left = g.xres / 2 - 50;
	r.center = 0;
	ggprint16(&r, 16, 0x00555555, "PvZ");

	Rect s;
	s.bot = g.yres - 100;
	s.left = g.xres / 2 - 125;
	s.center = 0;
	ggprint13(&s, 32, 0x00555555, "There's a zombie on your lawn!");
	ggprint13(&s, 16, 0x00555555, "Press s to start your defense!");
}

void transitionScreen() {
	glClearColor(0.08, 0.63, 0.12, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	Rect r;
	r.bot = g.yres / 2;
	r.left = g.xres / 2 - 100;
	r.center = 0;
	ggprint16(&r, 16, 0x00770011, "The zombies are coming. . .");
	x11.swapBuffers();
}

int main(int argc, char* argv[]) {
	init_opengl();
	//glutInit(&argc, argv);
	int done = 0;
	bool firstTime = true;
    //time_t timer;
	while(!done) {
		while(x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			x11.check_mouse(&e);
			done = x11.check_keys(&e);
		}
        //time_t currentTime = time(nullptr);
		if (introOn) {
			introScreen();
		}
		else { 
			if (firstTime) {
				transitionScreen();
				sleep(2.5);
				firstTime = false;
			}
           //if (currentTime - timer >= (1.0 / 20.0)) { 
			    physics();
			    render();
                //timer = currentTime;
           //}
		}
		x11.swapBuffers();
		usleep(512);
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
	XStoreName(dpy, win, "PvZ Clone");
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

const int numColumns = 9;
const int numRows = 5;

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
		if (e->xbutton.button== 3) {
			//Sets a box 
			int boxWidth = (g.xres) / numColumns;
			int boxHeight = (g.yres - 100) / numRows;
			int adjustX = e->xbutton.x;
			int adjustY = e->xbutton.y - 100;
			int row = adjustY / boxHeight;
			int col = adjustX / boxWidth;
			//Just in case column evals to over 9 set to max columns.
			if (col >= 9)
				col = 8;
			g.selectedBox[0] = row;
			g.selectedBox[1] = col;
			return;
		}

		if (e->xbutton.button== 1) {
			//Checks for sun collection
			for (int i = 0; i < g.nSun; ++i) { 
				if (e->xbutton.x >= suns[i].x - 20.0f &&
						e->xbutton.x <= suns[i].x + 20.0f &&
						g.yres - e->xbutton.y >= suns[i].y - 20.0f &&
						g.yres - e->xbutton.y <= suns[i].y + 20.0f) {
					g.sun += 25;
					suns[i].collected = true;
					for (int j = i; j < g.nSun - 1; ++j) {
						suns[j] = suns[j + 1];
					}
					--g.nSun;
					return;
				}
			}
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
				//Select sunflower
				if(!introOn) {
					if (g.sun >= 50 && g.mapView[g.selectedBox[0]][g.selectedBox[1]] == 0) {
						cout << "Sunflower Selected" << endl;
						p[g.nPlants].plantType = 1;
						p[g.nPlants].location.first = g.selectedBox[0];
						p[g.nPlants].location.second = g.selectedBox[1];
						cout << "Sunflower Plant number " << g.nPlants << " placed at " << p[g.nPlants].location.first << " " << p[g.nPlants].location.second << endl;
						g.sun -= 50;
						g.mapView[p[g.nPlants].location.first][p[g.nPlants].location.second] = 1;
						p[g.nPlants].n = g.nPlants;
						p[g.nPlants].timer = time(nullptr);
						++g.nPlants;
						++g.nSunflowers;
					}
					else
						cout << "Error placing plant: space occupied / insufficient sun" << endl;
				}
				break;

			case XK_2:
				if (!introOn) {
					if (g.sun >= 100 && g.mapView[g.selectedBox[0]][g.selectedBox[1]] == 0) {
						cout << "Peashooter Selected" << endl;
						p[g.nPlants].plantType = 2;
						p[g.nPlants].location.first = g.selectedBox[0];
						p[g.nPlants].location.second = g.selectedBox[1];
						cout << "Peashooter Plant number " << g.nPlants << " placed at " << p[g.nPlants].location.first << " " << p[g.nPlants].location.second << endl;
						g.sun -= 100;
						g.mapView[p[g.nPlants].location.first][p[g.nPlants].location.second] = 1;
						p[g.nPlants].n = g.nPlants;
						++g.nPlants;
						++g.nPeashooters;
					}
					else
						cout << "Error placing plant: space occupied / insufficient sun" << endl;
				}
				break;

			case XK_3:
				if (!introOn) {
					if (g.sun >= 50 && g.mapView[g.selectedBox[0]][g.selectedBox[1]] == 0) {
						cout << "Walnut Selected" << endl;
						p[g.nPlants].plantType = 3;
						p[g.nPlants].location.first = g.selectedBox[0];
						p[g.nPlants].location.second = g.selectedBox[1];
						cout << "Walnut Plant number " << g.nPlants << " placed at " << p[g.nPlants].location.first << " " << p[g.nPlants].location.second << endl;
						g.sun -= 100;
						g.mapView[p[g.nPlants].location.first][p[g.nPlants].location.second] = 1;
						p[g.nPlants].n = g.nPlants;
						++g.nPlants;
						++g.nWalnuts;
					}
					else
						cout << "Error placing plant: space occupied / insufficient sun" << endl;
				}
				break;

			case XK_4:
				if (!introOn) {
					if (g.sun >= 100 && g.mapView[g.selectedBox[0]][g.selectedBox[1]] == 0) {
						cout << "Repeater Selected" << endl;
						p[g.nPlants].plantType = 4;
						p[g.nPlants].location.first = g.selectedBox[0];
						p[g.nPlants].location.second = g.selectedBox[1];
						cout << "Repeater Plant number " << g.nPlants << " placed at " << p[g.nPlants].location.first << " " << p[g.nPlants].location.second << endl;
						g.sun -= 200;
						g.mapView[p[g.nPlants].location.first][p[g.nPlants].location.second] = 1;
						p[g.nPlants].n = g.nPlants;
						++g.nPlants;
						++g.nRepeaters;
					}
					else
						cout << "Error placing plant: space occupied / insufficient sun" << endl;
				}
				break;

				/*
				 * Different than other plants, appears briefly then disappears exploding enemies around it
				 *
				 case XK_5:
				 if (g.sun >= 100 && g.mapView[g.selectedBox[0]][g.selectedBox[1]] == 0) {
				 cout << "Doomshroom Selected" << endl;
				 p[g.nPlants].plantType = 2;
				 p[g.nPlants].location.first = g.selectedBox[0];
				 p[g.nPlants].location.second = g.selectedBox[1];
				 cout << "Doomshroom Plant number " << g.nPlants << " placed at " << p[g.nPlants].location.first << " " << p[g.nPlants].location.second << endl;
				 g.sun -= 100;
				 g.mapView[p[g.nPlants].location.first][p[g.nPlants].location.second] = 1;
				 p[g.nPlants].n = g.nPlants;
				 ++g.nPlants;
				 }
				 else
				 cout << "Error placing plant: space occupied / insufficient sun" << endl;
				 break;
				 */

			case XK_d:
				//Goes through plant array and deletes based on selected box, array elements shift down to fill gap
				if (g.mapView[g.selectedBox[0]][g.selectedBox[1]] != 0) {
					for (int i = 0; i < g.nPlants; ++i) {
						if (p[i].location.first == g.selectedBox[0] && p[i].location.second == g.selectedBox[1]) {
							if (p[i].plantType == 1)
								--g.nSunflowers;
							if (p[i].plantType == 2)
								--g.nPeashooters;
							if (p[i].plantType == 3)
								--g.nWalnuts;
							if (p[i].plantType == 4)
								--g.nRepeaters;
							for (int j = i; j < g.nPlants - 1; ++j) {
								p[j] = p[j + 1];
							}	       
							--g.nPlants;
							break;
						}
					}
				}
				else
					cout << "Error removing: Empty space" << endl;

				cout << "Plant " << g.nPlants << " removed at space " << g.selectedBox[0] << " " << g.selectedBox[1] << endl;
				break;
			case XK_s:
				introOn = false;
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
	glClearColor(0.08, 0.63, 0.12, 1.0);
	//
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
}

void physics() {
	//Make Zombies spawn and move
	//Plants w/ projectiles shoot
	int cellWidth = g.xres / numColumns;
	int cellHeight = (g.yres - 100) / numRows;

	time_t currentTime = time(nullptr);

    //Sunflower actions
	for (int i = 0; i < g.nPlants; ++i) {
		if (p[i].plantType == 1) {
			//sunflowers produce every 10s
			if ((currentTime - p[i].timer) >= 10.0) { 
				if (g.nSun < 20) {
					//Make Sun
					suns[g.nSun].x = p[i].location.second * cellWidth;
					suns[g.nSun].y = g.yres - ((p[i].location.first + 1) * cellHeight) - 100;
					cout << "x: " << suns[g.nSun].x << " y: " << suns[g.nSun].y << endl;
					suns[g.nSun].collected = false;
					++g.nSun;
					p[i].timer = currentTime;
				}
			}	
		}

        //Peashooter actions
		if (p[i].plantType == 2) {
			if (currentTime - p[i].timer >= 3.0) {
				//Make Projectiles
				cout << "Pea: " << g.nPeas << endl;	
				peas[g.nPeas].x = p[i].location.second * cellWidth + (0.6 * cellWidth);	
				peas[g.nPeas].y = g.yres - ((p[i].location.first + 1) * cellHeight) - (0.3 * cellHeight);
				++g.nPeas;
				p[i].timer = currentTime;
			}
		}

        //Repeater actions
        if (p[i].plantType == 4) {
            if (currentTime - p[i].timer >= 3.0) {
                cout << "Pea: " << g.nPeas << endl;
                peas[g.nPeas].x = p[i].location.second * cellWidth + (0.6 * cellWidth);
                peas[g.nPeas].y = g.yres - ((p[i].location.first + 1) * cellHeight) - (0.3 * cellHeight);
                ++g.nPeas;
                p[i].timer = currentTime;
            }
        }
	}

	//Pea Movement
	for (int i = 0; i < g.nPeas; ++i) {
		peas[i].x += 5.0;
		if (peas[i].x >= g.xres) {
			cout << "Pea: " << i << " went out of bounds" << endl;	
			peas[i] = peas[g.nPeas - 1];
			--g.nPeas;
			--i;
		}
	
	}

}


void render() {
	glClear(GL_COLOR_BUFFER_BIT);
	//
	int cellWidth = g.xres / numColumns;
	int cellHeight = (g.yres - 100) / numRows;

	//Render 5 x 9 grid
	glColor3f(0.59, 0.29, 0.0);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	for (int i = 0; i <= numColumns; ++i) {
		float x = (float)i * cellWidth;
		glVertex2f(x, g.yres - 100);
		glVertex2f(x, 0);
	}
	for (int i = 0; i <= numRows; ++i) {
		float y = (float)i * cellHeight;
		glVertex2f(0, y);
		glVertex2f(g.xres, y);
	}
	glEnd();

	//Render Plants
	//Sub-point: Render all projectiles 
	for(int i = 0; i < g.nPlants; ++i) {
		if (p[i].plantType == 1) {
			//Draws a box inside grid and scales it down
			glColor3f(0.92, 0.25, 0.2);
			float xPos = p[i].location.second * cellWidth;
			float yPos = g.yres - ((p[i].location.first + 1) * cellHeight) - 100;
			float centerX = xPos + cellWidth * 0.5f;
			float centerY = yPos + cellHeight * 0.5f;
			glPushMatrix();
			glTranslatef(centerX, centerY, 0.0f);
			glScalef(0.25f, 0.25f, 1.0f);
			glTranslatef(-centerX, -centerY, 0.0f);
			glBegin(GL_QUADS);
			glVertex2f(xPos, yPos);
			glVertex2f(xPos + cellWidth, yPos);
			glVertex2f(xPos + cellWidth, yPos + cellHeight);
			glVertex2f(xPos, yPos + cellHeight);
			glEnd();
			glPopMatrix();
		}
		if (p[i].plantType == 2) {
			//Draws a box inside grid and scales it down by 1/4
			//Placeholder texture, might use box in collision w/ Zombie
			glColor3f(0.1, 1.0, 0.2);
			float xPos = p[i].location.second * cellWidth;
			float yPos = g.yres - ((p[i].location.first + 1) * cellHeight) - 100;
			float centerX = xPos + cellWidth * 0.5f;
			float centerY = yPos + cellHeight * 0.5f;
			glPushMatrix();
			glTranslatef(centerX, centerY, 0.0f);
			glScalef(0.25f, 0.25f, 1.0f);
			glTranslatef(-centerX, -centerY, 0.0f);
			glBegin(GL_QUADS);
			glVertex2f(xPos, yPos);
			glVertex2f(xPos + cellWidth, yPos);
			glVertex2f(xPos + cellWidth, yPos + cellHeight);
			glVertex2f(xPos, yPos + cellHeight);
			glEnd();
			glPopMatrix();
		}
		if (p[i].plantType == 3) {
			//Draws a box inside grid and scales it down by 1/4
			//Placeholder texture, might use box in collision w/ Zombie
			glColor3f(0.86, 0.59, 0.0);
			float xPos = p[i].location.second * cellWidth;
			float yPos = g.yres - ((p[i].location.first + 1) * cellHeight) - 100;
			float centerX = xPos + cellWidth * 0.5f;
			float centerY = yPos + cellHeight * 0.5f;
			glPushMatrix();
			glTranslatef(centerX, centerY, 0.0f);
			glScalef(0.25f, 0.25f, 1.0f);
			glTranslatef(-centerX, -centerY, 0.0f);
			glBegin(GL_QUADS);
			glVertex2f(xPos, yPos);
			glVertex2f(xPos + cellWidth, yPos);
			glVertex2f(xPos + cellWidth, yPos + cellHeight);
			glVertex2f(xPos, yPos + cellHeight);
			glEnd();
			glPopMatrix();
		}
		if (p[i].plantType == 4) {
			//Draws a box inside grid and scales it down by 1/4
			//Placeholder texture, might use box in collision w/ Zombie
			glColor3f(0.01, 0.41, 0.0);
			float xPos = p[i].location.second * cellWidth;
			float yPos = g.yres - ((p[i].location.first + 1) * cellHeight) - 100;
			float centerX = xPos + cellWidth * 0.5f;
			float centerY = yPos + cellHeight * 0.5f;
			glPushMatrix();
			glTranslatef(centerX, centerY, 0.0f);
			glScalef(0.25f, 0.25f, 1.0f);
			glTranslatef(-centerX, -centerY, 0.0f);
			glBegin(GL_QUADS);
			glVertex2f(xPos, yPos);
			glVertex2f(xPos + cellWidth, yPos);
			glVertex2f(xPos + cellWidth, yPos + cellHeight);
			glVertex2f(xPos, yPos + cellHeight);
			glEnd();
			glPopMatrix();
		}


	}
	//Render projectiles: Sun & Peas
	for (int i = 0; i < g.nSun; ++i) {
		if (!suns[i].collected) {
			glColor3f(1.0, 1.0, 0.0);
			float sunRadius = 0.20 * cellWidth;
			float sunCenterX = suns[i].x;
			float sunCenterY = suns[i].y;
			int numSegments = 20;
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(sunCenterX, sunCenterY);
			for(int j = 0; j <= numSegments; j++) {
				float theta = 2.0f * 3.14159 * float(j) / float(20);
				float x = sunCenterX + sunRadius * cosf(theta);
				float y = sunCenterY + sunRadius * sinf(theta);
				glVertex2f(x, y);
			}
			glEnd();
		}
	}

	for (int i = 0; i < g.nPeas; ++i) {
		glColor3f(0.1, 1.0, 0.2);
		float peaRadius = 0.07 * cellWidth;
		float peaCenterX = peas[i].x;
		float peaCenterY = peas[i].y;
		int numSegments = 20;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(peaCenterX, peaCenterY);
		for(int j = 0; j <= numSegments; j++) {
			float theta = 2.0f * 3.14159 * float(j) / float(20);
			float x = peaCenterX + peaRadius * cosf(theta);
			float y = peaCenterY + peaRadius * sinf(theta);
			glVertex2f(x, y);
		}
		glEnd();
	}

	//Render Zombies

	glFlush();

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error: " << err << std::endl;
	}

	Rect r;
	r.bot = g.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00000000, "Right Click - Selected Box: (%i, %i)", g.selectedBox[0], g.selectedBox[1]);
	ggprint8b(&r, 16, 0x00000000, "Left Click - Sun: %i", g.sun);
	ggprint8b(&r, 16, 0x00000000, "1: Sunflower | 2: Peashooter | 3: Walnut | 4: Repeater | 5: DoomShroom | D: Dig up plant");

}



