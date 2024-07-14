//
//
//written by: Gordon Griesel
//date: Summer 2018
//Example of OpenGL texture using GLSL shaders.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define GL_GLEXT_PROTOTYPES
//Must include the glew header
//Makefile must link the glew lib
//example:
//g++ shader.cpp -Wall -o shader -lX11 -lGL -lGLU -lGLEW -lm
#include <GL/glew.h>
//the gl.h must be included after glew.h
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
//
#include <X11/Xlib.h>
#include <X11/keysym.h>

//simple vertex shader
const char vs1[] =  
"void main() {                                                   \n"
"   gl_FrontColor = gl_Color;                                    \n"
"   gl_Position=gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;\n"
"}                                                               \n";
//simple fragment shader
const char fs1[] =  
"void main() {                                      \n"
"  gl_FragColor = gl_Color;                         \n"
"}                                                  \n";
//source for help
//http://www.youtube.com/user/thecplusplusguy
//vertex shader
const char vs2[] = R"abc(
#version 120
uniform vec3 color;
varying vec3 color2;
attribute vec3 atr;
varying vec2 texcoord;
void main() {
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
	color2 = atr;
	texcoord = gl_MultiTexCoord0.xy;
})abc";

//fragment shader
const char fs2[] = R"abc(
#version 120
uniform sampler2D img;
uniform sampler2D img2;
uniform vec3 color;
varying vec3 color2;
varying vec2 texcoord;
void main() {
	vec4 texcolor = texture2D(img,texcoord);
	vec4 texcolor2 = texture2D(img2,texcoord);
	//gl_FragColor = texcolor;
	gl_FragColor = texcolor2*0.5 + texcolor*0.5;
})abc";

const unsigned int SZF = (unsigned int)sizeof(float);

enum Render_types {
	IMMEDIATE_MODE,
	VERTEX_ELEMENTS,
	END_OF_MODES
};

class Global {
public:
	float angle;
	int draw_state;
	int shader_state;
	int texture_state;
	GLuint texID1, texID2;
	Global() {
		angle = 0.0f;
		draw_state = IMMEDIATE_MODE;
		shader_state = 0;
		texture_state = 0;
	}
} g;

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		int slen = strlen(fname);
		if (slen <= 0)
			return;
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0) {
			ppmFlag = 1;
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
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
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
			printf("ERROR opening image: %s\n", ppmname);
			exit(0);
		}
		if (!ppmFlag)
			unlink(ppmname);
	}
} img = "tree.gif", img2 = "zebra.png";

class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
	int xres, yres;
public:
	X11_wrapper() {
		xres = 640;
		yres = 480;
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setup_screen_res(xres, yres);
		dpy = XOpenDisplay(NULL);
		if (!dpy) {
			printf("cannot connect to X server\n\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (!vi) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask =
			ExposureMask |
			StructureNotifyMask |
			SubstructureNotifyMask |
			KeyPressMask |
			KeyReleaseMask |
			PointerMotionMask |
			ButtonPressMask |
			ButtonReleaseMask;
		win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
			vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
	}
	~X11_wrapper() { XDestroyWindow(dpy, win); XCloseDisplay(dpy); }
	int getxres() { return xres; }
	int getyres() { return yres; }
	void set_title(void) {
		//Set the window title bar.
		XMapWindow(dpy, win);
		char ts[256];
		sprintf(ts, "OpenGL Shader->%s  Texture->%s  (arrows,n,s,t,esc)",
		((g.shader_state)?"ON ":"OFF"),
		((g.texture_state)?"ON ":"OFF"));
		XStoreName(dpy, win, ts);
	}
	void setup_screen_res(const int w, const int h) { xres = w; yres = h; }
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != xres || xce.height != yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
			xres = xce.width;
			yres = xce.height;
		}
	}
	bool getXPending() { return XPending(dpy); }
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void swapBuffers() { glXSwapBuffers(dpy, win); }
} x11;

void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);
void init_vbo(void);
void checkGLError(const char hdr[]);
void setupTexture(void);

GLuint program;
GLuint program2;
GLuint LoadShader(GLenum type, const char *shaderSrc);
GLuint setup_shaders(const char *vs, const char *fs);


//===========================================================================
// M A I N
//===========================================================================
int main(void)
{
	program = setup_shaders(vs1, fs1);
	program2 = setup_shaders(vs2, fs2);
	init_vbo();
	setupTexture();
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
	return 0;
}

