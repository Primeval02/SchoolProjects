//
//author:  Gordon Griesel
//date:    2018
//program: flag.cpp
//
//mass and springs flag
//
//When program starts, gravity will increase from zero upward.
//This gives the flag a better starting shape.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "fonts.h"
#include "log.h"

typedef float Flt;
typedef Flt Vec[3];
#define rnd() (float)rand() / (float)RAND_MAX
#define PI 3.14159265358979323846264338327950

#define VecCross(a,b,c) \
(c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1]; \
(c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2]; \
(c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]

void vecCrossProduct(Vec v0, Vec v1, Vec dest);
Flt vecDotProduct(Vec v0, Vec v1);
void vecZero(Vec v);
void vecMake(Flt a, Flt b, Flt c, Vec v);
void vecCopy(Vec source, Vec dest);
Flt vecLength(Vec v);
Flt getDistance(Vec v0, Vec v1);
void vecNormalize(Vec v);
void vecSub(Vec v0, Vec v1, Vec dest);
void vecAdd(Vec v0, Vec v1, Vec dest);
void vecScale(Flt sc, Vec v, Vec dest);
void getTriangleNormal(Vec t0, Vec t1, Vec t2, Vec norm);

class Logfile {
public:
	Logfile() {
		logOpen();
		Log("log is open.\n");
	}
} logfile;

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			sprintf(ppmname,"%s.ppm", name);
			char ts[100];
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
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
		if (!ppmFlag)
			unlink(ppmname);
	}
};
Image img[2] = { "./flag.jpg", "floor.jpg" };

class Global {
public:
    Flt flagStiff;
	int xres, yres;
	GLuint feltTextureId;
	GLfloat lightAmbient[4];
	GLfloat lightDiffuse[4];
	GLfloat lightSpecular[4];
	GLfloat lightPosition[4];
	int gravity_auto_start;
	Flt gravity;

