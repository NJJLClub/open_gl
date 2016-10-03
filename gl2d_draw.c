/* 
*
*  Intro to Opengl here:
*   	http://www.comp.leeds.ac.uk/marcelo/opengl/index.html
*/

/*
** Makefile - for suse linux

MOTIF_IPATH = -I/usr/X11R6/include
MOTIF_LIB =-L/usr/X11R6/lib64

INCLUDES = $(MOTIF_IPATH)
LIBS = $(MOTIF_LIB)

gl2d_draw : gl2d_draw.c
	g++ -o gl2d_draw gl2d_draw.c  $(INCLUDES) $(MOTIF_LIB) -lGL -lGLU -lglut -lXi -lXmu -lX11

clean:
	rm gl2d_draw

	
*/


#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

GLfloat gtx = 0.0, gtz = -8.0, gty = -3.0 ;  // control translation in x,y,z axis
GLfloat angle_rot=0.0;


void DrawGL() ;
void keyboard(unsigned char key, int x, int y );


 int main(int argc, char** argv) 
 {
	glutInit(&argc, argv);      	    	    	    	    // Initializes the GLUT library
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE);       	    // init params for the display, we are
	    	    	    	    	    	    	    	    // specifying RGB display along with double
								    // buffering, so the screen won't flicker when
								    // we redraw it. 	glutInitWindowSize(500, 500);	    	    	    	    // Initial window Size
	
	glutCreateWindow("Test gl_draw");   	    	    	    // Creates and sets the window title
	glutDisplayFunc(DrawGL);
    	glutKeyboardFunc(keyboard); 	    	    	    	    // Handle keyboard events


    	glMatrixMode( GL_PROJECTION );	    	    	    	    // Informs OpenGL that we are going to change the projection matrix
	  glLoadIdentity(); 	    	    	    	    	    // this loads an identity matrix
	  gluOrtho2D( 0,1,0,1 );    	    	    	    	    // this sets the window limits -xmin,xmax,ymin,ymax and is setting
	    	    	    	    	    	    	    	    // an orthographic (2D) transformation.

	glMatrixMode( GL_MODELVIEW );	    	    	    	    // this informs OpenGL that we are going to change the modelview matrix

	glutMainLoop();     	    	    	    	    	    // Starts the main event loop
	return 0;
 }


// This is where you draw your stuff to the screen
 // (shouldn't be doing heavy computations in here though)
 void DrawGL() 
 {
    	glClearColor(0, 0, 0, 0 );  	    	    	    	    // this sets teh color to fill the background
	glClear(GL_COLOR_BUFFER_BIT);   	    	    	    // this fills the background with bg color
	    	    	    	    	    	    	    	    // GL_COLOR_BUFFER_BIT - clear just the image

    	glColor3f(0, 1, 0);
	glBegin( GL_TRIANGLES );
	  glVertex2f(0,0);
	  glVertex2f(1,0);
	  glVertex2f(0.5,1);
	glEnd();

	glutSwapBuffers();  	    	    	    	    	    // must be last function called, this shows 
	                                                            // what was drawn
 }
 
 
void keyboard(unsigned char key, int x, int y )
{

    switch( key)
    {
    	case 27: // ESCAPE
	    exit(1);
	    break;
	    
	case 119:  // W
	    gtz += 0.2 ;
	    break;
	case 97:  // A
	    gtx -= 0.2 ;
	    break;
	case 115: // S
	    gtz -= 0.2 ;
	    break;
	case 100: // D
	    gtx += 0.2;
	    break;
	
	default:
	    break;
    }

    fprintf(stderr, "key press is %d\n", key);
    
    angle_rot += 10.0;

    
    glutPostRedisplay();    	    	    	    	    	    // To tell event loop to redisplay the screen
}