/*unsigned char *xbuildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	//Use the first pixel in the image as the transparent color.
	//Modify this as necessary.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	unsigned char a,b,c;
	for (int i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}
*/

void setupTexture(void)
{
	//Do this to allow textures
	//no need when using shaders, but ok.
	glEnable(GL_TEXTURE_2D);
	//
	int w = img.width;
	int h = img.height;
	glGenTextures(1, &g.texID1);
	//-------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, g.texID1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img.data);
	//
	w = img2.width;
	h = img2.height;
	glGenTextures(1, &g.texID2);
	//-------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, g.texID2);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img2.data);
}

void checkGLError(const char *hdr)
{
	int err = glGetError();
	if (err) {
		fprintf(stderr, "ERROR %s: %s\n", hdr, gluErrorString(err));
		exit(1);
	}
}

GLuint LoadShader(GLenum type, const char *shaderSrc)
{
	printf("LoadShader()...\n"); fflush(stdout);
	GLuint shader;
	GLint compiled;
	// Create the shader object
	printf("Create the shader object\n"); fflush(stdout);
	shader = glCreateShader(type);
	if (shader == 0) {
		printf("ERROR: shader not created. returning 0\n"); fflush(stdout);
		return 0;
	}
	// Load the shader source
	printf("Load the shader source\n"); fflush(stdout);
	glShaderSource(shader, 1, &shaderSrc, NULL);
	// Compile the shader
	printf("Compile the shader\n"); fflush(stdout);
	glCompileShader(shader);
	// Check the compile status
	printf("Check the compile status\n"); fflush(stdout);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1) {
			char *infoLog = (char *)malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			//esLogMessage("Error compiling shader:\n%s\n", infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
		return 0;
	}
	printf("return a good value!\n"); fflush(stdout);
	return shader;
}

//GLuint init_glsl()
GLuint setup_shaders(const char *vs, const char *fs)
{
	printf("setup_shaders()...\n"); fflush(stdout);
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;
	//
	printf("call glewInit()\n"); fflush(stdout);
	GLenum err = glewInit();
	if (err) {
		printf("error glewInit()\n"); fflush(stdout);
		return 0;
	}
	//Load the vertex/fragment shaders
	//printf("Load the vertex/fragment shaders\n"); fflush(stdout);
	vertexShader = LoadShader(GL_VERTEX_SHADER, vs);
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fs);
	// Create the program object
	//printf("Create the program object\n"); fflush(stdout);
	programObject = glCreateProgram();
	if (programObject == 0) {
		printf("error glCreateProgram()\n"); fflush(stdout);
		return 0;
	}
	//printf("Attach shaders\n"); fflush(stdout);
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
	// Bind vPosition to attribute 0   
	printf("Bind vPosition to attribute 0\n"); fflush(stdout);
	glBindAttribLocation(programObject, 0, "vPosition");
	// Link the program
	printf("Link the program\n"); fflush(stdout);
	glLinkProgram(programObject);
	// Check the link status
	printf("Check the link status\n"); fflush(stdout);
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1) {
			char *infoLog = (char *)malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			printf("Error linking program: **%s**\n", infoLog);
			free(infoLog);
		}
		glDeleteProgram(programObject);
		return 0;
	}
	//Return the program object
	return programObject;
}

void check_mouse(XEvent *e)
{
	//no mouse in this program
	if (e->type != ButtonPressMask && e->type != ButtonReleaseMask &&
		e->type != PointerMotionMask)
		return;
}

void screenCapture()
{
	static int inc = 0;
	int xres = x11.getxres();
	int yres = x11.getyres();
	//get pixels
	unsigned char *data = new unsigned char [xres * yres * 3]; 
    glReadPixels(0, 0, xres, yres, GL_RGB, GL_UNSIGNED_BYTE, data);
	//write ppm file...
	char ts[256];
	sprintf(ts, "img%03i.ppm", inc++);
	FILE *fpo = fopen(ts, "w");
	fprintf(fpo, "P6\n");
	fprintf(fpo, "%i %i\n", xres, yres);
	fprintf(fpo, "255\n");
	//go backwards a row at a time...
	unsigned char *p = data;
	p = p + ((yres-1) * xres * 3);
	unsigned char *start = p;
	for (int i=0; i<yres; i++) {
		for (int j=0; j<xres*3; j++) {
			fprintf(fpo, "%c", *p);
			++p;
		}
		start = start - (xres*3);
		p = start;
	}
	fclose(fpo);
}

