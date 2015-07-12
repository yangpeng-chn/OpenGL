
#include <stdlib.h>
#include <stdio.h>
#include <GL\glut.h>
#include <gl\gl.h>	
#include <gl\glu.h>	
#include <gl\glaux.h>
#include "OBJLoader.h"
#include "OBJ.h"
#define TEX_HEIGHT 16
#define TEX_WIDTH  16
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];
 vector<GLuint> texture;
 vector<AUX_RGBImageRec*> TextureImage;
int width = 640;
int height = 480;
mesh *object;
MODEL* model;
int mode = GL_FILL;

AUX_RGBImageRec *LoadBMP(const char *Filename)				
{
	FILE *File=NULL;									
	if (!Filename)										
	{
		return NULL;									
	}
	File=fopen(Filename,"r");							
	if (File)											
	{
		fclose(File);									
		return auxDIBImageLoad(Filename);				
	}
	return NULL;										
}



int LoadGLTextures(const char* _fileName)									
{
	static int counter = 0;
	int Status=FALSE;			
	
	//memset(TextureImage,0,sizeof(void *)*1);    
	texture.resize(counter+1);
	TextureImage.resize(counter+1);

	if (TextureImage[counter]=LoadBMP(_fileName)) 
	{
		Status=TRUE;									
		glGenTextures(1, &texture[counter]);					
		glBindTexture(GL_TEXTURE_2D, texture[counter]); 
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[counter]->sizeX, TextureImage[counter]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[counter]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	counter++;
	return Status;										
}
void initTexture(void)
{
	/*int i, j, c;
	for (i=0;i<TEX_HEIGHT;i++) {
		for (j=0;j<TEX_WIDTH;j++) {
			c = ( ((i&0x01)==0)^((j&0x01)==0) );
			image[i][j][0]= image[i][j][1]= image[i][j][2]=c*255;
			image[i][j][3]=255;
		}
	}*/
	//LoadGLTextures("model/coca1-2.bmp");
	//LoadGLTextures("model/coca2.bmp");
}
void displayTexPolygon(void)
{
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glEnable(GL_TEXTURE_2D);
	/*glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-5.0,-5.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f( 5.0, 5.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f( 5.0,-5.0, 0.0);
	glEnd();*/
	glBegin(GL_QUADS);
	// Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	// Back Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	// Top Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	// Bottom Face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	// Right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	// Left Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void showModel()
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
	int lastMaterial = -1;
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	for(size_t i=0;i < object->fTotal;++i)
	{
		// set material property if this face used different material
		if(lastMaterial != object->faceList[i].m)
		{
			lastMaterial = (int)object->faceList[i].m;
			glMaterialfv(GL_FRONT, GL_AMBIENT  , object->mList[lastMaterial].Ka);
			glMaterialfv(GL_FRONT, GL_DIFFUSE  , object->mList[lastMaterial].Kd);
			glMaterialfv(GL_FRONT, GL_SPECULAR , object->mList[lastMaterial].Ks);
			glMaterialfv(GL_FRONT, GL_SHININESS, &object->mList[lastMaterial].Ns);
			

			//you can obtain the texture name by object->mList[lastMaterial].map_Kd
			//load them once in the main function before mainloop
			//bind them in display function here
		}
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLES);
		for (size_t j=0;j<3;++j)
		{
			//textex corrd. object->tList[object->faceList[i][j].t].ptr
			glTexCoord3fv(object->tList[object->faceList[i][j].t].ptr);
			glNormal3fv(object->nList[object->faceList[i][j].n].ptr);
			glVertex3fv(object->vList[object->faceList[i][j].v].ptr);	
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

void showModel1()
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
	model->Draw();
}
void display(void)
{
	static float spin=0.0;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glPushMatrix();
	{
		gluPerspective(20, (GLfloat)width/(GLfloat)height, 1.0, 1000.0);
		glTranslatef(0.0, 0.0,-20.0);
		glRotatef(spin, 1.0, 1.0, 1.0);
		glColor3f(1.0, 1.0, 1.0);
		//displayTexPolygon();
	}
	spin+=1.0;
	glPopMatrix();
	//glFlush();
	//glutSwapBuffers();

	// clear the buffer
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //睲埃ノcolor
	//glClearDepth(1.0f);                        // Depth Buffer (碞琌z buffer) Setup
	//glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
	//glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//硂︽р礶睲Θ堵︹睲埃z buffer

	// viewport transformation
	//glViewport(0, 0, 640, 480);

	// projection transformation
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(60.0, (GLfloat)width/(GLfloat)height, 1.0, 1000.0);
	//gluPerspective(20, (GLfloat)width/(GLfloat)height, 300.0, 500);
	// viewing and modeling transformation
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	0, 300, 10,// eye
		0.0, 0.0, 0.0,     // center
		0.0, 1.0, 0.0);    // up*/

	

	//猔種light竚砞﹚璶gluLookAtぇ
	//light();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPushMatrix();
	{
		gluPerspective(100, (GLfloat)width/(GLfloat)height, 1.0, 1000.0);
		//glTranslatef(0, 0.0,-30);//ground
		//glRotatef(spin, 1.0, 0.0, 0.0);//ground

		//glTranslatef(0, 0.0,-3);//wall
		//glRotatef(spin, 1.0, 0.0, 0.0);//wall
		//showModel();
		

		//glTranslatef(-52.0, 0.0,54);//iphone
		//glRotatef(898.5, 1.0, 0.0, 0.0);//iphone

		glTranslatef(0, 0.0,-10);
		glRotatef(spin, 0.5, 0.0, 0.0);
		showModel1();
		//displayTexPolygon();
	}
	spin+=1.0;
	glPopMatrix();

	

	glutSwapBuffers();
}

void init(void) 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	object = new mesh("model/wall.obj");
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	initTexture();
	/*glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT, 
		//0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);			
	glLoadIdentity();				
	glFrustum(-5.0, 5.0,-5.0, 5.0, 5.0, 500.0); 
	glMatrixMode(GL_MODELVIEW);			
	glLoadIdentity();				
}

void Keyboard(unsigned char key, int x, int y)
{
	switch ( key )
	{
	case 'L'|'l':
		mode = GL_LINE;
		break;
	case 'F'|'f':
		mode = GL_FILL;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	
	glutInit(&argc, argv);			
	glutInitDisplayMode (GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize (250, 250);		
	glutInitWindowPosition (100, 100);		
	glutCreateWindow (argv[0]);			
	init();				
	model = new MODEL("model/coca.obj");
	glutIdleFunc(display);			
	glutDisplayFunc(display);			
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(Keyboard);
	glutMainLoop();				
	return 0;					
}