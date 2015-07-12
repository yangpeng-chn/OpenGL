
#include <stdlib.h>
#include <stdio.h>
#include <GL\glut.h>
#include <gl\gl.h>	
#include <gl\glu.h>	
#include <gl\glaux.h>
#define TEX_HEIGHT 16
#define TEX_WIDTH  16
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];
GLuint	texture[1];


AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image   //ﾐﾂﾔｯﾊ ｼﾓﾔﾘﾎｻﾍｼﾎﾄｼ
{
	FILE *File=NULL;									// File Handleﾎﾄｼｾ莖・

	if (!Filename)										// Make Sure A Filename Was Given ﾈ郢鉸ﾄｼﾃ鈊ｻﾌ盪ｩ
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists//ｳ｢ﾊﾔｴｪﾎﾄｼ

	if (File)											// Does The File Exist? ﾈ郢鉸ﾄｼｴ贇ﾚ
	{
		fclose(File);									// Close The Handleｹﾘｱﾕｾ莖・
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointerﾔﾘﾈ・ｻﾍｼｲ｢ｷｵｻﾘﾖｸﾕ・
	}

	return NULL;										// If Load Failed Return NULL ﾈ郢鋕ﾘﾈ・ｧｰﾜ ｷｵｻﾘNULL
}
//ﾏﾂﾒｻｲｿｷﾖｴ惲・ﾘﾈ・ｻﾍｼ(ｵﾃﾉﾏﾃ豬ﾄｴ惲・ｲ｢ﾗｪｻｻｳﾉﾎﾆﾀ悅｣
int LoadGLTextures()									// Load Bitmaps And Convert To Texturesﾔﾘﾈ・ｻﾍｼｵﾄｺｯﾊ
{
	int Status=FALSE;									// Status Indicatorﾗｴﾌｬﾖｸﾊｾﾆ・

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Textureｴｴｽｨﾎﾆﾀ昉ﾄｴ豢｢ｿﾕｼ・


	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULLﾇ蟲ﾍｼﾏﾇﾂｼ｣ｬﾈｷｱ｣ﾆ萋ﾚﾈﾝﾎｪｿﾕ

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP("model/1.bmp"))  //ﾏﾖﾔﾚﾔﾘﾈ・ｻﾍｼ｣ｬｲ｢ｽｫﾆ葫ｪｻｻﾎｪﾎﾆﾀ・ﾈ郢﨤ﾉｹｦ
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[0]);					// Create The Texture //ﾉ嵭ﾉﾒｻｸﾆﾀ朎錥ﾖ ｴｴｽｨﾎﾆﾀ・

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]); //ﾊｹﾓﾃﾀｴﾗﾔﾎｻﾍｼﾊｾﾝﾉ嵭ﾉ ｵﾄｵ菻ﾍﾎﾆﾀ・
		/*ﾏﾂﾀｴﾎﾒﾃﾇｴｴｽｨﾕ贏ｵﾄﾎﾆﾀ悅｣ﾏﾂﾃ贅ｻﾐﾐｸ賤ﾟOpenGLｴﾋﾎﾆﾀ桄ﾇﾒｻｸ・Dﾎﾆﾀ・( GL_TEXTURE_2D )｡｣ｲﾎﾊ｡ｰ0｡ｱｴ﨑楨ｼﾏﾄﾏ・ｸｳﾌｶﾈ｣ｬ
			ﾍｨｳ｣ｾﾍﾓﾉﾋ・ｪﾁ翳･ﾁﾋ｡｣ｲﾎﾊﾈﾊﾇﾊｾﾝｵﾄｳﾉｷﾖﾊ｡｣ﾒｪﾍｼﾏﾇﾓﾉｺ・ｫﾊｾﾝ｣ｬﾂﾌﾉｫﾊｾﾝ｣ｬﾀｶﾉｫﾊｾﾝﾈﾖﾖﾗ鮃ﾖﾗ魑ﾉ｡｣ 
			TextureImage[0]->sizeX ﾊﾇﾎﾆﾀ昉ﾄｿ昮ﾈ｡｣ﾈ郢鈺晗ｪｵﾀｿ昮ﾈ｣ｬﾄ惞ﾉﾒﾔﾔﾚﾕ簑・鏸・ｬｵｫｼﾆﾋ羹惞ﾉﾒﾔｺﾜﾈﾝﾒﾗｵﾄﾎｪﾄ晗ｸｳﾋﾖｵ｡｣
			TextureImage[0]->sizey ﾊﾇﾎﾆﾀ昉ﾄｸﾟｶﾈ｡｣ｲﾎﾊﾁ飜ﾇｱﾟｿﾄﾖｵ｣ｬﾒｻｰ羸ﾍﾊﾇ｡ｰ0｡ｱ｡｣ GL_RGB ｸ賤ﾟOpenGLﾍｼﾏｾﾝﾓﾉｺ・｢ﾂﾌ｡｢
			ﾀｶﾈﾉｫﾊｾﾝﾗ魑ﾉ｡｣GL_UNSIGNED_BYTE ﾒ簧ｶﾗﾅﾗ魑ﾉﾍｼﾏﾄﾊｾﾝﾊﾇﾎﾞｷ釚ﾅﾗﾖｽﾚﾀ獎ﾍｵﾄ｡｣ﾗ鋓・.. TextureImage[0]->data ｸ賤ﾟ
			OpenGLﾎﾆﾀ桄ｾﾝｵﾄﾀｴﾔｴ｡｣ｴﾋﾀﾖﾐﾖｸﾏ豺ﾅﾔﾚ TextureImage[0] ｼﾇﾂｼﾖﾐｵﾄﾊｾﾝ｡｣*/
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//ﾋ｡ｵﾃｱﾈﾔｭﾊｼﾎﾆﾀ槢｡
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//ｷﾅｴﾃｱﾈﾔｭﾊｼｵﾄﾎﾆﾀ昻・  GL_LINEARﾎｪｲﾉﾓﾃｵﾄﾂﾋｲｨｷｽﾊｽ
	}

	if (TextureImage[0])									// If Texture Exists//ﾖｮﾇｰｵﾄﾎｻﾍｼ
	{
		if (TextureImage[0]->data)							// If Texture Image Exists ﾎﾆﾀ楨ｼﾏ・
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return Status;										// Return The Status
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
	LoadGLTextures();
}
void displayTexPolygon(void)
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
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
	//glDisable(GL_TEXTURE_2D);
}

void display(void)
{
	static float spin=0.0;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	{
		glTranslatef(0.0, 0.0,-20.0);
		glRotatef(spin, 0.0, 1.0, 0.0);
		glColor3f(1.0, 1.0, 1.0);
		displayTexPolygon();
	}
	spin+=1.0;
	glPopMatrix();
	//glFlush();
	glutSwapBuffers();
}

void init(void) 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//glDepthFunc(GL_LEQUAL);
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

int main(int argc, char** argv)
{
	glutInit(&argc, argv);			
	glutInitDisplayMode (GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize (250, 250);		
	glutInitWindowPosition (100, 100);		
	glutCreateWindow (argv[0]);			
	init();					
	glutIdleFunc(display);			
	glutDisplayFunc(display);			
	glutReshapeFunc(reshape);			
	glutMainLoop();				
	return 0;					
}