#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"

#include "osusphere.cpp"
#include "loadobjfile.cpp"





// title of these windows:

const char *WINDOWTITLE = { "Our Solar System -- Conner Patzman" };
const char *GLUITITLE   = { "User Interface Window" };


// milliseconds per time cycle:

const int MS_PER_CYCLE = { 1000 };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// initial window size:
/// aspect ratio
const int INIT_WINDOW_SIZE = { 600 };
const int INIT_WINDOW_SIZEx = { 1280 };
const int INIT_WINDOW_SIZEy = { 720 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };

// scroll wheel button values:

const int SCROLL_WHEEL_UP = 3;
const int SCROLL_WHEEL_DOWN = 4;

// equivalent mouse movement when we click the scroll wheel:

const float SCROLL_WHEEL_CLICK_FACTOR = 5.f;

// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
	RESET,
	QUIT
};


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };

// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// non-constant global variables:
unsigned char* TextureArray0, * TextureArray1, * TextureArray2, * TextureArray3, * TextureArray4, * TextureArray5,
* TextureArray6, * TextureArray7, * TextureArray8, * TextureArray9, * TextureArray10; // global variables
GLuint Tex0, Tex2, Tex3, Tex4, Tex5, Tex6, Tex7, Tex8, Tex9, Tex1, Tex10; // global variables

unsigned char* Texture;	// the texels
unsigned int    WorldTex;	// the texture object

float  RAD = 7.5;
float mercuryOrbRad = RAD * 0.5;
float venusOrbRad = (RAD * 0.67);
float earthOrbRad = (RAD * 0.9);
float marsOrbRad = (RAD * 1.2);
float jupiterOrbRad = (RAD * 2.2);
float saturnOrbRad = (RAD * 3.3);
float uranusOrbRad = (RAD * 4.2);
float neptuneOrbRad = (RAD * 5.);
GLuint SphereList;
GLuint SunList;
GLuint MercuryList;
GLuint MercuryOrbitList;
GLuint VenusList;
GLuint VenusOrbitList;
GLuint EarthList;
GLuint EarthOrbitList;
GLuint MarsList;
GLuint MarsOrbitList;
GLuint JupiterList;
GLuint JupiterOrbitList;
GLuint SaturnList;
GLuint ringList;
GLuint SaturnOrbitList;
GLuint UranusList;
GLuint UranusOrbitList;
GLuint NeptuneList;
GLuint NeptuneOrbitList;
GLuint DL;
int width10;
int height10;
int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
bool	Freeze;
bool	Light0On;
bool	Light1On;
bool	Light2On;
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
float	Time;				// fraction (0.-1.) of the time cycle
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees

float White[3] = { 1., 1., 1. };

// function prototypes:

void	Animate( );
float	GetPeriod();
void	Display( );
void	DoAxesMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	Axes( float );
void	HsvRgb( float[3], float [3] );

void	SetMaterial( float, float, float,  float );
void	SetPointLight( int, float, float, float,  float, float, float );
void	SetSpotLight( int, float, float, float,  float, float, float, float, float, float );

float * Array3( float, float, float );
float * Array4( float, float, float, float );
float * BlendArray3( float, float [3], float [3] );
float * MulArray3( float, float [3] );
unsigned char* BmpToTexture(char* filename, int* width, int* height);

// main program:

int
main( int argc, char *argv[ ] )
{
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit( &argc, argv );


	// setup all the graphics stuff:

	InitGraphics( );

	
	// create the display structures that will not change:

	InitLists( );



	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );


	// setup all the user interface stuff:

	InitMenus( );


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );


	// this is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

float GetPeriod(float aRadius) {
	int ms = Time;
	int the_cycle = MS_PER_CYCLE * pow(aRadius, 3. / 2.);
	ms %= the_cycle;
	float theTime = (float)ms / (float)(the_cycle - 1);
	return theTime;
}