int check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyPress) {
		switch (key) {
			case XK_n:
				if (++g.draw_state >= END_OF_MODES)
					g.draw_state = IMMEDIATE_MODE;
				x11.set_title();
				break;
			case XK_s:
				g.shader_state ^= 1;
				x11.set_title();
				break;
			case XK_t:
				g.texture_state ^= 1;
				x11.set_title();
				break;
			case XK_Right:
				g.angle -= 3.0f;
				break;
			case XK_Left:
				g.angle += 3.0f;
				break;
			case XK_Escape:
				return 1;
				break;
		}
	}
	return 0;
}

void physics(void)
{

}


//============================================================================
//============================================================================
float f[]={0,2,-6,  -2,-2,-6,  2,-2,-6};
float c[]={1,1,0,   0,1,0,     0,0,1};
float t[]={0.5, 0.0, 0.0, 1.0, 1.0, 1.0};
float fc[]= {0,2,-6, 1,0,0,  -2,-2,-6,  0,1,0, 2,-2,-6,  0,0,1};
float ft[]= {0,2,-6, 0.5,0.0,   -2,-2,-6, 0.0,1.0,   2,-2,-6,1.0,1.0 };
float fct[]= {
 0, 2,-6,  1,0,0,  0.5,0.0,
-2,-2,-6,  0,1,0,  0.0,1.0,
 2,-2,-6,  0,0,1,  1.0,1.0 };
unsigned int ind[]={0,1,2};
unsigned int vbo, elem;

void init_vbo(void)
{
	printf("init_vbo()...\n"); fflush(stdout);
	glClearColor(0, 0, .3, 1);
	//
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &elem);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fct), fct, GL_STATIC_DRAW);
	//                                              GL_a_b
	//                                              a: STREAM, STATIC, DYNAMIC
	//                                              b: DRAW, READ, COPY
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//
	//printf("leaving init_vbo()...\n"); fflush(stdout);
}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1,1,-1,1,-1,1);
	gluPerspective(50, 640.0/480.0, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(g.angle, 0, 0, 1);
	if (g.shader_state) {
		glUseProgram(program);
		if (g.texture_state)
			glUseProgram(program2);
	}
	switch (g.draw_state) {
		case IMMEDIATE_MODE:
			if (g.texture_state) {
				glUniform3f(glGetUniformLocation(program2,"color"),0,0,1);
				int index = glGetAttribLocation(program2, "atr");
				//
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, g.texID1);
				//
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, g.texID2);
				//
				glUniform1i(glGetUniformLocation(program2, "img"),0);
				glUniform1i(glGetUniformLocation(program2, "img2"),1);
				glBegin(GL_TRIANGLES);
					glTexCoord2f(0.5, 0.0);
					glVertex3f( 0.0, 2.0, -6.0);
					glTexCoord2f(0.0, 1.0);
					glVertex3f(-2.0, -2.0, -6.0);
					glTexCoord2f(1.0, 1.0);
					glVertex3f( 2.0, -2.0, -6.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
			} else {
				glBegin(GL_TRIANGLES);
					glColor3ub(255,0,0);
					glVertex3f(0.0, 2.0, -6.0);
					glColor3ub(255,255,255);
					glVertex3f(-2.0, -2.0, -6.0);
					glColor3ub(0,0,255);
					glVertex3f(2.0, -2.0, -6.0);
				glEnd();
			}
			break;
		case VERTEX_ELEMENTS:
			//vao
			if (g.texture_state) {
				glColor3ub(255,255,255);
				glBindTexture(GL_TEXTURE_2D, g.texID1);
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, f);
				glColorPointer(3, GL_FLOAT, 0, c);
				glTexCoordPointer(2, GL_FLOAT, 0, t);
				glDrawArrays(GL_TRIANGLES, 0, 3);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
				glBindTexture(GL_TEXTURE_2D, 0);
			} else {
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, f);
				glColorPointer(3, GL_FLOAT, 0, c);
				glDrawArrays(GL_TRIANGLES, 0, 3);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
			break;
	}
	if (g.shader_state) {
		glUseProgram(0);
	}
	//checkGLError("End of render()");
}






























