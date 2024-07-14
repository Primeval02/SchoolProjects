//
//modified by: Bradley Tate
//date: March 2024
//
//4490 assignment:
//
//   1. Modify the A-star path algorithm to choose the order of neighbor
//      visiting randomly. This is similar to depth-first and breadth-first.
//      This is to see if the paths look more interesting, and so that
//      multiple iterations of path-finding give slightly different results.
//
//   2. Alter the use of the A-star heuristic value to change the final
//      path chosen. This can give the impression that the path-finding
//      has some intelligence behind it.
//         a. scale the value down
//         b. combine the value with some constant?
//         c. invent a heuristic strategy that looks good.
//
//   3. optional:
//      Modify the program timing and drawing to show the path finding
//      in-progress. Dijkstra's algorithm only reveals the path after the
//      algorithm has finished, but the search process is interesting to
//      see. Similar to the Wikipedia animation of Dijkstra's algorithm.
//
//
//
//4490 Path-finding algorithms
//Lab-9 A-star
//
//pathfind.cpp
//Gordon Griesel
//originally written for CMPS-312 Fall 2014
//modified: Spring 2020 for CMPS-4490
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "log.h"

#define MAX_IMAGES 9
//a global structure
struct Global {
	int xres, yres;
	int draw;
	int gridsize;
	int xgrid, ygrid;
	int bounds[2][2];
	//colors
	unsigned int cStart;
	unsigned int cPath;
	unsigned int cWall;
	unsigned int cGoal;
	//int grid[100][100];
	int start[2];
	int goal[2];
	int wall[200][200];
	int nwall;
	Global() {
		xres = 1024;
		yres = 900;
	}
} g;

typedef struct t_node {
	int xy[2];
	int parent[2];
	int visited;
	int chosen;
	//0 = not visited
	//1 = visited
	double cost;
	t_node() {
		visited = chosen = 0;
		cost = 9e9;
	}
} Node;

class X11_wrapper {
	Display *dpy;
	Window win;
	GC gc;
public:
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	X11_wrapper() {
		int scr;
		if (!(dpy=XOpenDisplay(NULL))) {
			fprintf(stderr, "ERROR: could not open display\n");
			exit(EXIT_FAILURE);
		}
		scr = DefaultScreen(dpy);
		win = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), 1, 1,
								g.xres, g.yres, 0, 0x00ffffff, 0x00000000);
		gc = XCreateGC(dpy, win, 0, NULL);
		XMapWindow(dpy, win);
		XSelectInput(dpy, win, ExposureMask | StructureNotifyMask |
								PointerMotionMask | ButtonPressMask |
								ButtonReleaseMask | KeyPressMask);
		setTitle();
	}
	void setTitle() {
		char ts[64];
		sprintf(ts,"CMPS-4490  Path Finding");
		XStoreName(dpy, win, ts);
	}
	void check_resize(XEvent *e) {
		//ConfigureNotify is sent when window size changes.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		g.xres = xce.width;
		g.yres = xce.height;
	}
	void resize_window(int w, int h) {
		g.xres = w;
		g.yres = h;
		unsigned int value_mask=0;
		value_mask |= CWWidth;
		value_mask |= CWHeight;
		XWindowChanges xwc;
		xwc.width = w;
		xwc.height = h;
		XConfigureWindow(dpy, win, value_mask, &xwc);
	}
	void clear_screen() {
		XClearWindow(dpy, win);
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
	void setForeground(unsigned int c) {
		XSetForeground(dpy, gc, c);
	}
	void drawRectangle(int x0, int y0, int x1, int y1) {
		XDrawRectangle(dpy, win, gc, x0, y0, x1, y1);
	}
	void fillRectangle(int x0, int y0, int x1, int y1) {
		XFillRectangle(dpy, win, gc, x0, y0, x1, y1);
	}
	void drawString(int x, int y, char *str) {
		XDrawString(dpy, win, gc, x, y, str, strlen(str));
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
} x11;

void init();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void render();
void showGrid();
void dfSearch();
void bfSearch();
void aStarSearch();

int main()
{
	logOpen();
	init();
	srand(time(NULL));
	int done=0;
	while (!done) {
		//Check the event queue
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
			render();
		}
	}
	logClose();
	return 0;
}