	Flt wind_strength;
	Vec wind_dir;
	Global() {
		srand(time(NULL));
		xres = 640;
		yres = 480;
		GLfloat la[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat ld[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat ls[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat lp[] = { 100.0f, 60.0f, 240.0f, 1.0f };
		//lp[0] = rnd() * 200.0 - 100.0;
		//lp[1] = rnd() * 100.0 + 20.0;
		//lp[2] = rnd() * 300.0 - 150.0;
		memcpy(lightAmbient, la, sizeof(GLfloat)*4);
		memcpy(lightDiffuse, ld, sizeof(GLfloat)*4);
		memcpy(lightSpecular, ls, sizeof(GLfloat)*4);
		memcpy(lightPosition, lp, sizeof(GLfloat)*4);
		gravity_auto_start = 1;
		//gravity = -0.002;
		gravity = 0.0;
        flagStiff = 0.1;
		wind_strength = 1.2;
		vecMake(.07, .01, .01, wind_dir);
	}
} g;

struct Mass {
	Flt mass, oomass;
	Vec pos, vel, force;
	Flt texcoord[2];
	int color[3];
	int attached;
	Vec norm;
	int nnorm;
	Mass() {
		attached = 0;
		nnorm = 0;
		vecMake(0,0,1,norm);
	}
};

struct Spring {
	int mass[2];
	Flt length;
	Flt stiffness;
};

const int MAX_MASSES = 4000;
const int MAX_SPRINGS = 20000;

class Flag {
public:
	Vec pos;
	int w, h;
	int nw, nh;
	Mass *mass;
	Spring *spring;
	int nmasses;
	int nsprings;
	GLuint textureId;
	int wire;
	Flt stiffness;
	~Flag() {
		delete [] mass;
		delete [] spring;
	}
	//=====================================================================
	Flag() {
		printf("constructor\n");
		wire = 0;
		vecMake(-5,5,0,pos);
		w = 7;
		h = 5;
		nw = 22;
		nh = 20;
		mass = new Mass[MAX_MASSES];
		spring = new Spring[MAX_SPRINGS];
		nmasses = 0;
		nsprings = 0;
		stiffness = 0.15;
		build_flag();
	}
	//=====================================================================
	void build_flag(void) {
		//position is upper left corner of flag.
		Flt x = 0.0, y = 0.0;
		Flt xstep = (Flt)w / (Flt)nw;
		Flt ystep = (Flt)h / (Flt)nh;
		Log("xy step: %f %f\n", xstep, ystep);
		nmasses = 0;
		for (int i=0; i<nh; i++) {
			for (int j=0; j<nw; j++) {
				if (j == 0)
					mass[nmasses].attached = 1;
				mass[nmasses].mass = 1.0;
				mass[nmasses].oomass = 1.0;
				mass[nmasses].pos[0] = x + pos[0];
				mass[nmasses].pos[1] = y + pos[1];
				mass[nmasses].pos[2] = pos[2];
				//Log("%f %f %f\n",
				//mass[nmasses].pos[0], mass[nmasses].pos[1],
				//mass[nmasses].pos[2]);
				mass[nmasses].vel[0] =
				mass[nmasses].vel[1] =
				mass[nmasses].vel[2] =
				mass[nmasses].force[0] =
				mass[nmasses].force[1] =
				mass[nmasses].force[2] = 0.0;
				mass[nmasses].color[0] = 200 + rand() % 40;
				mass[nmasses].color[1] = 40 + rand() % 20;
				mass[nmasses].color[2] = 10 + rand() % 10;
				mass[nmasses].texcoord[0] = (Flt)(j) / (Flt)nw;
				mass[nmasses].texcoord[1] = (Flt)(nh-i) / (Flt)nh;
				vecMake(0,0,1,mass[nmasses].norm);
				nmasses++;
				x += xstep;
			}
			x = 0.0;
			y += ystep;
		}

/*
		//show normals and quit
		Vec norm;
		for (int i=0; i<nh-1; i++) {
			for (int j=0; j<nw-1; j++) {
				int k = i * nw + j;
				int v0 = k;
				int v1 = v0+1;
				int v2 = v0+nw;
				int v3 = v2+1;
				Log("\nget normal for vertices...\n");
				Log("pos: %f %f %f\n",
				mass[v0].pos[0],mass[v0].pos[1],mass[v0].pos[2]);
				Log("     %f %f %f\n",
				mass[v2].pos[0],mass[v2].pos[1],mass[v2].pos[2]);
				Log("     %f %f %f\n",
				mass[v3].pos[0],mass[v3].pos[1],mass[v3].pos[2]);
				getTriangleNormal(
					mass[v0].pos, mass[v2].pos, mass[v3].pos, norm);
				Log("%i %i:  %f %f %f\n", i, j, norm[0], norm[1], norm[2]);
			}
		}
		exit(0);
*/
		setup_springs();
		printf("nmasses: %i nsprings: %i\n", nmasses, nsprings);
	}
	//=====================================================================
	void setup_springs(void) {
		nsprings = 0;
		for (int i=0; i<nh; i++) {
			for (int j=0; j<nw; j++) {
				//Structural springs, right and down.
				//which way can we move?
				int right = (j < nw-1);
				int down = (i < nh-1);
				int left = (j > 0);
				//int up = (i > 0);
				int right2 = (j < nw-2);
				int down2 = (i < nh-2);
				//int right3 = (j < nw-3);
				//int down3 = (i < nh-3);
				int right4 = (j < nw-4);
				int down4 = (i < nh-4);
				if (right) {
					//can move right
					int k = i * nw + j;
					int m = k + 1;
					spring[nsprings].mass[0] = k;
					spring[nsprings].mass[1] = m;
					spring[nsprings].length =
						getDistance(mass[k].pos, mass[m].pos);
					//spring[nsprings].stiffness = g.flagStiff;
					spring[nsprings].stiffness = rand() * 0.1 + 0.2;
					nsprings++;
				}
				if (down) {
					//down
					int k = i * nw + j;
					int m = k + nw;
					spring[nsprings].mass[0] = k;
					spring[nsprings].mass[1] = m;
					spring[nsprings].length =
						getDistance(mass[k].pos, mass[m].pos);
					//spring[nsprings].stiffness = g.flagStiff;
					spring[nsprings].stiffness = rand() * 0.1 + 0.2;
					nsprings++;
				}
				//Sheer springs...
				if (right && down) {
					int k = i * nw + j;
					int m = k+nw+1;
					spring[nsprings].mass[0] = k;
					spring[nsprings].mass[1] = m;
					spring[nsprings].length =
						getDistance(mass[k].pos, mass[m].pos);
					//spring[nsprings].stiffness = g.flagStiff;
					spring[nsprings].stiffness = rand() * 0.1 + 0.2;
					nsprings++;
				}
				if (left && down) {
					int k = i * nw + j;
					int m = k+nw-1;
					spring[nsprings].mass[0] = k;
					spring[nsprings].mass[1] = m;
					spring[nsprings].length =
						getDistance(mass[k].pos, mass[m].pos);
					//spring[nsprings].stiffness = g.flagStiff;
					spring[nsprings].stiffness = rand() * 0.1 + 0.2;
					nsprings++;
				}
				//Bend springs...
				//Join every x second mass.
				#define BEND_SPRINGS
				#ifdef BEND_SPRINGS
				if (right2) {
					int k = i*nw+j;
					int m = k+2;
					spring[nsprings].mass[0] = k;
					spring[nsprings].mass[1] = m;
					spring[nsprings].length =
						getDistance(mass[k].pos, mass[m].pos);
					//spring[nsprings].stiffness = g.flagStiff;
					spring[nsprings].stiffness = rand() * 0.1 + 0.2;
					nsprings++;
				}
				if (down2) {
					int k = i*nw+j;
					int m = k+nw*2;
					spring[nsprings].mass[0] = k;
					spring[nsprings].mass[1] = m;
					spring[nsprings].length =
						getDistance(mass[k].pos, mass[m].pos);
					//spring[nsprings].stiffness = g.flagStiff;
					spring[nsprings].stiffness = rand() * 0.1 + 0.2;
					nsprings++;
				}
				//Join every x second mass.
				if (right4) {
					int k = i*nw+j;
					int m = k+4;
					spring[nsprings].mass[0] = k;
					spring[nsprings].mass[1] = m;
					spring[nsprings].length =
						getDistance(mass[k].pos, mass[m].pos);
					//spring[nsprings].stiffness = g.flagStiff;
					spring[nsprings].stiffness = rand() * 0.1 + 0.2;
					nsprings++;
				}
				if (down4) {
					int k = i*nw+j;
					int m = k+nw*4;
					spring[nsprings].mass[0] = k;
					spring[nsprings].mass[1] = m;
					spring[nsprings].length =
						getDistance(mass[k].pos, mass[m].pos);
					//spring[nsprings].stiffness = g.flagStiff;
					spring[nsprings].stiffness = rand() * 0.1 + 0.2;
					nsprings++;
				}
				#endif
			}
		}
	}
	//=====================================================================
	void maintain_springs(void) {
		int i,m0,m1;
		Flt dist,oodist,factor;
		Vec springVec;
		Vec springforce;
		//const Flt penalty=0.05f;
		//Move the masses...
		for (int i=0; i<nh; i++) {
			for (int j=0; j<nw; j++) {
				int k = i * nw + j;

                //Flag will detach from pole when wind speeds are too high!
                if (g.wind_strength >= 4.8)
                    mass[k].attached = 0;

				if (mass[k].attached) {
					vecZero(mass[k].vel);
					vecZero(mass[k].force);
					//continue;
				}
				mass[k].vel[0] += mass[k].force[0] * mass[k].oomass;
				mass[k].vel[1] += mass[k].force[1] * mass[k].oomass;
				mass[k].vel[2] += mass[k].force[2] * mass[k].oomass;
				mass[k].pos[0] += mass[k].vel[0];
				mass[k].pos[1] += mass[k].vel[1];
				mass[k].pos[2] += mass[k].vel[2];
				mass[k].force[0] =
				mass[k].force[1] =
				mass[k].force[2] = 0.0;
				//hit the floor?
				if (mass[k].pos[1] < 0.0) {
					mass[k].vel[0] *= 0.8;
					mass[k].vel[2] *= 0.8;
					mass[k].force[1] = -0.5 * (mass[k].pos[1]);
				}
				//Max velocity
				if (mass[k].vel[0] > 10.0) mass[k].vel[0] = 10.0;
				if (mass[k].vel[1] > 10.0) mass[k].vel[1] = 10.0;
				if (mass[k].vel[2] > 10.0) mass[k].vel[2] = 10.0;
				//Air resistance, or some type of damping
				mass[k].vel[0] *= 0.99;
				mass[k].vel[1] *= 0.99;
				mass[k].vel[2] *= 0.99;
			}
		}
		//Resolve all springs...
		for (i=0; i<nsprings; i++) {
			m0 = spring[i].mass[0];
			m1 = spring[i].mass[1];
			//forces are applied here
			//vector between the two masses
			springVec[0] = mass[m0].pos[0] - mass[m1].pos[0];
			springVec[1] = mass[m0].pos[1] - mass[m1].pos[1];
			springVec[2] = mass[m0].pos[2] - mass[m1].pos[2];
			//distance between the two masses
			dist = sqrt(
				springVec[0]*springVec[0] +
				springVec[1]*springVec[1] +
				springVec[2]*springVec[2]);
			if (dist == 0.0) dist = 0.1;
			oodist = 1.0 / dist; 
			springVec[0] *= oodist;
			springVec[1] *= oodist;
			springVec[2] *= oodist;
			//the spring force is added to the mass force
			//factor = -(dist - spring[i].length) * spring[i].stiffness;
			factor = -(dist - spring[i].length) * g.flagStiff;
			springforce[0] = springVec[0] * factor;
			springforce[1] = springVec[1] * factor;
			springforce[2] = springVec[2] * factor;
			//apply force and negative force to each end of the spring...
			mass[m0].force[0] += springforce[0];
			mass[m0].force[1] += springforce[1];
			mass[m0].force[2] += springforce[2];
			mass[m1].force[0] -= springforce[0];
			mass[m1].force[1] -= springforce[1];
			mass[m1].force[2] -= springforce[2];
			//damping
			springforce[0] = (mass[m1].vel[0] - mass[m0].vel[0]) * 0.002;
			springforce[1] = (mass[m1].vel[1] - mass[m0].vel[1]) * 0.002;
			springforce[2] = (mass[m1].vel[2] - mass[m0].vel[2]) * 0.002;
			mass[m0].force[0] += springforce[0];
			mass[m0].force[1] += springforce[1];
			mass[m0].force[2] += springforce[2];
			mass[m1].force[0] -= springforce[0];
			mass[m1].force[1] -= springforce[1];
			mass[m1].force[2] -= springforce[2];
		}
	}
	//=====================================================================
	void physics(void) {
		Vec norm;
		/*
		//show normals and quit
		Vec norm;
		for (int i=0; i<nh-1; i++) {
			for (int j=0; j<nw-1; j++) {
				int k = i * nw + j;
				int v0 = k;
				int v1 = v0+1;
				int v2 = v0+nw;
				int v3 = v2+1;
				Log("\nget normal for vertices...\n");
				Log("pos: %f %f %f\n",
				mass[v0].pos[0],mass[v0].pos[1],mass[v0].pos[2]);
				Log("     %f %f %f\n",
				mass[v2].pos[0],mass[v2].pos[1],mass[v2].pos[2]);
				Log("     %f %f %f\n",
				mass[v3].pos[0],mass[v3].pos[1],mass[v3].pos[2]);
				getTriangleNormal(
					mass[v0].pos, mass[v2].pos, mass[v3].pos, norm);
				Log("%i %i:  %f %f %f\n", i, j, norm[0], norm[1], norm[2]);
			}
		}
		exit(0);
		*/
		//for (int i=0; i<nh*nw; i++) {
		//	vecZero(mass[i].norm);
		//}
		for (int i=0; i<nh-1; i++) {
			for (int j=0; j<nw-1; j++) {
				int k = i * nw + j;
				//mass[k].force[1] -= 0.002;
				mass[k].force[1] += g.gravity;
				//if (mass[k].attached) {
				//	vecZero(mass[k].force);
				//	vecZero(mass[k].vel);
				//	continue;
				//}
				// 0        1
				// +--------+
				// |\       |
				// | \   2  |
				// |  \     |
				// |   \    |
				// |    \   |
				// |     \  |
				// |  1   \ |
				// |       \|
				// +--------+
				// 2        3
				//
				//save normals for each vertex.
				int v0 = k;
				int v1 = v0+1;
				int v2 = v0+nw;
				int v3 = v2+1;
				getTriangleNormal(
					mass[v0].pos, mass[v3].pos, mass[v2].pos, norm);
				/*
				if (j==0 && i==3) {
				Log("\n1 before accum...\n");
				Log("pos: %f %f %f\n",
				mass[v0].pos[0],mass[v0].pos[1],mass[v0].pos[2]);
				Log("     %f %f %f\n",
				mass[v2].pos[0],mass[v2].pos[1],mass[v2].pos[2]);
				Log("     %f %f %f\n",
				mass[v3].pos[0],mass[v3].pos[1],mass[v3].pos[2]);
				Log("adding this norm %f %f %f\n",norm[0],norm[1],norm[2]);
				Log("mass[v0].norm %f %f %f\n",
				mass[v0].norm[0],mass[v0].norm[1],mass[v0].norm[2]);
				}
				*/
				vecAdd(norm, mass[v0].norm, mass[v0].norm);
				vecAdd(norm, mass[v2].norm, mass[v2].norm);
				vecAdd(norm, mass[v3].norm, mass[v3].norm);
				/*
				if (j==0 && i==3) {
				Log("\n1 after accum...\n");
				Log("pos: %f %f %f\n",
				mass[v0].pos[0],mass[v0].pos[1],mass[v0].pos[2]);
				Log("     %f %f %f\n",
				mass[v2].pos[0],mass[v2].pos[1],mass[v2].pos[2]);
				Log("     %f %f %f\n",
				mass[v3].pos[0],mass[v3].pos[1],mass[v3].pos[2]);
				Log("added this norm %f %f %f\n",norm[0],norm[1],norm[2]);
				Log("mass[v0].norm %f %f %f\n",
				mass[v0].norm[0],mass[v0].norm[1],mass[v0].norm[2]);
				}
				*/
				getTriangleNormal(
					mass[v0].pos, mass[v1].pos, mass[v3].pos, norm);
				/*
				if (j==0 && i==3) {
				Log("\n2 before accum...\n");
				Log("pos: %f %f %f\n",
				mass[v0].pos[0],mass[v0].pos[1],mass[v0].pos[2]);
				Log("     %f %f %f\n",
				mass[v3].pos[0],mass[v3].pos[1],mass[v3].pos[2]);
				Log("     %f %f %f\n",
				mass[v1].pos[0],mass[v1].pos[1],mass[v1].pos[2]);
				Log("adding this norm %f %f %f\n",norm[0],norm[1],norm[2]);
				Log("mass[v0].norm %f %f %f\n",
				mass[v0].norm[0],mass[v0].norm[1],mass[v0].norm[2]);
				}
				*/
				vecAdd(norm, mass[v0].norm, mass[v0].norm);
				vecAdd(norm, mass[v3].norm, mass[v3].norm);
				vecAdd(norm, mass[v1].norm, mass[v1].norm);
				/*
				if (j==0 && i==3) {
				Log("\n2 after accum...\n");
				Log("pos: %f %f %f\n",
				mass[v0].pos[0],mass[v0].pos[1],mass[v0].pos[2]);
				Log("     %f %f %f\n",
				mass[v3].pos[0],mass[v3].pos[1],mass[v3].pos[2]);
				Log("     %f %f %f\n",
				mass[v1].pos[0],mass[v1].pos[1],mass[v1].pos[2]);
				Log("added this norm %f %f %f\n",norm[0],norm[1],norm[2]);
				Log("mass[v0].norm %f %f %f\n",
				mass[v0].norm[0],mass[v0].norm[1],mass[v0].norm[2]);
				}
				*/
				/*
				if (j==0 && i==3) {
				Log("unnormalized mass[v0].norm: %f %f %f  v0: %i\n",
				mass[v0].norm[0],mass[v0].norm[1],mass[v0].norm[2], v0);
				vecNormalize(mass[v0].norm);
				Log("  normalized mass[v0].norm: %f %f %f\n",
				mass[v0].norm[0],mass[v0].norm[1],mass[v0].norm[2]);
				vecNormalize(mass[v1].norm);
				vecNormalize(mass[v2].norm);
				vecNormalize(mass[v3].norm);
				//Log("ns:v0 %f %f %f\n",
				mass[v0].norm[0],mass[v0].norm[1],mass[v0].norm[2]);
				//Log("   v1 %f %f %f\n",
				mass[v1].norm[0],mass[v1].norm[1],mass[v1].norm[2]);
				//Log("   v2 %f %f %f\n",
				mass[v2].norm[0],mass[v2].norm[1],mass[v2].norm[2]);
				//Log("   v3 %f %f %f\n",
				mass[v3].norm[0],mass[v3].norm[1],mass[v3].norm[2]);
				}
				*/
			}
		}
		//apply force from wind
		//Vec wind = {.1, .1, .02};
		//Vec wind = {.1, .04, .02};
		//Vec wind = {.07, .01, .01};
		Vec v, w;
		vecCopy(g.wind_dir, w);
		vecScale(g.wind_strength, w, w);
		vecCopy(w, v);
		vecNormalize(v);
		Vec f;
		for (int i=0; i<nh*nw; i++) {
			if (mass[i].attached) {
				//vecZero(mass[i].force);
				//vecZero(mass[i].vel);
				vecNormalize(mass[i].norm);
				continue;
			}
			vecNormalize(mass[i].norm);
			Flt dot = vecDotProduct(mass[i].norm, v);
			if (dot < 0.0) dot = -dot;
			dot *= 0.05;
			//vecScale(dot, g.wind, f);
			vecScale(dot, w, f);
			vecAdd(f, mass[i].force, mass[i].force);
		}
		/*
		//show normals and quit
		Vec norm;
		for (int i=0; i<nh-1; i++) {
			for (int j=0; j<nw-1; j++) {
				int k = i * nw + j;
				int v0 = k;
				int v1 = v0+1;
				int v2 = v0+nw;
				int v3 = v2+1;
				Log("\nget normal for vertices...\n");
				Log("pos: %f %f %f\n",
				mass[v0].pos[0],mass[v0].pos[1],mass[v0].pos[2]);
				Log("     %f %f %f\n",
				mass[v2].pos[0],mass[v2].pos[1],mass[v2].pos[2]);
				Log("     %f %f %f\n",
				mass[v3].pos[0],mass[v3].pos[1],mass[v3].pos[2]);
				getTriangleNormal(
					mass[v0].pos, mass[v2].pos, mass[v3].pos, norm);
				Log("%i %i:  %f %f %f\n", i, j, norm[0], norm[1], norm[2]);
			}
		}
		exit(0);
		*/
	}
} flag;



class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper(void) {
		GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setup_screen_res(640, 480);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			printf("\n\tcannot connect to X server\n\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask =
			ExposureMask |
			KeyPressMask |
			KeyReleaseMask |
			StructureNotifyMask |
			SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
			vi->depth, InputOutput, vi->visual,
			CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
	}
	~X11_wrapper(void) {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void set_title(void) {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Flag Lab");
	}
	void setup_screen_res(const int w, const int h) {
		g.xres = w;
		g.yres = h;
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		//
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		set_title();
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
} x11;

void init_opengl(void);
void init_textures(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);

int main(void)
{
	init_opengl();
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	init_textures();
	//setup_flag();
	int done = 0;
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
	cleanup_fonts();
	return 0;
}

void getTriangleNormal(Vec t0, Vec t1, Vec t2, Vec norm)
{
	Vec v0,v1;
	vecSub(t1, t0, v0);
	vecSub(t2, t0, v1);
	vecCrossProduct(v0, v1, norm);
	vecNormalize(norm);
}

void init_textures(void)
{
	//create opengl texture elements
	glGenTextures(1, &flag.textureId);
	int w = img[0].width;
	int h = img[0].height;
	glBindTexture(GL_TEXTURE_2D, flag.textureId);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[0].data);
	//
	glGenTextures(1, &g.feltTextureId);
	w = img[1].width;
	h = img[1].height;
	glBindTexture(GL_TEXTURE_2D, g.feltTextureId);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[1].data);
}



void init_opengl(void)
{
	glViewport(0, 0, g.xres, g.yres);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)g.xres/(GLfloat)g.yres,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,5,10,  0,0,0,  0,1,0);
	//Enable this so material colors are the same as vert colors.
	glEnable(GL_COLOR_MATERIAL);
	glEnable( GL_LIGHTING );
	glLightfv(GL_LIGHT0, GL_AMBIENT, g.lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g.lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g.lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, g.lightPosition);
	glEnable(GL_LIGHT0);
}

void vecCrossProduct(Vec v0, Vec v1, Vec dest)
{
	dest[0] = v0[1]*v1[2] - v1[1]*v0[2];
	dest[1] = v0[2]*v1[0] - v1[2]*v0[0];
	dest[2] = v0[0]*v1[1] - v1[0]*v0[1];
}

Flt vecDotProduct(Vec v0, Vec v1)
{
	return v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2];
}

