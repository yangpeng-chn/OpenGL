/*************************************************************************
�@�@Mouse.cpp
�@�@
�@�@Mouse Function Library

�@�@Version : 1.0
�@�@Date : Nov. 02, 2007
�@�@Author : Pocol
*************************************************************************/

//
//�@includes
//
#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include "Mouse.h"
using namespace std;


//
//�@global
//
char axisLabel[3] = { 'x', 'y', 'z' };
GLfloat axisColor_Red[4] = {1.0, 0.0, 0.0, 1.0};
GLfloat axisColor_Blue[4] = {0.0, 0.0, 1.0, 1.0};
GLfloat axisColor_Green[4] = {0.0, 1.0, 0.0, 1.0};
GLfloat axisColor_Cyan[4] = {0.0, 1.0, 1.0, 1.0};
GLfloat axisColor_Black[4] = {0.0, 0.0, 0.0, 1.0};


///////////////////////////////////////////////////////////////////////////
//�@�@MouseButton class
///////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------
//�@�@MouseButton
//�@�@Desc : �R���X�g���N�^
//------------------------------------------------------------------------------------------------------
MouseButton::MouseButton()
{
	Reset();
}


//------------------------------------------------------------------------------------------------------
//�@�@~MouseButton
//�@�@Desc : �f�X�g���N�^
//------------------------------------------------------------------------------------------------------
MouseButton::~MouseButton()
{
}


//------------------------------------------------------------------------------------------------------
//�@�@Reset
//�@�@Desc : ���Z�b�g����
//------------------------------------------------------------------------------------------------------
void MouseButton::Reset()
{
	before.x = 0.0;
	before.y = 0.0;
	current.x = 0.0;
	current.y = 0.0;
	after.x = 0.0;
	after.y = 0.0;
	state = None;
}

//------------------------------------------------------------------------------------------------------
//�@�@ConsoleOut
//�@�@Desc : �R���\�[���\��
//-------------------------------------------------------------------------------------------------------
void MouseButton::ConsoleOut()
{
	cout << "before"; 
	before.ConsoleOut();
	cout << "current";
	current.ConsoleOut();
	cout << "after";
	after.ConsoleOut();
}

///////////////////////////////////////////////////////////////////////////
//�@�@ViewCamera
///////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------------
//�@�@ViewCamera
//�@�@Desc : �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------
ViewCamera::ViewCamera(double dist)
{
	distance = dist;
	Reset();
}

//-------------------------------------------------------------------------------------------------------
//�@�@~ViewCamera
//�@�@Desc : �f�X�g���N�^
//-------------------------------------------------------------------------------------------------------
ViewCamera::~ViewCamera()
{
}

//-------------------------------------------------------------------------------------------------------
//�@�@MouseMotion
//�@�@Desc�F�}�E�X�̈ړ�����
//-------------------------------------------------------------------------------------------------------
void ViewCamera::MouseMotion( int x, int y ) 
{
	//�@���{�^���̏���
	if ( left.state == Push )
	{
		//�@�ړ��ʂ��v�Z
		left.current.x = (double)x - left.before.x + left.after.x;
		left.current.y = (double)y - left.before.y + left.after.y;

		if ( left.current.y >= 360.0 ) left.current.y -= 360.0;
		else if ( left.current.y < 0.0 ) left.current.y += 360.0;
		
		angle[0] = DegToRad(angle[0] + left.current.x);
		angle[1] = DegToRad(angle[1] + left.current.y);
	}

	//�@�E�{�^���̏���
	if ( right.state == Push )
	{
		right.current.x = (double)x - right.before.x + right.after.x;
		right.current.y  = -(double)y -right.before.y + right.after.y;
	}

	//�@���{�^���̏���
	if ( middle.state == Push )
	{
		//�@�ړ��ʂ��v�Z
		middle.current.x = (double)x -middle.before.x + middle.after.x;
		middle.current.y = (double)y -middle.before.y + middle.after.y;
		translate[0] = middle.current.x * 0.005;
		translate[1] = -middle.current.y * 0.005;
	}
}


//-------------------------------------------------------------------------------------------------------
//�@�@MouseInput
//�@�@Desc�F�}�E�X�̃{�^������
//-------------------------------------------------------------------------------------------------------
void ViewCamera::MouseInput( int button, int state, int x, int y ) 
{
	switch( button )
	{
	//�@���{�^��
	case GLUT_LEFT_BUTTON :
		if( state == GLUT_DOWN )
		{
			left.before.x = (double)x;
			left.before.y = (double)y;
			left.state = Push;
		}
		else if( state == GLUT_UP )
		{	
			left.after.x = left.current.x;
			left.after.y = left.current.y;
			left.state = Release;
		}
		break;

	//�@�E�{�^��
	case GLUT_RIGHT_BUTTON :
		if( state == GLUT_DOWN ) 
		{ 		
			right.before.x = (double)x;
			right.before.y = -(double)y;
			right.state = Push;
		}
		else if( state == GLUT_UP )
		{			
			right.after.x = right.current.x;
			right.after.y = right.current.y;
			right.state = Release;
		}
		break;

	//�@���{�^��
	case GLUT_MIDDLE_BUTTON :
		if ( state == GLUT_DOWN )
		{
			middle.before.x = (double)x;
			middle.before.y = (double)y;
			middle.state = Push;
		}
		else if ( state == GLUT_UP )
		{
			middle.after.x = middle.current.x;
			middle.after.y = middle.current.y;
			middle.state = Release;
		}
		break;
	}

}