void
Animate( )
{
	int ms = glutGet(GLUT_ELAPSED_TIME);
	Time = ms;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
	
	//int ms = glutGet( GLUT_ELAPSED_TIME );
	//ms %= MS_PER_CYCLE;
	//Time = (float)ms / (float)( MS_PER_CYCLE - 1 );
	//glutSetWindow( MainWindow );
	//glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if( DebugOn != 0 )
	{
		fprintf( stderr, "Display\n" );
	}

	// set which window we want to do the graphics into:

	glutSetWindow( MainWindow );

	// erase the background:

	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );

	// specify shading:

	glShadeModel( GL_SMOOTH );

	// set the viewport to a square centered in the window:   INIT_WINDOW_SIZEx

	GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);

	//GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	//GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	
	/// <summary>
	/// get rid of lines, and change xl = 0, yb=0,  glViewport(0,0,vx,vy)
	///aspect
	/// </summary>
	//GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	//GLint xl = ( vx - v ) / 2;
	//GLint yb = ( vy - v ) / 2;
	//glViewport( xl, yb,  v, v );
	glViewport(0, 0, vx, vy);
	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
	else
		gluPerspective( 90., 1.777,	0.1, 1000. );

	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// set the eye position, look-at position, and up-vector:

	gluLookAt( 0., 0., 30.,     0., 0., 0.,     0., 1., 0. );

	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0., 1., 0. );
	glRotatef( (GLfloat)Xrot, 1., 0., 0. );

	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );

	glDisable( GL_FOG );

	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glColor3f( 1., 1., 1. );
		glCallList( AxesList );
	}

	// since we are using glScalef( ), be sure normals get unitized:

	glEnable( GL_NORMALIZE );

	// draw the objects:

	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, MulArray3( .3f, White ) );
	glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
	glEnable( GL_LIGHTING );



	///Orbits
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glCallList(MercuryOrbitList);
	glCallList(VenusOrbitList);
	glCallList(EarthOrbitList);
	glCallList(MarsOrbitList);
	glCallList(JupiterOrbitList);
	glCallList(SaturnOrbitList);
	glCallList(UranusOrbitList);
	glCallList(NeptuneOrbitList);


	glEnable(GL_LIGHTING);

	glPopMatrix();





	float theta = (float)(2. * M_PI) * Time;
	
	



	/// sun

	glPushMatrix( );
	
	SetPointLight(GL_LIGHT2, 0, 0., 0., 1., 1., 1.);
	glEnable(GL_LIGHT2);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
		SetMaterial( 0.1f, 1.f, 0.1f,  16.f );
		glTranslatef( 0., 0., 0. );
		glShadeModel( GL_SMOOTH );
		SetMaterial(1., 1., 1., 15.);
		glCallList(SunList);

		glDisable(GL_TEXTURE_2D);
	glPopMatrix( );
	
	
	

	//star backround
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex9);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	glShadeModel(GL_SMOOTH);
	SetMaterial(1., 1., 1., 15.);
	glScalef(200., 200., 200.);
	glCallList(SphereList);

	glPopMatrix();


	glEnable(GL_LIGHTING);

	

	/// mercury
	float mercTime = GetPeriod(mercuryOrbRad);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SetMaterial(0.1f, 1.f, 0.1f, 16.f);
	glRotatef(mercTime * 360, 0, 1, 0);
	glTranslatef((RAD * 0.5), 0., 0.);
	
	glShadeModel(GL_SMOOTH);
	SetMaterial(1., 1., 1., 15.);
	glCallList(MercuryList);

	glPopMatrix();
	

	///Venus
	float venusTime = GetPeriod(venusOrbRad);
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, Tex2);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SetMaterial(0.1f, 1.f, 0.1f, 16.f);
	glRotatef(venusTime * 360, 0, 1, 0);
	glTranslatef(venusOrbRad, 0., 0.);
	glShadeModel(GL_SMOOTH);
	SetMaterial(1., 1., 1., 15.);
	glCallList(VenusList);
	glPopMatrix();

	//Earth
	float earthTime = GetPeriod(earthOrbRad);
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, Tex3);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SetMaterial(0.1f, 1.f, 0.1f, 16.f);
	glRotatef(earthTime * 360, 0, 1, 0);
	glTranslatef((earthOrbRad ), 0., 0.);

	glShadeModel(GL_SMOOTH);
	SetMaterial(1., 1., 1., 15.);
	
	glCallList(EarthList);
	glPopMatrix();

	//Mars
	float marsTime = GetPeriod(marsOrbRad);
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, Tex4);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SetMaterial(0.1f, 1.f, 0.1f, 16.f);
	glRotatef(marsTime * 360, 0, 1, 0);
	glTranslatef((marsOrbRad), 0., 0.);

	glShadeModel(GL_SMOOTH);
	SetMaterial(1., 1., 1., 15.);
	glCallList(MarsList);
	glPopMatrix();

	//Jupiter
	float jupiterTime = GetPeriod(jupiterOrbRad);
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, Tex5);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SetMaterial(0.1f, 1.f, 0.1f, 16.f);
	glRotatef(jupiterTime * 360, 0, 1, 0);
	glTranslatef(jupiterOrbRad, 0., 0.);

	glShadeModel(GL_SMOOTH);
	SetMaterial(1., 1., 1., 15.);
	glCallList(JupiterList);
	glPopMatrix();
	
	//Saturn
	float saturnTime = GetPeriod(saturnOrbRad);
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, Tex6);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SetMaterial(0.1f, 1.f, 0.1f, 16.f);
	glRotatef(saturnTime * 360, 0, 1, 0);
	glTranslatef((saturnOrbRad), 0.3, 0.);
	glRotatef(30, 0, 0, 1);

	glShadeModel(GL_SMOOTH);
	SetMaterial(1., 1., 1., 15.);
	glCallList(SaturnList);
	glCallList(ringList);
	
	glPopMatrix();


	

	//Uranus
	float uranusTime = GetPeriod(uranusOrbRad);
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, Tex7);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SetMaterial(0.1f, 1.f, 0.1f, 16.f);
	glRotatef(uranusTime * 360, 0, 1, 0);
	glTranslatef(uranusOrbRad, 0., 0.);
	

	glShadeModel(GL_SMOOTH);
	SetMaterial(1., 1., 1., 15.);
	glCallList(UranusList);
	glPopMatrix();
	
	//Neptune
	float neptuneTime = GetPeriod(neptuneOrbRad);
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, Tex8);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SetMaterial(0.1f, 1.f, 0.1f, 16.f);
	//glTranslatef((float)((2*RAD + 30)* cos(theta)), (float)((2*RAD + 30)* sin(theta)), 0.);
	glRotatef(neptuneTime * 360, 0, 1, 0);
	glTranslatef(neptuneOrbRad, 0., 0.);
	
	glShadeModel(GL_SMOOTH);
	SetMaterial(1., 1., 1., 15.);
	glCallList(NeptuneList);
	glPopMatrix();
	

	

	glDisable( GL_LIGHTING );

	

	glDisable(GL_TEXTURE_2D);

	// swap the double-buffered framebuffers:

	glutSwapBuffers( );
	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	glutSetWindow( MainWindow );

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	//glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics( )
{
	
	
	
	
	
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:
	/// aspect ratio
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZEx, INIT_WINDOW_SIZEy );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );
	glutIdleFunc( Animate );

	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	int width0, height0, width1, height1, width2, height2, width3, height3, width4, height4,w10,h10;
	int width5, height5, width6, height6, width7, height7, width8, height8, width9, height9;
	unsigned char* TextureArray0 = BmpToTexture("2k_sun.bmp", &width0, &height0);
	unsigned char* TextureArray1 = BmpToTexture("2k_mercury.bmp", &width1, &height1);
	unsigned char* TextureArray2 = BmpToTexture("2k_venus_surface.bmp", &width2, &height2);
	unsigned char* TextureArray3 = BmpToTexture("worldtex.bmp", &width3, &height3);
	unsigned char* TextureArray4 = BmpToTexture("2k_mars.bmp", &width4, &height4);
	unsigned char* TextureArray5 = BmpToTexture("2k_jupiter.bmp", &width5, &height5);
	unsigned char* TextureArray6 = BmpToTexture("2k_saturn.bmp", &width6, &height6);
	unsigned char* TextureArray7 = BmpToTexture("2k_uranus.bmp", &width7, &height7);
	unsigned char* TextureArray8 = BmpToTexture("2k_neptune.bmp", &width8, &height8);
	unsigned char* TextureArray9 = BmpToTexture("8k_stars_milky_way.bmp", &width9, &height9);
	unsigned char* TextureArray10 = BmpToTexture("2k_saturn_ring_alpha.bmp", &w10, &h10);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &Tex0); // assign binding “handles”
	glGenTextures(1, &Tex1);
	glGenTextures(1, &Tex2);
	glGenTextures(1, &Tex3);
	glGenTextures(1, &Tex4);
	glGenTextures(1, &Tex5);
	glGenTextures(1, &Tex6);
	glGenTextures(1, &Tex7);
	glGenTextures(1, &Tex8);
	glGenTextures(1, &Tex9);
	glGenTextures(1, &Tex10);


	//now bind em...
	glBindTexture(GL_TEXTURE_2D, Tex0); // make the Tex0 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width0, height0, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray0);

	glBindTexture(GL_TEXTURE_2D, Tex1); // make the Tex1 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray1);

	glBindTexture(GL_TEXTURE_2D, Tex2); // make the Tex1 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray2);

	glBindTexture(GL_TEXTURE_2D, Tex3); // make the Tex1 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray3);

	glBindTexture(GL_TEXTURE_2D, Tex4); // make the Tex1 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width4, height4, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray4);

	glBindTexture(GL_TEXTURE_2D, Tex5); // make the Tex1 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width5, height5, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray5);

	glBindTexture(GL_TEXTURE_2D, Tex6); // make the Tex1 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width6, height6, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray6);

	glBindTexture(GL_TEXTURE_2D, Tex7); // make the Tex1 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width7, height7, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray7);

	glBindTexture(GL_TEXTURE_2D, Tex8); // make the Tex1 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width8, height8, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray8);


	glBindTexture(GL_TEXTURE_2D, Tex9); // make the Tex1 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width9, height9, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray9);

	glBindTexture(GL_TEXTURE_2D, Tex10); // make the Tex1 texture current and set its parametersglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w10, h10, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray10);


	int width, height;
	Texture = BmpToTexture((char*)"worldtex.bmp", &width, &height);
	if (Texture == NULL)
		fprintf(stderr, "Cannot open texture '%s'\n", "worldtex.bmp");
	else
		fprintf(stderr, "Width = %d ; Height = %d\n", width, height);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &WorldTex);
	glBindTexture(GL_TEXTURE_2D, WorldTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);
}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;
	glutSetWindow( MainWindow );

	SphereList = glGenLists(1);
	glNewList(SphereList, GL_COMPILE);
	OsuSphere(1, 30, 30);
	glEndList();

	SunList = glGenLists(1);
	glNewList(SunList, GL_COMPILE);
	OsuSphere(3, 30, 30);
	glEndList();

	MercuryList = glGenLists(1);
	glNewList(MercuryList, GL_COMPILE);
	OsuSphere(0.19, 30, 30);
	glEndList();


	// make orbit lists
	MercuryOrbitList = glGenLists(1);
	glNewList(MercuryOrbitList, GL_COMPILE);
	glColor4f(1., 1., 1., 0.);
	float dang = 2. * M_PI / 149.;
	float ang = 0.;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 150; i++)
	{
		glVertex3f((mercuryOrbRad) * cos(ang), 0., (mercuryOrbRad) * sin(ang));
		ang += dang;
	}
	glEnd();
	glEndList();

	VenusOrbitList = glGenLists(1);
	glNewList(VenusOrbitList, GL_COMPILE);

	dang = 2. * M_PI / 149.;
	ang = 0.;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 150; i++)
	{
		glVertex3f((venusOrbRad)*cos(ang), 0., (venusOrbRad)*sin(ang));
		ang += dang;
	}
	glEnd();
	glEndList();

	EarthOrbitList = glGenLists(1);
	glNewList(EarthOrbitList, GL_COMPILE);
	
	dang = 2. * M_PI / 149.;
	ang = 0.;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 150; i++)
	{
		glVertex3f((earthOrbRad)*cos(ang), 0., (earthOrbRad)*sin(ang));
		ang += dang;
	}
	glEnd();
	glEndList();

	MarsOrbitList = glGenLists(1);
	glNewList(MarsOrbitList, GL_COMPILE);
	
	dang = 2. * M_PI / 149.;
	ang = 0.;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 150; i++)
	{
		glVertex3f((marsOrbRad)*cos(ang), 0., (marsOrbRad)*sin(ang));
		ang += dang;
	}
	glEnd();
	glEndList();

	JupiterOrbitList = glGenLists(1);
	glNewList(JupiterOrbitList, GL_COMPILE);
	
	dang = 2. * M_PI / 149.;
	ang = 0.;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 150; i++)
	{
		glVertex3f((jupiterOrbRad)*cos(ang), 0., (jupiterOrbRad)*sin(ang));
		ang += dang;
	}
	glEnd();
	glEndList();

	SaturnOrbitList = glGenLists(1);
	glNewList(SaturnOrbitList, GL_COMPILE);
	
	dang = 2. * M_PI / 149.;
	ang = 0.;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 150; i++)
	{
		glVertex3f((saturnOrbRad)*cos(ang), 0., (saturnOrbRad)*sin(ang));
		ang += dang;
	}
	glEnd();
	glEndList();

	UranusOrbitList = glGenLists(1);
	glNewList(UranusOrbitList, GL_COMPILE);
	
	dang = 2. * M_PI / 149.;
	ang = 0.;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 150; i++)
	{
		glVertex3f((uranusOrbRad)*cos(ang), 0., (uranusOrbRad)*sin(ang));
		ang += dang;
	}
	glEnd();
	glEndList();

	NeptuneOrbitList = glGenLists(1);
	glNewList(NeptuneOrbitList, GL_COMPILE);
	
	dang = 2. * M_PI / 149.;
	ang = 0.;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 150; i++)
	{
		glVertex3f((neptuneOrbRad)*cos(ang), 0., (neptuneOrbRad)*sin(ang));
		ang += dang;
	}
	glEnd();
	glEndList();

	
	/// Planet Lists now

	VenusList = glGenLists(1);
	glNewList(VenusList, GL_COMPILE);
	OsuSphere(0.474, 30, 30);
	glEndList();

	EarthList = glGenLists(1);
	glNewList(EarthList, GL_COMPILE);
	OsuSphere(0.5, 30, 30);
	glEndList();

	MarsList = glGenLists(1);
	glNewList(MarsList, GL_COMPILE);
	OsuSphere(0.266, 30, 30);
	glEndList();

	JupiterList = glGenLists(1);
	glNewList(JupiterList, GL_COMPILE);
	OsuSphere(3., 30, 30);
	glEndList();

	SaturnList = glGenLists(1);
	glNewList(SaturnList, GL_COMPILE);
	OsuSphere(2.32, 30, 30);
	glEndList();

	UranusList = glGenLists(1);
	glNewList(UranusList, GL_COMPILE);
	OsuSphere(1.4, 30, 30);
	glEndList();

	NeptuneList = glGenLists(1);
	glNewList(NeptuneList, GL_COMPILE);
	OsuSphere(1.24, 30, 30);
	glEndList();



	ringList = glGenLists(1);
	glNewList(ringList, GL_COMPILE);
	dang = 2. * M_PI / 149.;
	ang = 0.;
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 150; i++)
	{
		float s = i / 150;
		glTexCoord2f(s, 1.);
		glNormal3f(3. * cos(ang), 0., 3. * sin(ang));
		glVertex3f(5. * cos(ang), 0., 5. * sin(ang));
		
		glTexCoord2f(s, 0.);
		glNormal3f(5. * cos(ang), 0., 5. * sin(ang));
		glVertex3f(3.*cos(ang), 0., 3.*sin(ang));
		ang += dang;
	}
	glEnd();
	glEndList();
	//DL = glGenLists(1);
	//glNewList(DL, GL_COMPILE);
	//LoadObjFile("star wars star destroyer.obj");
