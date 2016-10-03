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

gl_planets : gl_planets.c
	g++ -o gl_planets gl_planets.c  $(INCLUDES) $(MOTIF_LIB) -lGL -lGLU -lglut -lXi -lXmu -lX11

clean:
	rm gl_planets

	
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

GLfloat gtx = 0.0, gtz = -28.0, gty = -13.0 ;  // control translation in x,y,z axis
GLfloat angle_rot=0.0;
double G = 6.67 * powf(10,-11); // 6.672 x 10-11 m^3 / (kg sec^2)
double MASS_earth = 5.96 * powf(10,24) ; // kg (kilograms)
double MASS_sun   = MASS_earth * 343000.0 ;
double SUN_earth_dist = 1.496 * powf(10,11) ; //  1.496 x 10^11 meters
double SCALE = 1.0 / powf(10,11) ;  // to see the planets on the screen


static GLUquadricObj *quadratic = NULL ;

typedef struct _planet
{
    GLfloat x,y,z;  	  // position of planet
    double vx,vy,vz;      // velocity vector
    double radius;
    double mass;          // size of planet (units of kg)
} PLANET, *PLANET_PTR;


void DrawGL() ;
void ResizeGL(int w, int h);
void keyboard(unsigned char key, int x, int y );
void special_keyboard(int key, int x, int y );
static void initPlanets(void);
void updatePlanets(void);
static float rnd( float max);

#define NPLANETS 2
PLANET planets[NPLANETS];

GLfloat sun_direction[] = { 0.0, 2.0, -1.0, 1.0 };
GLfloat sun_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };


 int main(int argc, char** argv) 
 {
    int ith;
    
	glutInit(&argc, argv);      	    	    	    	    // Initializes the GLUT library
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);   // init params for the display, we are
	    	    	    	    	    	    	    	    // specifying RGB display along with double
								    // buffering, so the screen won't flicker when
								    // we redraw it. GLUT_DEPTH gets rid of the
								    // hidden surfaces (the ones taht are being
								    // blocked by others).
								    
	glutInitWindowSize(640, 480);	    	    	    	    // Initial window Size
	
	glutCreateWindow("Test gl_planets");   	    	    	    // Creates and sets the window title
	glutDisplayFunc(DrawGL);
    	glutKeyboardFunc(keyboard); 	    	    	    	    // Handle keyboard events
    	glutSpecialFunc(special_keyboard);                          // Handle special keys F1..F12, LEFT,RIGHT,UP,DOWN,PAGEUP,PAGEDOWN,HOME
//	glutReshapeFunc(ResizeGL);

	glEnable(GL_DEPTH_TEST);    	    	    	    	    // we need to enable the Z buffer here
        glEnable(GL_LIGHTING);		  // Set up ambient light.
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);
        glEnable(GL_LIGHT0);                // Set up sunlight.
        glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);
  
    	glMatrixMode( GL_PROJECTION );	    	    	    	    // perspecitive projection
	glLoadIdentity();
	gluPerspective( 90, 1.0, 0.1, 100 );	    	    	    // field of view: 90 deg, aspect ratio 1,
	    	    	    	    	    	    	    	    // near clipping plane 0.1, and far clipping
								    // plane 100.
	glMatrixMode( GL_MODELVIEW);

    	initPlanets();
	
 
	glutMainLoop();     	    	    	    	    	    // Starts the main event loop
	
	gluDeleteQuadric( quadratic );	    	    	    	    // Delete Quadratic - Free Resources
	
	return 0;
 }

static void initPlanets(void)
{
    int ith;
    float angle = 0.0 ;
    float inc_angle = 360.0 / NPLANETS;
    float x,y,z;
    float radius = SUN_earth_dist ; // radius from center of SUN
    float d2r = 3.14159 / 180.0 ; //  PI radians/ 180 degrees -- to convert degrees to radians
    float vx=0.0,vy=0.0,vz=0.0;

    // convert inc_angle to radians
    inc_angle = inc_angle * d2r ;
    
    for ( ith=0; ith < NPLANETS; ith++ )
    {
	planets[ith].radius = 1 + rnd(3.0) ; // radius of graphic sphere to represent planet (not actual radius of planet)
	planets[ith].mass = MASS_earth;      // actual MASS of planet ( in kg )

	x = cos( angle ) * radius;
	y = 0;
	z = sin( angle ) * radius;

	planets[ith].x = 0.0 + x;
	planets[ith].y = 0.0 + y;
	planets[ith].z = 0.0 + z;
	planets[ith].vx = vx;
	planets[ith].vy = vy;
	planets[ith].vz = vz;

    	angle += inc_angle;
	
    //	    printf("%f,%f,%f\n", planets[ith].x, planets[ith].y, planets[ith].z );
    }

}


