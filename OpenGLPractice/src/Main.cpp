/*		This code has been created by Banu Octavian aka Choko - 20 may 2000
 *		and uses NeHe tutorials as a starting point (window initialization,
 *		texture loading, GL initialization and code for keypresses) - very good
 *		tutorials, Jeff. If anyone is interested about the presented algorithm
 *		please e-mail me at boct@romwest.ro
 *		Attention!!! This code is not for beginners.
 */


#include "Settings.h"									// Header File For 3D Object Handling

typedef float GLvector4f[4];							// Typedef's For VMatMult Procedure
typedef float GLmatrix16f[16];							// Typedef's For VMatMult Procedure

glObject	obj;										// Object
GLfloat		xrot=0, xspeed=0;							// X Rotation & X Speed
GLfloat		yrot=0, yspeed=0;							// Y Rotation & Y Speed

float LightPos[] = { 0.0f, 5.0f,-4.0f, 1.0f};			// Light Position
float LightAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f};			// Ambient Light Values
float LightDif[] = { 0.6f, 0.6f, 0.6f, 1.0f};			// Diffuse Light Values
float LightSpc[] = {-0.2f, -0.2f, -0.2f, 1.0f};			// Specular Light Values

float MatAmb[] = {0.4f, 0.4f, 0.4f, 1.0f};				// Material - Ambient Values
float MatDif[] = {0.2f, 0.6f, 0.9f, 1.0f};				// Material - Diffuse Values
float MatSpc[] = {0.0f, 0.0f, 0.0f, 1.0f};				// Material - Specular Values
float MatShn[] = {0.0f};								// Material - Shininess

float ObjPos[] = {-2.0f,-2.0f,-5.0f};					// Object Position

GLUquadricObj	*q;										// Quadratic For Drawing A Sphere
float SpherePos[] = {-4.0f,-5.0f,-6.0f};


void VMatMult(GLmatrix16f M, GLvector4f v)
{
	GLfloat res[4];										// Hold Calculated Results
	res[0]=M[ 0]*v[0]+M[ 4]*v[1]+M[ 8]*v[2]+M[12]*v[3];
	res[1]=M[ 1]*v[0]+M[ 5]*v[1]+M[ 9]*v[2]+M[13]*v[3];
	res[2]=M[ 2]*v[0]+M[ 6]*v[1]+M[10]*v[2]+M[14]*v[3];
	res[3]=M[ 3]*v[0]+M[ 7]*v[1]+M[11]*v[2]+M[15]*v[3];
	v[0]=res[0];										// Results Are Stored Back In v[]
	v[1]=res[1];
	v[2]=res[2];
	v[3]=res[3];										// Homogenous Coordinate
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.001f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGLObjects()										// Initialize Objects
{
	if (!ReadObject("Data/Object2.txt", &obj))			// Read Object2 Into obj
	{
		return false;									// If Failed Return False
	}

	SetConnectivity(&obj);								// Set Face To Face Connectivity

	for (unsigned int i=0;i<obj.nPlanes;i++)			// Loop Through All Object Planes
		CalcPlane(obj, &(obj.planes[i]));				// Compute Plane Equations For All Faces

	return true;										// Return True
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	if (!InitGLObjects()) return false;					// Function For Initializing Our Object(s)
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Stencil Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);		// Set Light1 Position
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);			// Set Light1 Ambience
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDif);			// Set Light1 Diffuse
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc);		// Set Light1 Specular
	glEnable(GL_LIGHT1);								// Enable Light1
	glEnable(GL_LIGHTING);								// Enable Lighting

	glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmb);			// Set Material Ambience
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDif);			// Set Material Diffuse
	glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpc);		// Set Material Specular
	glMaterialfv(GL_FRONT, GL_SHININESS, MatShn);		// Set Material Shininess

	glCullFace(GL_BACK);								// Set Culling Face To Back Face
	glEnable(GL_CULL_FACE);								// Enable Culling
	glClearColor(0.1f, 1.0f, 0.5f, 1.0f);				// Set Clear Color (Greenish Color)

	q = gluNewQuadric();								// Initialize Quadratic
	gluQuadricNormals(q, GL_SMOOTH);					// Enable Smooth Normal Generation
	gluQuadricTexture(q, GL_FALSE);						// Disable Auto Texture Coords

	return true;										// Initialization Went OK
}