void init()
{
	//initialize variables
	g.draw=0;
	g.gridsize=20;
	g.cStart = 0x0000ff00;
	g.cWall  = 0x00ffffff;
	g.cGoal  = 0x00ff0000;
	g.cPath  = 0x00ffff00;
	//
	g.start[0] = 3;
	g.start[1] = (g.yres/g.gridsize) - 6;
	//
	//this location is dependent on window size.
	//you may adjust it.
	g.goal[0] = (g.xres/g.gridsize) - 2 - 6;
	g.goal[1] = 6;
	//printf("goal: %i %i\n", g.goal[0], g.goal[1]);
	//
	//grid bounds
	g.bounds[0][0] = 0; //x min
	g.bounds[0][1] = (g.xres/g.gridsize) - 2; //x max
	g.bounds[1][0] = 0; //y min
	g.bounds[1][1] = (g.yres/g.gridsize) - 4; //y max
	//
	//build the wall
	g.nwall = 0;
	for (int i=10; i<14; i++) {
		for (int j=14; j<34; j++) {
			g.wall[g.nwall][0] = j;
			g.wall[g.nwall][1] = i;
			g.nwall++;
		}
	}
	//
	//optional...
	//put a crack in the wall...
	//
	#define CRACK_IN_WALL
	#ifdef CRACK_IN_WALL
	for (int i=14; i<24; i++) {
	#else
	for (int i=14; i<25; i++) {
	#endif
		for (int j=30; j<34; j++) {
			g.wall[g.nwall][0] = j;
			g.wall[g.nwall][1] = i;
			g.nwall++;
		}
	}
	for (int i=25; i<30; i++) {
		for (int j=30; j<34; j++) {
			g.wall[g.nwall][0] = j;
			g.wall[g.nwall][1] = i;
			g.nwall++;
		}
	}
}

void fillNode(int x, int y, unsigned int color)
{
	x11.setForeground(color);
	x11.fillRectangle(x * g.gridsize + g.gridsize/2 + 1,
		(y+2) * g.gridsize + g.gridsize/2 + 1,
		g.gridsize-5, g.gridsize-5);
}

void check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	//
	int mx = e->xbutton.x;
	int my = e->xbutton.y;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		//Log("ButtonPress %i %i\n", e->xbutton.x, e->xbutton.y);
		if (e->xbutton.button==1) {
            g.goal[0] = mx / g.gridsize;
            g.goal[1] = my / g.gridsize - 2;
            fillNode(g.goal[0], g.goal[1], g.cGoal);
            showGrid();
            aStarSearch();
			//Left button pressed
		}
		if (e->xbutton.button==3) {
			//Right button pressed
		}
	}
	if (savex != mx || savey != my) {
		//mouse moved
		savex = mx;
		savey = my;
	}
}

int check_keys(XEvent *e)
{
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type != KeyPress)
		return 0;
	switch (key) {
		case XK_1:
			showGrid();
			break;
		case XK_a:
			showGrid();
			aStarSearch();
			break;
		case XK_d:
			showGrid();
			dfSearch();
			break;
		case XK_b:
			showGrid();
			bfSearch();
			break;
		case XK_equal:
			x11.setTitle();
			break;
		case XK_minus:
			x11.setTitle();
			break;
		case XK_Delete:
			break;
		case XK_Escape:
			//quitting the program
			return 1;
	}
	//clear_screen();
	//g.draw=1;
	return 0;
}