void vecZero(Vec v)
{
	v[0] = v[1] = v[2] = 0.0;
}

void vecMake(Flt a, Flt b, Flt c, Vec v)
{
	v[0] = a;
	v[1] = b;
	v[2] = c;
}

void vecCopy(Vec source, Vec dest)
{
	dest[0] = source[0];
	dest[1] = source[1];
	dest[2] = source[2];
}

Flt vecLength(Vec v)
{
	return sqrt(vecDotProduct(v, v));
}

Flt getDistance(Vec v0, Vec v1)
{
	Vec v;
	vecSub(v0, v1, v);
	return vecLength(v);
}

void vecNormalize(Vec v)
{
	Flt len = vecLength(v);
	if (len == 0.0) {
		Log("-->warning: len=0 when normalizing!");
		Log("  %f %f %f\n", v[0], v[1], v[2]);
		vecMake(0,0,1,v);
		return;
	}
	len = 1.0 / len;
	v[0] *= len;
	v[1] *= len;
	v[2] *= len;
}

void vecSub(Vec v0, Vec v1, Vec dest)
{
	dest[0] = v0[0] - v1[0];
	dest[1] = v0[1] - v1[1];
	dest[2] = v0[2] - v1[2];
}

void vecAdd(Vec v0, Vec v1, Vec dest)
{
	dest[0] = v0[0] + v1[0];
	dest[1] = v0[1] + v1[1];
	dest[2] = v0[2] + v1[2];
}

