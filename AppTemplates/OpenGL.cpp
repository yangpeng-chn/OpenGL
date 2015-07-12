#include <GLUT/glut.h>
#include <cstdlib> /* for exit */
#include <vector>
using namespace std;

float zoom = 1000.f;
float rotx = 20;
float roty = 0;
float tx = 0;
float ty = 0;
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};
float lightPosition[] = { -200, 300, 300, 1.0f};
float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
float diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
float specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };

void DrawCoordinate(float _flengthX, float _flengthY, float _flengthZ)
{
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(_flengthX,0,0);
    glEnd();
    
    glBegin(GL_LINES);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,_flengthY,0);
    glEnd();
    
    glBegin(GL_LINES);
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,_flengthZ);
    glEnd();
}

void Init()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
}


void Reshape(int w, int h)
{
    if(w==0)
        h = 1;
    
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(float)w/h,0.1,2000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void Motion(int x,int y)
{
    int diffx=x-lastx;
    int diffy=y-lasty;
    lastx=x;
    lasty=y;
    
    if( Buttons[2] )
    {
        zoom -= (float)  2* diffx*2;
    }
    else
        if( Buttons[0] )
        {
            rotx += (float) 1 * diffy;
            roty += (float) 1 * diffx;
        }
        else
            if( Buttons[1] )
            {
                tx += (float) 1 * diffx;
                ty -= (float) 1 * diffy;
            }
    glutPostRedisplay();
}

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

void DrawGrid(int _halfLen, int _gridWid)
{
    glColor3f(1.0f,1.0f,1.0f);
    glLineWidth(2);
    glBegin(GL_LINES);
    for(int i = -_halfLen;i <= _halfLen; i += _gridWid) 
    {
        glVertex3f(i,0,-_halfLen);
        glVertex3f(i,0,_halfLen);
        
        glVertex3f(_halfLen,0,i);
        glVertex3f(-_halfLen,0,i);
    }
    glEnd();
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(0,0,-zoom);
    glTranslatef(tx,ty,0);
    glRotatef(rotx,1,0,0);
    glRotatef(roty,0,1,0);
    
    DrawGrid(500,100);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    glColor3f(0, 1, 0);
    glPushMatrix();
    glTranslatef(0, 100, 0);
    glutSolidSphere(100, 50, 50);
    glPopMatrix();
    DrawCoordinate(500,500,500);
    
    glDisable( GL_LIGHTING );
    glDisable(GL_COLOR_MATERIAL);
    
    glutPostRedisplay();
    glutSwapBuffers();
}



void Keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 'q':
        case 'Q':
        case 27: // ESC key
            exit(0);
            break;
    }
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(100,100);
    glutCreateWindow("GLUT Framework");
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutKeyboardFunc(Keyboard);
    Init();
    glutMainLoop();
    return 0;
}