void show_menu()
{
	int x=10, y=16;
	char ts[200];
	x11.set_color_3i(255,255,255);
	sprintf(ts,"4490 Path Finding  (1) show-grid  (D)epth-first  (B)readth-first  (A)star  (M1)Click to change A* Goal");
	x11.drawString(x, y, ts);
	y += 16;
	//
	x = 10;
	x11.setForeground(g.cStart);
	x11.fillRectangle(x,24,12,12);
	x11.set_color_3i(255,255,255);
	sprintf(ts,"start");
	x11.drawString(x+14, 36, ts);
	//
	x += 100;
	x11.setForeground(g.cWall);
	x11.fillRectangle(x,24,12,12);
	x11.set_color_3i(255,255,255);
	sprintf(ts,"wall");
	x11.drawString(x+14, 36, ts);
	//
	x += 100;
	x11.setForeground(g.cGoal);
	x11.fillRectangle(x,24,12,12);
	x11.set_color_3i(255,255,255);
	sprintf(ts,"goal");
	x11.drawString(x+14, 36, ts);
	//
	x += 100;
	x11.setForeground(g.cPath);
	x11.fillRectangle(x,24,12,12);
	x11.set_color_3i(255,255,255);
	sprintf(ts,"path");
	x11.drawString(x+14, 36, ts);
}
/*
void fillNode(int x, int y, unsigned int color)
{
	x11.setForeground(color);
	x11.fillRectangle(x * g.gridsize + g.gridsize/2 + 1,
		(y+2) * g.gridsize + g.gridsize/2 + 1,
		g.gridsize-5, g.gridsize-5);
}*/

void fillPath(int x, int y, unsigned int color)
{
	x11.setForeground(color);
	x11.fillRectangle(x * g.gridsize + g.gridsize/2 + 4,
		(y+2) * g.gridsize + g.gridsize/2 + 4,
		g.gridsize-11, g.gridsize-11);
}

void showGrid()
{
	int i,j;
	
	x11.clear_screen();
	g.xgrid = g.ygrid = 0;
	x11.set_color_3i(255,255,255);
	for (i=0; i<(g.yres-g.gridsize*2)/g.gridsize-1; i++) {
		g.ygrid++;
		g.xgrid = 0;
		for (j=0; j<g.xres/g.gridsize-1; j++) {
			x11.drawRectangle(j*g.gridsize+g.gridsize/2,
				(i+2)*g.gridsize+g.gridsize/2,
				g.gridsize-4, g.gridsize-4);
			g.xgrid++;
		}
	}
	//printf("xgrid: %i ygrid: %i\n",g.xgrid,g.ygrid);
	g.bounds[0][0] = 0; //x min
	g.bounds[0][1] = (g.xgrid) - 1; //x max
	g.bounds[1][0] = 0; //y min
	g.bounds[1][1] = (g.ygrid) - 1; //y max
	//show starting grid
	fillNode(g.start[0], g.start[1], g.cStart);
	//show goal grid
	fillNode(g.goal[0], g.goal[1], g.cGoal);
	//show wall
	for (i=0; i<g.nwall; i++) {
		fillNode(g.wall[i][0], g.wall[i][1], g.cWall);
	}
}

int checkBounds(const int x, const int y)
{
	//1 = point is ok
	//0 = point out of range, or hit wall
	//
	//check edges of grid
	if (x < g.bounds[0][0]) return 0;
	if (x > g.bounds[0][1]) return 0;
	if (y < g.bounds[1][0]) return 0;
	if (y > g.bounds[1][1]) return 0;
	//check walls
	for (int i=0; i<g.nwall; i++) {
		if (g.wall[i][0] == x && g.wall[i][1] == y) return 0;
	}
	return 1;
}

