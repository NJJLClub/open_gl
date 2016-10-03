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



typedef struct _element
{
    struct _element *l_next;
    void *userDataP;
} ELEMENT, *ELEMENT_PTR;


/*
** A room has a location and width,length,height attributes
*/
typedef struct _room
{
    ELEMENT element;	    	    // is a type of element
    GLfloat x,y,z;
    GLfloat length,width,height;
} ROOM, *ROOM_PTR;


/*
** our world contains a list of rooms
*/
typedef struct _world
{
    ROOM_PTR roomsHeadP,roomsTailP;
} WORLD, *WORLD_PTR;


/*
** GLOBALS
*/

GLfloat gtx = 0.0, gtz = -8.0, gty = -3.0 ;  // control translation in x,y,z axis
GLfloat angle_rot=0.0;
WORLD_PTR G_worldP;


void DrawGL() ;
void ResizeGL(int w, int h);
void keyboard(unsigned char key, int x, int y );
void special_keyboard(int key, int x, int y );
void randomWorld(void);


ROOM_PTR NewRoom(WORLD_PTR worldP, GLfloat x,GLfloat y,GLfloat z,GLfloat length,GLfloat width,GLfloat height)
{
    ROOM_PTR roomP = (ROOM_PTR)malloc(sizeof(ROOM));
    roomP->element.l_next = NULL;
    roomP->x = x;
    roomP->y = y;
    roomP->z = z;
    roomP->length = length;
    roomP->width = width;
    roomP->height = height;
    
    if ( worldP->roomsTailP )
    {
    	worldP->roomsTailP->element.l_next = (ELEMENT_PTR)roomP;
	worldP->roomsTailP = roomP;
    }
    else
    {
    	worldP->roomsHeadP = roomP;
	worldP->roomsTailP = roomP;
    }
    
    return roomP;
}