void vecScale(Flt sc, Vec v, Vec dest)
{
	dest[0] = v[0] * sc;
	dest[1] = v[1] * sc;
	dest[2] = v[2] * sc;
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
	static int shift = 0;
	static int ctrl = 0;
	static int alt = 0;
	//Was there input from the keyboard?
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	//printf("key: %i   shift: %i\n", key, shift); fflush(stdout);
	if (e->type == KeyRelease) {
		//shift, ctrl, or alt key released?
		if (key == XK_Shift_L || key == XK_Shift_R) {
			//printf("clearing shift\n"); fflush(stdout);
			shift = 0;
			return 0;
		}
		if (key == XK_Control_L || key == XK_Control_R) { ctrl=0; return 0; }
		if (key == XK_Alt_L || key == XK_Alt_R) { alt = 0; return 0; }
		//don't process any other keys on a release
		return 0;
	}
	if (key == XK_Shift_L || key == XK_Shift_R) {shift = 1; return 0; }
	if (key == XK_Control_L || key == XK_Control_R) { ctrl = 1; return 0; }
	if (key == XK_Alt_L || key == XK_Alt_R) { alt = 1; return 0; }
	(void)alt;
	(void)ctrl;
	switch (key) {
		case XK_g:
			if (!shift) {
				if (g.gravity > -.2)
					g.gravity *= 1.01;
			} else {
				if (g.gravity < -0.00005)
					g.gravity *= (1.0 / 1.01);
			}
			break;
		case XK_w:
			if (!shift) {
				if (g.wind_strength < 5.0)
					g.wind_strength *= 1.01;
			} else {
				if (g.wind_strength > 0.0005)
					g.wind_strength *= (1.0 / 1.01);
			}
			break;
		case XK_i:
			flag.wire ^= 1;
			break;
		case XK_Left:
			break;
		case XK_Right:
			break;
		case XK_l:
			//set light position
			g.lightPosition[0] = rnd() * 200.0 - 100.0;
			g.lightPosition[1] = rnd() * 100.0 + 20.0;
			g.lightPosition[2] = rnd() * 300.0 - 150.0;
			break;
        case XK_s:
            if (!shift) {
                if (g.flagStiff < 0.27)
                    g.flagStiff *= 1.01;
            } else {
                if (g.flagStiff > 0.05)
                    g.flagStiff *= (1.0 / 1.01);
            }
            break;
		case XK_Escape:
			return 1;
	}
	return 0;
}