void updatePlanets(void)
{
    int ith,nth;
    float dx,dy,dz;
    double ax,ay,az;
    float force;
    float radius_xyz, radius_xz, angle_xz, angle_xy;
    
    for (ith=0; ith < NPLANETS; ith++)
    {
    	for (nth=0; nth<NPLANETS; nth++)
	{
	    if ( ith != nth )
	    {
	    	dx = planets[ith].x - planets[nth].x;
		dy = planets[ith].y - planets[nth].y;
		dz = planets[ith].z - planets[nth].z;
		
	    	radius_xyz = dx*dx + dy*dy + dz*dz ; // distance apart ^2
		
		//
		// F = G Mm / r^2
		//
	    	force = G * planets[ith].mass * planets[nth].mass  / radius_xyz ;
		
		//
		// determine angle in xz plane
		//
		angle_xz = atan2f( dz, dx);
		angle_xy = atan2f( dy, dx);

		ax = cos(angle_xz) * force;
		ay = 0;
		az = sin(angle_xz) * force;

printf("debug: ax,ay,az: %e,%e,%e\n", ax, ay, az);
		
		planets[ith].vx += ax;
		planets[ith].vy += ay;
		planets[ith].vz += az;

	    }
	}
    }
    

    for (ith=0; ith < NPLANETS; ith++ )
    {
	planets[ith].x += planets[ith].vx;
	planets[ith].y += planets[ith].vy;
	planets[ith].z += planets[ith].vz;
    }

}


static float rnd( float max)
{
    float num = (1.0 * random()) / RAND_MAX;
    
//    printf( "rnd( %f ) => %f\n", max, num );
    
    num = num * max;
    
    return num;
    
}

// This is where you draw your stuff to the screen
 // (shouldn't be doing heavy computations in here though)
 void DrawGL() 
 {
    int ith;
    float x,y,z;
 
    	glClearColor(0, 0, 0, 0 );  	    	    	    	    // this sets teh color to fill the background
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);   	    // this fills the background with bg color
	    	    	    	    	    	    	    	    // GL_COLOR_BUFFER_BIT - clear just the image
								    // GL_DEPTH_BUFFER_BIT - clears Z-buffer too
fprintf(stderr, "DrawGl called\n");

	glLoadIdentity();   	    	    	    	    	    // clear modelview
	

    	glTranslatef( gtx, gty, gtz);  	    	    	    	    // ... move forwards and down a bit (x,y,z)
	glRotatef(angle_rot, 0, 1, 0);	    	    	    	    // rotate a little in the Y axis

//    	glutSolidTeapot( 1.0 );   // glut routine to draw a solid 3d teapot

 
    	// Drawing the planets
	
	if ( NULL == quadratic )
	{
	    quadratic=gluNewQuadric();
	    gluQuadricNormals(quadratic, GLU_SMOOTH);  // Create Smooth Normals
	    gluQuadricTexture(quadratic, GL_TRUE);     // Create texture coords
    	}

    	for (ith=0; ith < NPLANETS; ith++)
	{
	    glPushMatrix();
	    x = planets[ith].x * SCALE;
	    y = planets[ith].y * SCALE;
	    z = planets[ith].z * SCALE;
	    
	    glTranslatef( x, y, z);
    	    gluSphere( quadratic, planets[ith].radius , 32, 32 ); // Radius,subdivisions_around_z_axis,subdvisions_along_z_axis
    	    glPopMatrix();
    	}

	glutSwapBuffers();  	    	    	    	    	    // must be last function called, this shows 
	                                                            // what was drawn


    	updatePlanets();
	
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
    	case GLUT_KEY_DOWN:
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
	case 122: // Z
	    gty += 0.2 ;
	    break;
	case 120: // X
	    gty -= 0.2 ;
	    break;
	
	default:
	    break;
    }

    fprintf(stderr, "keyboard key is %d\n", key);
    fprintf(stderr, "gt* is %f,%f,%f\n", gtx, gty, gtz );
    
 
    
    glutPostRedisplay();    	    	    	    	    	    // To tell event loop to redisplay the screen
}