void dfSearch()
{
	//setup a grid array to keep track of used nodes
	//size of 200 is arbitrary.
	Node **grid = new Node*[200];
	for (int i=0; i<200; i++)
		grid[i] = new Node[200];
	//start with no nodes visited
	for (int i=0; i<200; i++) {
		for (int j=0; j<200; j++) {
			grid[i][j].visited = 0;
		}
	}
	//
	//setup a stack array to hold nodes
	//stack might have to hold all nodes on screen
	#define MAXSTACK 10000
	Node stack[MAXSTACK];
	int nstack = 0;
	//
	//
	//push start node onto stack
	stack[nstack].xy[0] = g.start[0];
	stack[nstack].xy[1] = g.start[1];
	nstack++;
	//starting node has been visited
	int x = g.start[0];
	int y = g.start[1];
	grid[x][y].visited=1;
	//starting node has no parent (important)
	grid[x][y].parent[0] = -1;
	grid[x][y].parent[1] = -1;
	//
	//
	//setup a current node
	Node currNode;
	//
	//
	//flag indicating goal reached
	int goal=0;
	//
	//find a path to the goal node...
	//
	while (!goal) {
		//
		//("nstack: %i\n",nstack);
		//pop the stack into current node
		currNode.xy[0] = stack[nstack-1].xy[0];
		currNode.xy[1] = stack[nstack-1].xy[1];
		//Log("currNode: %i %i\n",currNode.xy[0],currNode.xy[1]);
		nstack--;
		//
		//have we reached the goal?
		//
		if (currNode.xy[0] == g.goal[0] && currNode.xy[1] == g.goal[1]) {
			//Yes!
			goal=1;
			//printf("Goal found.\n");
			continue;
		}
		//no.
		//push 4 neighboring nodes onto stack
		int xx;
		int r[4]={0,0,0,0};
		r[0] = r[1] = r[2] = r[3]= 0;
		while(!r[0] || !r[1] || !r[2] || !r[3]) {
			//Visit the neighbor in a random each move.
			xx = rand() % 4;
			//printf("%i",xx);
			if (r[xx]) continue;
			r[xx]=1;
			switch (xx) {
				case 0:
					x = currNode.xy[0]-1;
					y = currNode.xy[1];
					if (checkBounds(x,y) && !grid[x][y].visited) {
						stack[nstack].xy[0] = x;
						stack[nstack].xy[1] = y;
						nstack++;
						grid[x][y].visited=1;
						grid[x][y].parent[0]=currNode.xy[0];
						grid[x][y].parent[1]=currNode.xy[1];
					}
					break;
				case 1:
					x = currNode.xy[0]+1;
					y = currNode.xy[1];
					if (checkBounds(x,y) && !grid[x][y].visited) {
						stack[nstack].xy[0] = x;
						stack[nstack].xy[1] = y;
						nstack++;
						grid[x][y].visited=1;
						grid[x][y].parent[0]=currNode.xy[0];
						grid[x][y].parent[1]=currNode.xy[1];
					}
					break;
				case 2:
					x = currNode.xy[0];
					y = currNode.xy[1]-1;
					if (checkBounds(x,y) && !grid[x][y].visited) {
						stack[nstack].xy[0] = x;
						stack[nstack].xy[1] = y;
						nstack++;
						grid[x][y].visited=1;
						grid[x][y].parent[0]=currNode.xy[0];
						grid[x][y].parent[1]=currNode.xy[1];
					}
					break;
				case 3:
					x = currNode.xy[0];
					y = currNode.xy[1]+1;
					if (checkBounds(x,y) && !grid[x][y].visited) {
						stack[nstack].xy[0] = x;
						stack[nstack].xy[1] = y;
						nstack++;
						grid[x][y].visited=1;
						grid[x][y].parent[0]=currNode.xy[0];
						grid[x][y].parent[1]=currNode.xy[1];
					}
					break;
			}
		}
		if (nstack >= MAXSTACK)
			printf("ERROR: stack size exceeded.\n");
	}
	//
	//We found the goal
	//Now, step backwards from the goal to the start
	//Use the node parent to do this
	//
	//
	//assert current node is at the goal
	if (currNode.xy[0] != g.goal[0] || currNode.xy[1] != g.goal[1]) {
		printf("ERROR: not at goal node!\n");
		exit(0);
	}
	//
	for (int i=0; i<g.ygrid; i++) {
		for (int j=0; j<g.xgrid; j++) {
			if (grid[j][i].visited)
				fillNode(j, i, 0x00555555);
		}
	}
	//
	//find a path to the goal node...
	while (grid[currNode.xy[0]][currNode.xy[1]].parent[0] >= 0) {
		//simplify to x, y
		x = currNode.xy[0];
		y = currNode.xy[1];
		//get the node's parent
		currNode.xy[0] = grid[x][y].parent[0];
		currNode.xy[1] = grid[x][y].parent[1];
		//draw this node with path color
		x = currNode.xy[0];
		y = currNode.xy[1];
		if (grid[x][y].parent[0] >= 0) {
			fillNode(currNode.xy[0], currNode.xy[1], g.cPath);
		}
	}
	//show start and goal.
	fillNode(g.start[0], g.start[1], g.cStart);
	fillNode(g.goal[0], g.goal[1], g.cGoal);
	//
	//cleanup
	for (int i=0; i<200; i++)
		delete grid[i];
	delete [] grid;
}