//	glEndList();

	// create the axes:

	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case '0':
			Light0On = ! Light0On;
			break;

		case '1':
			Light1On = ! Light1On;
			break;

		case '2':
			Light2On = ! Light2On;
			break;

		case 'f':
		case 'F':
			Freeze = ! Freeze;
			if( Freeze )
				glutIdleFunc( NULL );
			else
				glutIdleFunc( Animate );
			break;

		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			WhichProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		case SCROLL_WHEEL_UP:
			Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		case SCROLL_WHEEL_DOWN:
			Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}


	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthCueOn = 0;
	Freeze = false;
	Light0On = true;
	Light1On = true;
	Light2On = true;
	Time = 0.;
	Scale  = 1.0;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
		0.f, 1.f, 0.f, 1.f
	      };

static float xy[ ] = {
		-.5f, .5f, .5f, -.5f
	      };

static int xorder[ ] = {
		1, 2, -3, 4
		};

static float yx[ ] = {
		0.f, 0.f, -.5f, .5f
	      };

static float yy[ ] = {
		0.f, .6f, 1.f, 1.f
	      };

static int yorder[ ] = {
		1, 2, 3, -2, 4
		};

static float zx[ ] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };

static float zy[ ] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };

static int zorder[ ] = {
		1, 2, 3, 4, -5, 6
		};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = (float)floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r, g, b;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}