void drawFlag(void)
{
	//the floor
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.feltTextureId);
	glBegin(GL_QUADS);
		glNormal3f(0.0,1.0,0.0);
		glTexCoord2f( 0.0,  0.0); glVertex3f(-20.0, 0.0, -20.0);
		glTexCoord2f( 0.0, 10.0); glVertex3f(-20.0, 0.0,  20.0);
		glTexCoord2f(10.0, 10.0); glVertex3f( 20.0, 0.0,  20.0);
		glTexCoord2f(10.0,  0.0); glVertex3f( 20.0, 0.0, -20.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	//
	//the flag
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, flag.textureId);
	if (flag.wire) {
		glBegin(GL_LINES);
			for (int k=0; k<flag.nsprings; k++) {
				int i = flag.spring[k].mass[0];
				int j = flag.spring[k].mass[1];
				//
				// 0        1
				// +--------+
				// |\       |
				// | \   2  |
				// |  \     |
				// |   \    |
				// |    \   |
				// |     \  |
				// |  1   \ |
				// |       \|
				// +--------+
				// 2        3
				//
				glNormal3fv(flag.mass[i].norm);
				glTexCoord2fv(flag.mass[i].texcoord);
				glVertex3fv(flag.mass[i].pos);
				glNormal3fv(flag.mass[j].norm);
				glTexCoord2fv(flag.mass[j].texcoord);
				glVertex3fv(flag.mass[j].pos);
			}
		glEnd();
	} else {
		glBegin(GL_TRIANGLES);
			for (int i=0; i<flag.nh-1; i++) {
				for (int j=0; j<flag.nw-1; j++) {
					//
					// 0        1
					// +--------+
					// |\       |
					// | \   2  |
					// |  \     |
					// |   \    |
					// |    \   |
					// |     \  |
					// |  1   \ |
					// |       \|
					// +--------+
					// 2        3
					//
					int v0 = i*flag.nw+j;
					int v1 = v0+1;
					int v2 = v0+flag.nw;
					int v3 = v2+1;
					//
					glNormal3fv(flag.mass[v0].norm);
					glTexCoord2fv(flag.mass[v0].texcoord);
					glVertex3fv(flag.mass[v0].pos);
					glNormal3fv(flag.mass[v2].norm);
					glTexCoord2fv(flag.mass[v2].texcoord);
					glVertex3fv(flag.mass[v2].pos);
					glNormal3fv(flag.mass[v3].norm);
					glTexCoord2fv(flag.mass[v3].texcoord);
					glVertex3fv(flag.mass[v3].pos);
					//
					glNormal3fv(flag.mass[v0].norm);
					glTexCoord2fv(flag.mass[v0].texcoord);
					glVertex3fv(flag.mass[v0].pos);
					glNormal3fv(flag.mass[v3].norm);
					glTexCoord2fv(flag.mass[v3].texcoord);
					glVertex3fv(flag.mass[v3].pos);
					glNormal3fv(flag.mass[v1].norm);
					glTexCoord2fv(flag.mass[v1].texcoord);
					glVertex3fv(flag.mass[v1].pos);
				}
			}
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void physics(void)
{ 
	if (g.gravity_auto_start) {
		g.gravity -= 0.00001;
		if (g.gravity <= -0.001) {
			g.gravity = -0.001;
			g.gravity_auto_start = 0;
		}
	}
	flag.physics();
	flag.maintain_springs();
}

void render(void)
{
	Rect r;
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)g.xres/(GLfloat)g.yres,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0,5.5,11,  0,5.5,0,  0,1,0);
	gluLookAt(-1,6.0,10,  flag.mass[230].pos[0],flag.mass[230].pos[1],flag.mass[230].pos[2],  0,1,0);
	//Enable this so material colors are the same as vert colors.
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, g.lightPosition);
	drawFlag();
	//
	//set 2D mode
	//
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	r.bot = 75;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00887766, "flag");
	ggprint8b(&r, 16, 0x008877aa, "I - Wire-frame");
	ggprint8b(&r, 16, 0x008877aa, "G - Gravity (%f)", g.gravity);
	ggprint8b(&r, 16, 0x008877aa, "W - Wind Strength (%f) | HIGH WINDS AT 4.8!", g.wind_strength);
    ggprint8b(&r, 16, 0x008877aa, "S - Stiffness (%f)", g.flagStiff);
}



