#include <GLUT/glut.h>
#include <cstdlib> /* for exit */
#include <vector>
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

using namespace std;

float zoom = 2000.f;
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

btDiscreteDynamicsWorld* mp_btDynamicsWorld = NULL;
btRigidBody *cube = NULL;
btRigidBody *ground = NULL;

void InitWorld()
{
    btDefaultCollisionConfiguration *config = new btDefaultCollisionConfiguration();
    btCollisionDispatcher *dispatcher = new btCollisionDispatcher(config);
    
    btDbvtBroadphase *broadphase = new btDbvtBroadphase();  // Dynamic AABB tree method
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    mp_btDynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
    mp_btDynamicsWorld->setGravity(btVector3(0, -9800, 0)); //millimeter 9.8 * 1000
}

void InitObject()
{
    //init cube
    btCollisionShape *collisionShape = new btBoxShape(btVector3(200,200,200));
    //initial position
    btVector3 pos = btVector3(0, 600, 0);
    btQuaternion qrot(0, 0, 0, 1);
    
    btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, pos));
    
    btScalar mass = btScalar(10);
    btVector3 inertia = btVector3(0, 0, 0);//guan xing
    collisionShape->calculateLocalInertia(mass, inertia);
    
    cube = new btRigidBody(mass, motion_state, collisionShape, inertia);
    
    btScalar restitution = btScalar(0);
    cube->setRestitution(restitution);
    //default 0.5
    btScalar friction = btScalar(0.8);
    cube->setFriction(friction);
    
    mp_btDynamicsWorld->addRigidBody(cube);
    
    //init ground
    btCollisionShape *groundShape = new btBoxShape(btVector3(1000,0.5,1000)); //half size
    
    btVector3 groundpos = btVector3(0,0,0);
    btQuaternion groundrot(0, 0, 0, 1);
    btDefaultMotionState* groundMotion = new btDefaultMotionState(btTransform(groundrot, groundpos));
    ground  = new btRigidBody(0.0, groundMotion, groundShape);//mass = 0 means it is a static object
    btScalar rest = btScalar(1);
    ground->setRestitution(rest);
    mp_btDynamicsWorld->addRigidBody(ground);
}

void DeleteBullet()
{
    //cube
    delete cube->getMotionState();
    mp_btDynamicsWorld->removeRigidBody(cube);
    delete cube;
    cube = NULL;
    
    //ground
    delete ground->getMotionState();
    mp_btDynamicsWorld->removeRigidBody(ground);
    delete ground;
    ground = NULL;
    
    //world
    delete mp_btDynamicsWorld->getBroadphase();
    delete mp_btDynamicsWorld;
    mp_btDynamicsWorld = NULL;
}

void DrawGrid(int _halfLen, int _gridNum)
{
    glColor3f(1.0f,1.0f,1.0f);
    
    // draw grid
    glLineWidth(2);
    glBegin(GL_LINES);
    for(int i = -_halfLen;i <= _halfLen; i += (_halfLen/_gridNum)) {
        glVertex3f(i,0,-_halfLen);
        glVertex3f(i,0,_halfLen);
        
        glVertex3f(_halfLen,0,i);
        glVertex3f(-_halfLen,0,i);
    }
    glEnd();
}
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

void DrawBulletObject()
{
    //cube
    btTransform trans = cube->getWorldTransform();
    btScalar m[16];
    trans.getOpenGLMatrix(m);
    glColor3f(0, 0, 1);
    glPushMatrix();
    glMultMatrixf((GLfloat*)m);
    glutSolidCube(400);
    glPopMatrix();
    
    //ground
    glColor3f(0, 1, 0);
    glPushMatrix();
    glScalef(1, 0.0005, 1);
    glutSolidCube(2000); //size
    glPopMatrix();
    
}

void Simulate()
{
    double dt = 1.f/60.0f;
    if(mp_btDynamicsWorld)
        mp_btDynamicsWorld->stepSimulation(dt,1);
}
//-------------------------------------------------------------------------------
///
void Display()
{
    Simulate();
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    
    glLoadIdentity();
    
    glTranslatef(0,0,-zoom);
    glTranslatef(tx,ty,0);
    glRotatef(rotx,1,0,0);
    glRotatef(roty,0,1,0);
    
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    DrawBulletObject();
    glDisable( GL_LIGHTING );
    glDisable(GL_COLOR_MATERIAL);
    
    //DrawGrid(1000, 10);
    //DrawCoordinate(1000,1000,1000);
    
    glutPostRedisplay();
    glutSwapBuffers();
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
    
    
    InitWorld();
    InitObject();
}


void Reshape(int w, int h)
{
    // prevent divide by 0 error when minimised
    if(w==0)
        h = 1;
    
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(float)w/h,0.1,5000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


//-------------------------------------------------------------------------------
//
void Motion(int x,int y)
{
    int diffx=x-lastx;
    int diffy=y-lasty;
    lastx=x;
    lasty=y;
    
    if( Buttons[2] )
    {
        zoom -= (float)  1* diffx*2;
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


void Keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 'q':
        case 'Q':
        case 27: // ESC key
            exit(0);
        break;
        case 'r':
            DeleteBullet();
            InitWorld();
            InitObject();
        break;
        default:
        break;
    }
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Bullet Framework");
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutKeyboardFunc(Keyboard);
    Init();
    
    glutMainLoop();
    
    return 0;
}