void DrawGLRoom()										// Draw The Room (Box)
{
	glBegin(GL_QUADS);									// Begin Drawing Quads
		// Floor
		glNormal3f(0.0f, 1.0f, 0.0f);					// Normal Pointing Up
		glVertex3f(-10.0f,-10.0f,-20.0f);				// Back Left
		glVertex3f(-10.0f,-10.0f, 20.0f);				// Front Left
		glVertex3f( 10.0f,-10.0f, 20.0f);				// Front Right
		glVertex3f( 10.0f,-10.0f,-20.0f);				// Back Right
		// Ceiling
		glNormal3f(0.0f,-1.0f, 0.0f);					// Normal Point Down
		glVertex3f(-10.0f, 10.0f, 20.0f);				// Front Left
		glVertex3f(-10.0f, 10.0f,-20.0f);				// Back Left
		glVertex3f( 10.0f, 10.0f,-20.0f);				// Back Right
		glVertex3f( 10.0f, 10.0f, 20.0f);				// Front Right
		// Front Wall
		glNormal3f(0.0f, 0.0f, 1.0f);					// Normal Pointing Away From Viewer
		glVertex3f(-10.0f, 10.0f,-20.0f);				// Top Left
		glVertex3f(-10.0f,-10.0f,-20.0f);				// Bottom Left
		glVertex3f( 10.0f,-10.0f,-20.0f);				// Bottom Right
		glVertex3f( 10.0f, 10.0f,-20.0f);				// Top Right
		// Back Wall
		glNormal3f(0.0f, 0.0f,-1.0f);					// Normal Pointing Towards Viewer
		glVertex3f( 10.0f, 10.0f, 20.0f);				// Top Right
		glVertex3f( 10.0f,-10.0f, 20.0f);				// Bottom Right
		glVertex3f(-10.0f,-10.0f, 20.0f);				// Bottom Left
		glVertex3f(-10.0f, 10.0f, 20.0f);				// Top Left
		// Left Wall
		glNormal3f(1.0f, 0.0f, 0.0f);					// Normal Pointing Right
		glVertex3f(-10.0f, 10.0f, 20.0f);				// Top Front
		glVertex3f(-10.0f,-10.0f, 20.0f);				// Bottom Front
		glVertex3f(-10.0f,-10.0f,-20.0f);				// Bottom Back
		glVertex3f(-10.0f, 10.0f,-20.0f);				// Top Back
		// Right Wall
		glNormal3f(-1.0f, 0.0f, 0.0f);					// Normal Pointing Left
		glVertex3f( 10.0f, 10.0f,-20.0f);				// Top Back
		glVertex3f( 10.0f,-10.0f,-20.0f);				// Bottom Back
		glVertex3f( 10.0f,-10.0f, 20.0f);				// Bottom Front
		glVertex3f( 10.0f, 10.0f, 20.0f);				// Top Front
	glEnd();											// Done Drawing Quads
}

