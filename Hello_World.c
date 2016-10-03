/* example from: http://pages.cpsc.ucalgary.ca/~olsenl/wiki/pmwiki.php/TA453/Lab01CppIntro
*/

/*
** Makefile - for suse linux

MOTIF_IPATH = -I/usr/X11R6/include
MOTIF_LIB =-L/usr/X11R6/lib64

INCLUDES = $(MOTIF_IPATH)
LIBS = $(MOTIF_LIB)

Hello_World : Hello_World.c
	g++ -o Hello_World Hello_World.c  $(INCLUDES) $(MOTIF_LIB) -lGL -lGLU -lglut -lXi -lXmu -lX11

clean:
	rm Hello_World
	
*/


#include <stdlib.h>
#include <GL/glut.h>

void DrawGL() ;
void ResizeGL(int w, int h);



 int main(int argc, char** argv) 
 {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Hello World GL");
	glutDisplayFunc(DrawGL);
	glutReshapeFunc(ResizeGL);
	//InitGL();
	glutMainLoop();
	return 0;
 }


// This is where you draw your stuff to the screen
 // (shouldn't be doing heavy computations in here though)
 void DrawGL() 
 {
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	char* str = "Hello World";
	int len = 11, i;

	glRasterPos2d(0, 0);
	for (i=0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

	glutSwapBuffers();
 }
 
 // This is where you define your viewing projection
 // Called by the window manager whenever the window is resized
 void ResizeGL(int w, int h)
 {
	glViewport(0,0,w,h); // set the drawable region of the window
	glMatrixMode(GL_PROJECTION); // set up the projection matrix
	glLoadIdentity(); // clear any previous transform and set to the identity matrix
	glOrtho(-1,1,-1,1,-1,1); // use an orthographic projection
	glMatrixMode(GL_MODELVIEW); // go back to modelview matrix
 }