void
SetMaterial( float r, float g, float b,  float shininess )
{
	glMaterialfv( GL_BACK, GL_EMISSION, Array3( 0., 0., 0. ) );
	glMaterialfv( GL_BACK, GL_AMBIENT, MulArray3( .4f, White ) );
	glMaterialfv( GL_BACK, GL_DIFFUSE, MulArray3( 1., White ) );
	glMaterialfv( GL_BACK, GL_SPECULAR, Array3( 0., 0., 0. ) );
	glMaterialf ( GL_BACK, GL_SHININESS, 5.f );

	glMaterialfv( GL_FRONT, GL_EMISSION, Array3( 0., 0., 0. ) );
	glMaterialfv( GL_FRONT, GL_AMBIENT, Array3( r, g, b ) );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, Array3( r, g, b ) );
	glMaterialfv( GL_FRONT, GL_SPECULAR, MulArray3( .8f, White ) );
	glMaterialf ( GL_FRONT, GL_SHININESS, shininess );
}


void
SetPointLight( int ilight, float x, float y, float z,  float r, float g, float b )
{
	glLightfv( ilight, GL_POSITION,  Array3( x, y, z ) );
	glLightfv( ilight, GL_AMBIENT,   Array3( 0., 0., 0. ) );
	glLightfv( ilight, GL_DIFFUSE,   Array3( r, g, b ) );
	glLightfv( ilight, GL_SPECULAR,  Array3( r, g, b ) );
	glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
	glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
	glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
	glEnable( ilight );
}