void bfSearch()
{
	//copy the code from dfSearch() to here.
	//We will discuss how to do this in class.
	//
	//setup a grid array to keep track of used nodes
	//size of 200 is arbitrary.
	Node **grid = new Node*[200];
	for (int i=0; i<200; i++)
		grid[i] = new Node[200];
	//start with no nodes visited
	for (int i=0; i<200; i++) {
		for (int j=0; j<200; j++) {
			grid[i][j].visited = 0;
		}
	}
	//
	//
	//setup a queue array to hold nodes
	//queue might have to hold all nodes on screen
	#define MAXQUEUE 10000
	Node queue[MAXQUEUE];
	int nqueue = 0;
	int qfront = 0;
	int qback = 0;
	//
	//
	//push start node onto queue
	queue[qback].xy[0] = g.start[0];
	queue[qback].xy[1] = g.start[1];
	qback++;
	nqueue++;
	//starting node has been visited
	int x = g.start[0];
	int y = g.start[1];
	grid[x][y].visited=1;
	//starting node has no parent (important)
	grid[x][y].parent[0] = -1;
	grid[x][y].parent[1] = -1;
	//
	//
	//setup a current node
	Node currNode;
	//
	//
	//flag indicating goal reached
	int goal=0;
	//
	//find a path to the goal node...
	//
	while (!goal) {
		//
		//Log("nqueue: %i\n",nqueue);
		//dequeue into current node
		currNode.xy[0] = queue[qfront].xy[0];
		currNode.xy[1] = queue[qfront].xy[1];
		qfront++;
		nqueue++;
		//Log("currNode: %i %i\n",currNode.xy[0],currNode.xy[1]);
		//
		//have we reached the goal?
		//
		if (currNode.xy[0] == g.goal[0] && currNode.xy[1] == g.goal[1]) {
			//Yes!
			goal=1;
			//printf("Goal found.\n");
			continue;
		}
		//no.
		//enqueue the 4 surrounding nodes onto queue
		int xx;
		int r[4]={0,0,0,0};
		r[0] = r[1] = r[2] = r[3]= 0;
		while(!r[0] || !r[1] || !r[2] || !r[3]) {
			xx = rand() % 4;
			//printf("%i",xx);
			if (r[xx]) continue;
			r[xx]=1;
			switch(xx) {
				case 0:
					x = currNode.xy[0]-1;
					y = currNode.xy[1];
					if (checkBounds(x,y) && !grid[x][y].visited) {
						queue[qback].xy[0] = x;
						queue[qback].xy[1] = y;
						qback++;
						nqueue++;
						grid[x][y].visited=1;
						grid[x][y].parent[0]=currNode.xy[0];
						grid[x][y].parent[1]=currNode.xy[1];
					}
					break;
				case 1:
					x = currNode.xy[0]+1;
					y = currNode.xy[1];
					if (checkBounds(x,y) && !grid[x][y].visited) {
						//stack[nstack].xy[0] = x;
						//stack[nstack].xy[1] = y;
						//nstack++;
						queue[qback].xy[0] = x;
						queue[qback].xy[1] = y;
						qback++;
						nqueue++;
						grid[x][y].visited=1;
						grid[x][y].parent[0]=currNode.xy[0];
						grid[x][y].parent[1]=currNode.xy[1];
					}
					break;
				case 2:
					x = currNode.xy[0];
					y = currNode.xy[1]-1;
					if (checkBounds(x,y) && !grid[x][y].visited) {
						//stack[nstack].xy[0] = x;
						//stack[nstack].xy[1] = y;
						//nstack++;
						queue[qback].xy[0] = x;
						queue[qback].xy[1] = y;
						qback++;
						nqueue++;
						grid[x][y].visited=1;
						grid[x][y].parent[0]=currNode.xy[0];
						grid[x][y].parent[1]=currNode.xy[1];
					}
					break;
				case 3:
					x = currNode.xy[0];
					y = currNode.xy[1]+1;
					if (checkBounds(x,y) && !grid[x][y].visited) {
						//stack[nstack].xy[0] = x;
						//stack[nstack].xy[1] = y;
						//nstack++;
						queue[qback].xy[0] = x;
						queue[qback].xy[1] = y;
						qback++;
						nqueue++;
						grid[x][y].visited=1;
						grid[x][y].parent[0]=currNode.xy[0];
						grid[x][y].parent[1]=currNode.xy[1];
					}
					break;
			}
		}
		if (nqueue >= MAXQUEUE)
			printf("ERROR: queue size exceeded.\n");
	}
	//
	//We found the goal
	//Now, step backwards from the goal to the start
	//Use the node parent to do this
	//
	//
	//assert current node is at the goal
	if (currNode.xy[0] != g.goal[0] || currNode.xy[1] != g.goal[1]) {
		printf("ERROR: not at goal node!\n");
		exit(0);
	}
	//fillNode(currNode.xy[0], currNode.xy[1], 0x00555555);
	//return;
	//
	//for (int i=0; i<(g.yres-g.gridsize*2)/g.gridsize-1; i++) {
	//	for (int j=0; j<g.xres/g.gridsize-1; j++) {
	for (int i=0; i<g.ygrid; i++) {
		for (int j=0; j<g.xgrid; j++) {
			if (grid[j][i].visited)
				fillNode(j, i, 0x00555555);
		}
	}
	//find a path to the goal node...
	//
	while (grid[currNode.xy[0]][currNode.xy[1]].parent[0] >= 0) {
		//simplify to x, y
		x = currNode.xy[0];
		y = currNode.xy[1];
		//get the node's parent
		currNode.xy[0] = grid[x][y].parent[0];
		currNode.xy[1] = grid[x][y].parent[1];
		//draw this node with path color
		x = currNode.xy[0];
		y = currNode.xy[1];
		if (grid[x][y].parent[0] >= 0) {
			fillNode(currNode.xy[0], currNode.xy[1], g.cPath);
		}
	}
	//show start and goal.
	fillNode(g.start[0], g.start[1], g.cStart);
	fillNode(g.goal[0], g.goal[1], g.cGoal);
	//
	//cleanup
	for (int i=0; i<200; i++)
		delete grid[i];
	delete [] grid;
}