float zoom = 0;
void DrawGLScene()									// Main Drawing Routine
{
	GLmatrix16f Minv;
	GLvector4f wlp, lp;

	// Clear Color Buffer, Depth Buffer, Stencil Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glLoadIdentity();									// Reset Modelview Matrix
	glTranslatef(0.0f, 0.0f, -20.0f);					// Zoom Into Screen 20 Units
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);		// Position Light1
	glTranslatef(SpherePos[0], SpherePos[1], SpherePos[2]);	// Position The Sphere
	gluSphere(q, 1.5f, 32, 16);							// Draw A Sphere

	// calculate light's position relative to local coordinate system
	// dunno if this is the best way to do it, but it actually works
	// if u find another aproach, let me know ;)

	// we build the inversed matrix by doing all the actions in reverse order
	// and with reverse parameters (notice -xrot, -yrot, -ObjPos[], etc.)
	glLoadIdentity();									// Reset Matrix
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);					// Rotate By -yrot On Y Axis
	glRotatef(-xrot, 1.0f, 0.0f, 0.0f);					// Rotate By -xrot On X Axis
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// Retrieve ModelView Matrix (Stores In Minv)
	lp[0] = LightPos[0];								// Store Light Position X In lp[0]
	lp[1] = LightPos[1];								// Store Light Position Y In lp[1]
	lp[2] = LightPos[2];								// Store Light Position Z In lp[2]
	lp[3] = LightPos[3];								// Store Light Direction In lp[3]
	VMatMult(Minv, lp);									// We Store Rotated Light Vector In 'lp' Array
	glTranslatef(-ObjPos[0], -ObjPos[1], -ObjPos[2]);	// Move Negative On All Axis Based On ObjPos[] Values (X, Y, Z)
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// Retrieve ModelView Matrix From Minv
	wlp[0] = 0.0f;										// World Local Coord X To 0
	wlp[1] = 0.0f;										// World Local Coord Y To 0
	wlp[2] = 0.0f;										// World Local Coord Z To 0
	wlp[3] = 1.0f;
	VMatMult(Minv, wlp);								// We Store The Position Of The World Origin Relative To The
														// Local Coord. System In 'wlp' Array
	lp[0] += wlp[0];									// Adding These Two Gives Us The
	lp[1] += wlp[1];									// Position Of The Light Relative To
	lp[2] += wlp[2];									// The Local Coordinate System

	glColor4f(0.7f, 0.4f, 0.0f, 1.0f);					// Set Color To An Orange
	glLoadIdentity();									// Reset Modelview Matrix
	glTranslatef(0.0f, 0.0f, -20.0f);					// Zoom Into The Screen 20 Units
	DrawGLRoom();										// Draw The Room
	glTranslatef(ObjPos[0], ObjPos[1], ObjPos[2]);		// Position The Object
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);					// Spin It On The X Axis By xrot
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);					// Spin It On The Y Axis By yrot
	DrawGLObject(obj);									// Procedure For Drawing The Loaded Object
	CastShadow(&obj, lp);								// Procedure For Casting The Shadow Based On The Silhouette

	glColor4f(0.7f, 0.4f, 0.0f, 1.0f);					// Set Color To Purplish Blue
	glDisable(GL_LIGHTING);								// Disable Lighting
	glDepthMask(GL_FALSE);								// Disable Depth Mask
	glTranslatef(lp[0], lp[1], lp[2]);					// Translate To Light's Position
	gluSphere(q, 0.2f, 16, 8);							// Draw A Little Yellow Sphere (Represents Light)
	glEnable(GL_LIGHTING);								// Enable Lighting
	glDepthMask(GL_TRUE);								// Enable Depth Mask

	xrot += xspeed;										// Increase xrot By xspeed
	yrot += yspeed;										// Increase yrot By yspeed
	glutPostRedisplay();
	glutSwapBuffers();
	//glFlush();											// Flush The OpenGL Pipeline
}

float rotx = 0;
float roty = 10;
float tx = 0;
float ty = 0;
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};

void Motion(int x,int y)
{
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[2] )
	{
		zoom -= (float) 0.05f * diffx*2;
	}
	else
		if( Buttons[0] )
		{
			rotx += (float) 0.5f * diffy;
			roty += (float) 0.5f * diffx;      
		}
		else
			if( Buttons[1] )
			{
				tx += (float) 0.05f * diffx;
				ty -= (float) 0.05f * diffy;
			}
			glutPostRedisplay();
}

//-------------------------------------------------------------------------------
//
void Mouse(int b,int s,int x,int y)
{
	lastx=x;
	lasty=y;
	switch(b)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
	default:
		break;      
	}
	glutPostRedisplay();
}

int main(int argc,char** argv) 
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(200,100);
	glutCreateWindow("TestBed Framework");
	glutReshapeFunc(ReSizeGLScene);
	glutDisplayFunc(DrawGLScene);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	InitGL();

	glutMainLoop();

	return 0;
}
