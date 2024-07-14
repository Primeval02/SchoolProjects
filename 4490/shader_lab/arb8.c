//
//
//
//
//written by : Gordon Griesel
//date: summer 2018 to 2019
//
//This program was derived from:
//http://www.devmaster.net/main.php?showtid=280
//arb_vertex_program.c
//
//additional sources
//An example of defining and using an ARB vertex program.
//found at:
//https://www.opengl.org/discussion_boards/
//showthread.php/165165-Sample-use-of-shaders-on-Linux
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
 
GLuint VP_handle = 0;
 
const char VP_source[] =
"!!ARBvp1.0\n"
"\n"
"#Input\n"
"ATTRIB InPos   = vertex.position;\n"
"ATTRIB InColor = vertex.color;\n"
"\n"
"#Output\n"
"OUTPUT OutPos = result.position;\n"
"OUTPUT OutColor = result.color;\n"
"\n"
"PARAM MVP[4] = { state.matrix.mvp }; # Modelview Projection Matrix\n"
"TEMP Temp;\n"
"\n"
"# Transform vertex to clip space\n"
"DP4 Temp.x, MVP[0], InPos;\n"
"DP4 Temp.y, MVP[1], InPos;\n"
"DP4 Temp.z, MVP[2], InPos;\n"
"DP4 Temp.w, MVP[3], InPos;\n"
"\n"
"#Output\n"
"MOV OutPos, Temp;\n"
"MOV OutColor, InColor;\n"
"\n"
"END\n";

void render(void);
void reshape(int width, int height);
void initVertexProgram(void);
float angle = 30.0;

void handleKeyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 27:
            exit(0);
            break;
        // Move rectangle    
        case 'r':
            angle *= 1.01;
            break;
        // Move opposite dir.
        case 'R':
            angle *= (1.0 / 1.01);
            break;
    }
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyboard);
	glutReshapeWindow(400,400);
	printf("GL_RENDERER = %s\n",glGetString(GL_RENDERER));
	glClearColor(0.0, 0.2, 0.58, 0.0);
	initVertexProgram();
	glutMainLoop();
	return 0;
}
 
void checkGLError( const char hdr[] )
{
	int err = glGetError();
	if (err) {
		fprintf(stderr, "ERROR %s: %s\n", hdr, gluErrorString(err));
		exit(1);
	}
}

void initVertexProgram(void)
{
	// Allocate
	glGenProgramsARB(1, &VP_handle);
	checkGLError("Allocating Vertex Program");
	// Activate
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, VP_handle);
	checkGLError("Binding Vertex Program");
	// Compile
	glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
		strlen(VP_source), VP_source);
	if (glGetError() == GL_INVALID_OPERATION) {
		fprintf(stderr, "ERROR Compiling Vertex Program:\n");
		// Find the error position
		GLint errPos;
		glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errPos);
		// Print implementation-dependent program errors and warnings
		const GLubyte *errString = glGetString(GL_PROGRAM_ERROR_STRING_ARB);
		fprintf(stderr, "Error at position: %d\n%s\n", errPos, errString);
		exit(1);
	}
	checkGLError("Compiling Vertex Program");
}

void activateVertexProgram()
{
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, VP_handle);
	checkGLError("Binding Vertex Program");
	glEnable(GL_VERTEX_PROGRAM_ARB);
	checkGLError("Enabling Vertex Program");
}

void deactivateVertexProgram()
{
	glDisable(GL_VERTEX_PROGRAM_ARB);
	checkGLError("Disabling Vertex Program");
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}


void render(void)
{
	//int err=0;
    printf("angle: %f", angle);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1, -1,1, -1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(angle, 0,0,1);
	glColor4f(1.0, 0.78, 0.17, 1.0);
	activateVertexProgram();
	glRectf(-0.5, -0.5, 0.5, 0.5);
	deactivateVertexProgram();
	glutSwapBuffers();
	checkGLError("End of display()");
}
 