void
SetSpotLight( int ilight, float x, float y, float z,  float xdir, float ydir, float zdir, float r, float g, float b )
{
	glLightfv( ilight, GL_POSITION,  Array3( x, y, z ) );
	glLightfv( ilight, GL_SPOT_DIRECTION,  Array3(xdir,ydir,zdir) );
	glLightf(  ilight, GL_SPOT_EXPONENT, 1. );
	glLightf(  ilight, GL_SPOT_CUTOFF, 45. );
	glLightfv( ilight, GL_AMBIENT,   Array3( 0., 0., 0. ) );
	glLightfv( ilight, GL_DIFFUSE,   Array3( r, g, b ) );
	glLightfv( ilight, GL_SPECULAR,  Array3( r, g, b ) );
	glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
	glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
	glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
	glEnable( ilight );
}

float *
Array3( float a, float b, float c )
{
        static float array[4];

        array[0] = a;
        array[1] = b;
        array[2] = c;
        array[3] = 1.;
        return array;
}

float *
Array4( float a, float b, float c, float d )
{
        static float array[4];

        array[0] = a;
        array[1] = b;
        array[2] = c;
        array[3] = d;
        return array;
}

float *
BlendArray3( float factor, float array0[3], float array1[3] )
{
        static float array[4];

        array[0] = factor * array0[0]  +  ( 1.f - factor ) * array1[0];
        array[1] = factor * array0[1]  +  ( 1.f - factor ) * array1[1];
        array[2] = factor * array0[2]  +  ( 1.f - factor ) * array1[2];
        array[3] = 1.;
        return array;
}

