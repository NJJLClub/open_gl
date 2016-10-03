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

gl_draw : gl_draw.c
	g++ -o gl_draw gl_draw.c  $(INCLUDES) $(MOTIF_LIB) -lGL -lGLU -lglut -lXi -lXmu -lX11

clean:
	rm gl_draw

	
*/


#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

GLfloat gtx = 0.0, gtz = -8.0, gty = -3.0 ;  // control translation in x,y,z axis
GLfloat angle_rot=0.0;


void DrawGL() ;
void ResizeGL(int w, int h);
void keyboard(unsigned char key, int x, int y );


 int main(int argc, char** argv) 
 {
	glutInit(&argc, argv);      	    	    	    	    // Initializes the GLUT library
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);   // init params for the display, we are
	    	    	    	    	    	    	    	    // specifying RGB display along with double
								    // buffering, so the screen won't flicker when
								    // we redraw it. GLUT_DEPTH gets rid of the
								    // hidden surfaces (the ones taht are being
								    // blocked by others).
								    
	glutInitWindowSize(640, 480);	    	    	    	    // Initial window Size
	
	glutCreateWindow("Test gl_draw");   	    	    	    // Creates and sets the window title
	glutDisplayFunc(DrawGL);
    	glutKeyboardFunc(keyboard); 	    	    	    	    // Handle keyboard events
	glutReshapeFunc(ResizeGL);

	glEnable(GL_DEPTH_TEST);    	    	    	    	    // we need to enable the Z buffer here

    	glMatrixMode( GL_PROJECTION );	    	    	    	    // perspecitive projection
	glLoadIdentity();
	gluPerspective( 90, 1.0, 0.1, 100 );	    	    	    // field of view: 90 deg, aspect ratio 1,
	    	    	    	    	    	    	    	    // near clipping plane 0.1, and far clipping
								    // plane 100.
	glMatrixMode( GL_MODELVIEW);


	glutMainLoop();     	    	    	    	    	    // Starts the main event loop
	return 0;
 }


// This is where you draw your stuff to the screen
 // (shouldn't be doing heavy computations in here though)
 void DrawGL() 
 {
    	glClearColor(0, 0, 0, 0 );  	    	    	    	    // this sets teh color to fill the background
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);   	    // this fills the background with bg color
	    	    	    	    	    	    	    	    // GL_COLOR_BUFFER_BIT - clear just the image
								    // GL_DEPTH_BUFFER_BIT - clears Z-buffer too
fprintf(stderr, "DrawGl called\n");

	glLoadIdentity();   	    	    	    	    	    // clear modelview
	

    	glTranslatef( gtx, gty, gtz);  	    	    	    	    // ... move forwards and down a bit (x,y,z)
	glRotatef(angle_rot, 0, 1, 0);	    	    	    	    // rotate a little in the Y axis

    	// Drawing the cube
	glBegin(GL_QUADS);  	    	    	    	    	    // using quadrilaterals as primitive
	// Front
	    glColor3f( 0, 1, 0);	    	    	    	    // set color to pure green
	    glVertex3f( -1,  0,  1 );	    	    	    	    // 3f means three floating point values
	    glVertex3f( -1,  2,  1 );
	    glVertex3f(  1,  2,  1 );
	    glVertex3f(  1,  0,  1 );

	// Back
	    glColor3f( 0, 0.8, 0 );		    	    	    // a slightly darker green for the back
	    glVertex3f( -1,  0, -1 );
	    glVertex3f(  1,  0, -1 );
	    glVertex3f(  1,  2, -1 );
	    glVertex3f( -1,  2, -1 );

	// Left side
	    glColor3f( 0, 0.6, 0 );		    	    	    // an even darker shade for the sides
	    glVertex3f( -1,  0,  1 );
	    glVertex3f( -1,  2,  1 );
	    glVertex3f( -1,  2, -1 );
	    glVertex3f( -1,  0, -1 );

	// Right side
	    glVertex3f(  1,  0,  1 );
	    glVertex3f(  1,  0, -1 );
	    glVertex3f(  1,  2, -1 );
	    glVertex3f(  1,  2,  1 );
	glEnd();    	    	    	    	    	    	    // you need to call this after the last vertex


    	// Now draw the roof
	glBegin(GL_TRIANGLES);
	// Front
	  glColor3f( 1, 1, 0 );		// yellow
	  glVertex3f( -1,  2,  1 );
	  glVertex3f(  0,  4,  0 );
	  glVertex3f(  1,  2,  1 );
	// Right side
	  glColor3f( 0.9, 0.9, 0 );	// darker yellow
	  glVertex3f(  1,  2,  1 );
	  glVertex3f(  1,  2, -1 );
	  glVertex3f(  0,  4,  0 );
	// Back
	  glColor3f( 0.8, 0.8, 0 );	// even darker yellow
	  glVertex3f(  1,  2, -1 );
	  glVertex3f( -1,  2, -1 );
	  glVertex3f(  0,  4,  0 );
	// Left side
	  glColor3f( 0.7, 0.7, 0 );	// a bit darker, but still yellow
	  glVertex3f( -1,  2,  1 );
	  glVertex3f(  0,  4,  0 );
	  glVertex3f( -1,  2, -1 );
	glEnd();



	glutSwapBuffers();  	    	    	    	    	    // must be last function called, this shows 
	                                                            // what was drawn
 }
 
 // This is where you define your viewing projection
 // Called by the window manager whenever the window is resized
 void ResizeGL(int w, int h)
 {
	glViewport(0,0,w,h); // set the drawable region of the window
	glMatrixMode(GL_PROJECTION); // set up the projection matrix
	glLoadIdentity(); // clear any previous transform and set to the identity matrix
//	glOrtho(-1,1,-1,1,-1,1); // use an orthographic projection
	glMatrixMode(GL_MODELVIEW); // go back to modelview matrix
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