void aStarSearch()
{
	//size of 200 is arbitrary.
	Node **grid = new Node*[200];
	for (int i=0; i<200; i++)
		grid[i] = new Node[200];
	//start with no nodes visited
	for (int i=0; i<200; i++) {
		for (int j=0; j<200; j++) {
			grid[i][j].visited = 0;
		}
	}
	//starting node has been visited
	int x = g.start[0];
	int y = g.start[1];
	//printf("start: %i %i\n", x,y);
	//exit(0);
	grid[x][y].cost = 0.0;
	grid[x][y].chosen = 0;
	//starting node has no parent (important)
	grid[x][y].parent[0] = -1;
	grid[x][y].parent[1] = -1;
	//setup a current node
	Node currNode;
	//flag indicating goal reached
	int goal=0;
	while (!goal) {
		double low_cost = 9e9;
		int lowx=0, lowy=0;
		for (int i=0; i<200; i++) {
			for (int j=0; j<200; j++) {
				if (!grid[i][j].chosen) {
					//printf("cost: %lf\n",grid[i][j].cost);
					//add heuristic
					double x = (double)i - g.goal[0];
					double y = (double)j - g.goal[1];
					double dist = x*x + y*y;
                    dist *= 0.1;
					double newcost = grid[i][j].cost + dist;
					if (newcost < low_cost) {
						low_cost = grid[i][j].cost + dist;
						lowx = i;
						lowy = j;
					}
				}
			}
		}
		currNode.xy[0] = lowx;
		currNode.xy[1] = lowy;
		currNode.cost = grid[lowx][lowy].cost;
		currNode.chosen = 1;
		grid[lowx][lowy].cost = low_cost;
		grid[lowx][lowy].chosen = 1;
		//have we reached the goal?
		if (currNode.xy[0] == g.goal[0] && currNode.xy[1] == g.goal[1]) {
			//Yes!
			goal=1;
			//printf("Goal reached.\n");
			continue;
		}
		//not at goal...
        //Precedence:
        //check diagonals first going right to left
 		x = currNode.xy[0]+1;
		y = currNode.xy[1]+1;
		if (checkBounds(x,y) && !grid[x][y].chosen) {
			grid[x][y].parent[0]=currNode.xy[0];
			grid[x][y].parent[1]=currNode.xy[1];
			grid[x][y].cost = currNode.cost + 1.1;
			grid[x][y].visited = 1;
		}
            
		x = currNode.xy[0]+1;
		y = currNode.xy[1]-1;
		if (checkBounds(x,y) && !grid[x][y].chosen) {
			grid[x][y].parent[0]=currNode.xy[0];
			grid[x][y].parent[1]=currNode.xy[1];
			grid[x][y].cost = currNode.cost + 1.1;
			grid[x][y].visited = 1;
		}

		x = currNode.xy[0]-1;
		y = currNode.xy[1]+1;
		if (checkBounds(x,y) && !grid[x][y].chosen) {
			grid[x][y].parent[0]=currNode.xy[0];
			grid[x][y].parent[1]=currNode.xy[1];
			grid[x][y].cost = currNode.cost + 1.0;
			grid[x][y].visited = 1;
		}

		x = currNode.xy[0]-1;
		y = currNode.xy[1]-1;
		if (checkBounds(x,y) && !grid[x][y].chosen) {
			grid[x][y].parent[0]=currNode.xy[0];
			grid[x][y].parent[1]=currNode.xy[1];
			grid[x][y].cost = currNode.cost + 1.0;
			grid[x][y].visited = 1;
		}
        //then horizontal & vertical follow
		x = currNode.xy[0]-1;
		y = currNode.xy[1];
		if (checkBounds(x,y) && !grid[x][y].chosen) {
			grid[x][y].parent[0]=currNode.xy[0];
			grid[x][y].parent[1]=currNode.xy[1];
			grid[x][y].cost = currNode.cost + 1.0;
			grid[x][y].visited = 1;
		}
		x = currNode.xy[0]+1;
		y = currNode.xy[1];
		if (checkBounds(x,y) && !grid[x][y].chosen) {
			grid[x][y].parent[0]=currNode.xy[0];
			grid[x][y].parent[1]=currNode.xy[1];
			grid[x][y].cost = currNode.cost + 1.0;
			grid[x][y].visited = 1;
		}
		x = currNode.xy[0];
		y = currNode.xy[1]-1;
		if (checkBounds(x,y) && !grid[x][y].chosen) {
			grid[x][y].parent[0]=currNode.xy[0];
			grid[x][y].parent[1]=currNode.xy[1];
			grid[x][y].cost = currNode.cost + 1.0;
			grid[x][y].visited = 1;
		}
		x = currNode.xy[0];
		y = currNode.xy[1]+1;
		if (checkBounds(x,y) && !grid[x][y].chosen) {
			grid[x][y].parent[0]=currNode.xy[0];
			grid[x][y].parent[1]=currNode.xy[1];
			grid[x][y].cost = currNode.cost + 1.0;
			grid[x][y].visited = 1;
		}
	}
	//We found the goal
	//Now, step backwards from the goal to the start
	//Use the node parent to do this
	//assert current node is at the goal
	if (currNode.xy[0] != g.goal[0] || currNode.xy[1] != g.goal[1]) {
		printf("ERROR: not at goal node!\n");
		exit(0);
	}
	//
	for (int i=0; i<g.ygrid; i++) {
		for (int j=0; j<g.xgrid; j++) {
			if (grid[j][i].chosen)
				fillNode(j, i, 0x00555555);
		}
	}
	//
	//find a path to the goal node...
	while (grid[currNode.xy[0]][currNode.xy[1]].parent[0] >= 0) {
		//simplify to x, y
		x = currNode.xy[0];
		y = currNode.xy[1];
		//get the node's parent
		currNode.xy[0] = grid[x][y].parent[0];
		currNode.xy[1] = grid[x][y].parent[1];
		//draw this node with path color
		x = currNode.xy[0];
		y = currNode.xy[1];
		if (grid[x][y].parent[0] >= 0) {
			fillNode(currNode.xy[0], currNode.xy[1], g.cPath);
		}
	}
	//show start and goal.
	fillNode(g.start[0], g.start[1], g.cStart);
	fillNode(g.goal[0], g.goal[1], g.cGoal);
	//
	//cleanup
	for (int i=0; i<200; i++)
		delete grid[i];
	delete [] grid;
}

void render()
{
	//showGrid();
	show_menu();
}