float *
MulArray3( float factor, float array0[3] )
{
        static float array[4];

        array[0] = factor * array0[0];
        array[1] = factor * array0[1];
        array[2] = factor * array0[2];
        array[3] = 1.;
        return array;
}


#include <stdio.h>

int	ReadInt(FILE*);
short	ReadShort(FILE*);


struct bmfh
{
	short bfType;
	int bfSize;
	short bfReserved1;
	short bfReserved2;
	int bfOffBits;
} FileHeader;

struct bmih
{
	int biSize;
	int biWidth;
	int biHeight;
	short biPlanes;
	short biBitCount;
	int biCompression;
	int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	int biClrUsed;
	int biClrImportant;
} InfoHeader;

const int birgb = { 0 };



/**
 ** read a BMP file into a Texture:
 **/

unsigned char*
BmpToTexture(char* filename, int* width, int* height)
{

	int s, t, e;		// counters
	int numextra;		// # extra bytes each line in the file is padded with
	FILE* fp;
	unsigned char* texture;
	int nums, numt;
	unsigned char* tp;


	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		fprintf(stderr, "Cannot open Bmp file '%s'\n", filename);
		return NULL;
	}

	FileHeader.bfType = ReadShort(fp);


	// if bfType is not 0x4d42, the file is not a bmp:

	if (FileHeader.bfType != 0x4d42)
	{
		fprintf(stderr, "Wrong type of file: 0x%0x\n", FileHeader.bfType);
		fclose(fp);
		return NULL;
	}


	FileHeader.bfSize = ReadInt(fp);
	FileHeader.bfReserved1 = ReadShort(fp);
	FileHeader.bfReserved2 = ReadShort(fp);
	FileHeader.bfOffBits = ReadInt(fp);


	InfoHeader.biSize = ReadInt(fp);
	InfoHeader.biWidth = ReadInt(fp);
	InfoHeader.biHeight = ReadInt(fp);

	nums = InfoHeader.biWidth;
	numt = InfoHeader.biHeight;

	InfoHeader.biPlanes = ReadShort(fp);
	InfoHeader.biBitCount = ReadShort(fp);
	InfoHeader.biCompression = ReadInt(fp);
	InfoHeader.biSizeImage = ReadInt(fp);
	InfoHeader.biXPelsPerMeter = ReadInt(fp);
	InfoHeader.biYPelsPerMeter = ReadInt(fp);
	InfoHeader.biClrUsed = ReadInt(fp);
	InfoHeader.biClrImportant = ReadInt(fp);


	// fprintf( stderr, "Image size found: %d x %d\n", ImageWidth, ImageHeight );


	texture = new unsigned char[3 * nums * numt];
	if (texture == NULL)
	{
		fprintf(stderr, "Cannot allocate the texture array!\b");
		return NULL;
	}


	// extra padding bytes:

	numextra = 4 * (((3 * InfoHeader.biWidth) + 3) / 4) - 3 * InfoHeader.biWidth;


	// we do not support compression:

	if (InfoHeader.biCompression != birgb)
	{
		fprintf(stderr, "Wrong type of image compression: %d\n", InfoHeader.biCompression);
		fclose(fp);
		return NULL;
	}



	rewind(fp);
	fseek(fp, 14 + 40, SEEK_SET);

	if (InfoHeader.biBitCount == 24)
	{
		for (t = 0, tp = texture; t < numt; t++)
		{
			for (s = 0; s < nums; s++, tp += 3)
			{
				*(tp + 2) = fgetc(fp);		// b
				*(tp + 1) = fgetc(fp);		// g
				*(tp + 0) = fgetc(fp);		// r
			}

			for (e = 0; e < numextra; e++)
			{
				fgetc(fp);
			}
		}
	}

	fclose(fp);

	*width = nums;
	*height = numt;
	return texture;
}



int
ReadInt(FILE* fp)
{
	unsigned char b3, b2, b1, b0;
	b0 = fgetc(fp);
	b1 = fgetc(fp);
	b2 = fgetc(fp);
	b3 = fgetc(fp);
	return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
}


short
ReadShort(FILE* fp)
{
	unsigned char b1, b0;
	b0 = fgetc(fp);
	b1 = fgetc(fp);
	return (b1 << 8) | b0;
}