//------------------------------------------------------------------------------------------------------
//�@�@Reset
//�@�@Desc�F�p�����[�^�����Z�b�g����
//------------------------------------------------------------------------------------------------------
void ViewCamera::Reset()
{
	left.Reset();
	right.Reset();
	middle.Reset();

	angle[0] = DegToRad(45.0);
	angle[1] = DegToRad(45.0);
	angle[2] = 0.0;
	position[0] = 0.0;
	position[1] = 0.0;
	position[2] = 0.0;
	target[0] = 0.0;
	target[1] = 0.0;
	target[2] = 0.0;
	upvector[0] = 0.0;
	upvector[1] = 1.0;
	upvector[2] = 0.0;
	translate[0] = 0.0;
	translate[1] = 0.0;
	translate[2] = 0.0;
}
		

//------------------------------------------------------------------------------------------------------
//�@�@drawDisk
//�@�@Desc:�~�Ղ�`�悷��
//------------------------------------------------------------------------------------------------------
void drawDisk()
{
	GLUquadricObj *qobj;
	qobj = gluNewQuadric();
	glPushMatrix();
		glRotated( 180, 1.0, 0.0, 0.0 );
		gluDisk( qobj, 0.0, 0.35, 10, 10 );
	glPopMatrix();
}


//------------------------------------------------------------------------------------------------------
//�@�@Set
//�@�@Desc�F���_�̐ݒ�
//------------------------------------------------------------------------------------------------------
void ViewCamera::Set()
{
	double zoom = distance;
	zoom += right.current.y/30.0;

	//�@���_�ʒu������
	position[0] = sin(angle[0]) * cos(angle[1]) * zoom;
	position[1] = sin(angle[1]) * zoom;
	position[2] = cos(angle[0]) * cos(angle[1]) * zoom;

	//�@�A�b�v�x�N�g���̐ݒ�
	if( angle[1] > DegToRad(90.0)  &&  angle[1] < DegToRad(270.0) ) upvector[1] = -1.0;
	else upvector[1] = 1.0;

	//�@���s�ړ�
	glTranslated( translate[0], translate[1], translate[2] );

	//�@���_�ʒu�̐ݒ�
	gluLookAt(
		position[0], position[1], position[2],
		target[0], target[1], target[2],
		upvector[0], upvector[1], upvector[2] );

}

//------------------------------------------------------------------------------------------------------
//�@�@RenderSubAxis
//�@�@Desc�F�⏕���̕`��
//------------------------------------------------------------------------------------------------------
void ViewCamera::RenderSubAxis(int w, int h)
{
	const double zoom = 15.0;
	double eye[3] = {0.0, 0.0, 0.0};
	
	//�@�E�B���h�E�S�̂��r���[�|�[�g�ɂ���
	glViewport( w-100, h-100, 100, 100);//todo
	
	//�@�����ϊ��s��̐ݒ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1, 1, 100000.0);

	//�@���f���r���[�ϊ��̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	eye[0] = sin(angle[0])*cos(angle[1])*zoom;
	eye[1] = sin(angle[1])*zoom;
	eye[2] = cos(angle[0])*cos(angle[1])*zoom;

	//�@���_�ʒu�̐ݒ�
	gluLookAt( 
		eye[0], eye[1], eye[2],
		0.0, 0.0, 0.0,
		upvector[0],upvector[1], upvector[2] );

	//�@���̕���
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor4fv(axisColor_Black ); 
	glRasterPos3d(2.25, 0.0, 0.0);		
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, (int)axisLabel[0]);
	glRasterPos3d(0.0, 2.25, 0.0);		
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, (int)axisLabel[1]);
	glRasterPos3d(0.0, 0.0, 2.25);		
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, (int)axisLabel[2]);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	

	//�@x����
	glPushMatrix();
	glColor4fv(axisColor_Red);
	glMaterialfv(GL_FRONT, GL_AMBIENT, axisColor_Red);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, axisColor_Red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, axisColor_Red);
	glTranslated(1.75, 0.0, 0.0);
	glRotated(-90.0, 0.0, 1.0, 0.0);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();	

	//�@y����
	glPushMatrix();	
	glColor4fv(axisColor_Green);
	glMaterialfv(GL_FRONT, GL_AMBIENT, axisColor_Green);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, axisColor_Green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, axisColor_Green);
	glTranslated(0.0, 1.75, 0.0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();

	//�@z����
	glPushMatrix();
	glColor4fv(axisColor_Blue);
	glMaterialfv(GL_FRONT, GL_AMBIENT, axisColor_Blue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, axisColor_Blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, axisColor_Blue);
	glTranslated(0.0, 0.0, 1.75);
	glRotated(180.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();

	//�@�L���[�u
	glPushMatrix();
	glColor4fv(axisColor_Cyan);
	glMaterialfv(GL_FRONT, GL_AMBIENT, axisColor_Cyan);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, axisColor_Cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, axisColor_Cyan);
	glutSolidCube(1.0);
	glPopMatrix();

	//�@x����
	glPushMatrix();
	glTranslated(-1.75, 0.0, 0.0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();

	//�@y����
	glPushMatrix();
	glTranslated(0.0, -1.75, 0.0);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();

	//�@z����
	glPushMatrix();
	glTranslated(0.0, 0.0, -1.75);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();	

}