int main(int argc, char** argv) 
 {

      G_worldP = (WORLD_PTR)malloc(sizeof(WORLD));
      
      G_worldP->roomsHeadP = (ROOM_PTR)NULL;
      
      G_worldP->roomsTailP = (ROOM_PTR)NULL;


      randomWorld();
	
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
    	glutSpecialFunc(special_keyboard);                          // Handle special keys F1..F12, LEFT,RIGHT,UP,DOWN,PAGEUP,PAGEDOWN,HOME
//	glutReshapeFunc(ResizeGL);

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



#define X 0
#define Y 1
#define Z 2

void drawRoom(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat length , GLfloat width, GLfloat height )
{
    /*
    ** room template defined here
    */
    GLfloat front[4][3] = { {-1, 0, 1}, {-1, 2, 1}, { 1, 2, 1},  { 1, 0, 1} };
    GLfloat  back[4][3] = { {-1, 0,-1}, { 1, 0,-1}, { 1, 2,-1},  {-1, 2,-1} };
    GLfloat lside[4][3] = { {-1, 0, 1}, {-1, 2, 1}, {-1, 2,-1},  {-1, 0,-1} };
    GLfloat rside[4][3] = { { 1, 0, 1}, { 1, 0,-1}, { 1, 2,-1},  { 1, 2, 1} };
    int i,n;
    
    for( i=0; i < 4; i++)
    {
    	    front[i][Z] *= length;
    	     back[i][Z] *= length;
    	    lside[i][Z] *= length;
    	    rside[i][Z] *= length;

    	    front[i][X] *= width;
    	     back[i][X] *= width;
    	    lside[i][X] *= width;
    	    rside[i][X] *= width;

    	    front[i][Y] *= height;
    	     back[i][Y] *= height;
    	    lside[i][Y] *= height;
    	    rside[i][Y] *= height;

	    
	    front[i][X] += tx;
	    front[i][Y] += ty;
	    front[i][Z] += tz;

	    back[i][X] += tx;
	    back[i][Y] += ty;
	    back[i][Z] += tz;

	    lside[i][X] += tx;
	    lside[i][Y] += ty;
	    lside[i][Z] += tz;

	    rside[i][X] += tx;
	    rside[i][Y] += ty;
	    rside[i][Z] += tz;

    }
     
    // Front
	glColor3f( 0, 1, 0);	    	    	    	    // set color to pure green
    	for( i=0; i < 4; i++)
	{
	    glVertex3f( front[i][X], front[i][Y], front[i][Z] );
	}


    // Back
	glColor3f( 0, 0.8, 0 );		    	    	    // a slightly darker green for the back
    	for( i=0; i < 4; i++)
	{
	    glVertex3f( back[i][X], back[i][Y], back[i][Z] );
	}


    // Left side
	glColor3f( 0, 0.6, 0 );		    	    	    // an even darker shade for the sides
    	for( i=0; i < 4; i++)
	{
	    glVertex3f( lside[i][X], lside[i][Y], lside[i][Z] );
	    
	}

    // Right side
	glColor3f( 0, 0.4, 0 );		    	    	    // an even darker shade for the sides
    	for( i=0; i < 4; i++)
	{
	    glVertex3f( rside[i][X], rside[i][Y], rside[i][Z] );
	}

}

// test to see many rectangles of varying sizes
void randomWorld(void)
{
    int i;
    float sign=1;
    float length,width,height;
    float x,y,z;
    
    length=1;
    width=1;
    height=1;
    
    x=0;
    y=0;
    z=0;
    
    for( i=0 ; i < 10; i++)
    {
    	sign = -1 * sign;
	
/*	(void) NewRoom( G_worldP, 
	    sign * 10 * ((1.0 * random() ) / RAND_MAX), 
	    0,					        
	    sign * 10 * ((1.0 * random() ) / RAND_MAX), 
	    4.0 * random() / RAND_MAX,		        
	    1,					       
	    1                   		        
	); */

	(void) NewRoom( G_worldP, 
	    x,
	    y,					        
	    z, 
	    length,		        
	    width,					       
	    height  );
	    
	length += 1.0;
	x += 2.0 + width;
	z += 2.0 ;


    }


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

//    	glutSolidTeapot( 1.0 );   // glut routine to draw a solid 3d teapot

 
    	// Drawing the cube
	glBegin(GL_QUADS);  	    	    	    	    	    // using quadrilaterals as primitive


    	ROOM_PTR roomP = G_worldP->roomsHeadP;
	while ( roomP )
	{
	    drawRoom(  roomP->x, roomP->y, roomP->z, roomP->length, roomP->width, roomP->length );
	    roomP = (ROOM_PTR) roomP->element.l_next;
	}
	
	
	glEnd();    	    	    	    	    	    	    // you need to call this after the last vertex



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
	glOrtho(-1,1,-1,1,-1,1); // use an orthographic projection
	glMatrixMode(GL_MODELVIEW); // go back to modelview matrix
 }

void special_keyboard( int key, int x, int y )
{
    switch( key)
    {
    	case GLUT_KEY_RIGHT:
    	    angle_rot += 10.0;
	    break;
    	case GLUT_KEY_LEFT:
    	    angle_rot -= 10.0;
	    break;
	    
    	case GLUT_KEY_UP:
	    gty += 0.2;
	    break;
	    
    	case GLUT_KEY_DOWN:
	    gty -= 0.2;
	    break;
	    
	case GLUT_KEY_F1:
	case GLUT_KEY_F2:
	case GLUT_KEY_F3:
	case GLUT_KEY_F4:
	case GLUT_KEY_F5:
	case GLUT_KEY_F6:
	case GLUT_KEY_F7:
	case GLUT_KEY_F8:
	case GLUT_KEY_F9:
	case GLUT_KEY_F10:
	case GLUT_KEY_F11:
	case GLUT_KEY_F12:
	case GLUT_KEY_PAGE_UP:
	case GLUT_KEY_PAGE_DOWN:
	case GLUT_KEY_HOME:
	case GLUT_KEY_END:
	case GLUT_KEY_INSERT:
	
	default:
	    break;
    }
    
    fprintf(stderr, "special key %d\n", key);
    
    glutPostRedisplay();	    	    // To tell event loop to redisplay the screen
    
}

void dumpWorld(void)
{

    ROOM_PTR roomP = G_worldP->roomsHeadP;
    while( roomP )
    {
    	printf("Room: %f,%f,%f   l=%f  w=%f  h=%f\n",
	    	roomP->x,
	    	roomP->y,
	    	roomP->z,
	    	roomP->length,
	    	roomP->width,
	    	roomP->height);
		
    	roomP = (ROOM_PTR)roomP->element.l_next;
    }
}

void keyboard(unsigned char key, int x, int y )
{

    switch( key)
    {
    	case 27: // ESCAPE
    	    dumpWorld();
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

    fprintf(stderr, "keyboard key is %d\n", key);
    
 
    
    glutPostRedisplay();    	    	    	    	    	    // To tell event loop to redisplay the screen
}
