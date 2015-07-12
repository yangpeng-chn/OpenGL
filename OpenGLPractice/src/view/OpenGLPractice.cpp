#include <GL/glut.h>
#include <windows.h>
#define MAX_CHAR       128

int width = 500;
int height = 500;

GLdouble vertex[][3] = {
	{ 0.0, 0.0, 0.0 }, /* A */
	{ 1.0, 0.0, 0.0 }, /* B */
	{ 1.0, 1.0, 0.0 }, /* C */
	{ 0.0, 1.0, 0.0 }, /* D */
	{ 0.0, 0.0, 1.0 }, /* E */
	{ 1.0, 0.0, 1.0 }, /* F */
	{ 1.0, 1.0, 1.0 }, /* G */
	{ 0.0, 1.0, 1.0 }  /* H */
};

int edge[][2] = {
	{ 0, 1 }, /* ア (A-B) */
	{ 1, 2 }, /* イ (B-C) */
	{ 2, 3 }, /* ウ (C-D) */
	{ 3, 0 }, /* エ (D-A) */
	{ 4, 5 }, /* オ (E-F) */
	{ 5, 6 }, /* カ (F-G) */
	{ 6, 7 }, /* キ (G-H) */
	{ 7, 4 }, /* ク (H-E) */
	{ 0, 4 }, /* ケ (A-E) */
	{ 1, 5 }, /* コ (B-F) */
	{ 2, 6 }, /* サ (C-G) */
	{ 3, 7 }  /* シ (D-H) */
};

void drawString(const char* str) {
	//glViewport(0,0,200,200);
	static int isFirstCall = 1;
	static GLuint lists;

	if( isFirstCall ) { 

		isFirstCall = 0;

		lists = glGenLists(MAX_CHAR);

		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}

	for(; *str!='\0'; ++str)
		glCallList(lists + *str);
}

void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void reshape( int w, int h )
{
	width = w;
	height = h;
	//glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(45.0, (double)w / (double)h, 0.1, 1000);

	/*glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
	/* ウィンドウ全体をビューポートにする */
	/*glViewport(0, 0, w, h);

	/ * 変換行列の初期化 * /
	glLoadIdentity();

	/ * スクリーン上の表示領域をビューポートの大きさに比例させる * /
	glOrtho(-w / 200.0, w / 200.0, -h / 200.0, h / 200.0, -1.0, 1.0);*/

	/*glViewport(0, 0, w, h);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glTranslated(0.0, 0.0, -5.0);*/

	/*glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);*/
}



void DrawHuman()
{

}

void DrawCoor()
{
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(1,0,0);
	glVertex3f(0,0,0);
	glVertex3f(1000,0,0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0,1,0);
	glVertex3f(0,0,0);
	glVertex3f(0,1000,0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0,0,1);
	glVertex3f(0,0,0);
	glVertex3f(0,0,1000);
	glEnd();
}

void DrawObj()
{
	glRotated(25.0, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glColor3d(1.0, 0.0, 0.0); /* 赤 */
	glVertex2d(-0.9, -0.9);
	glColor3d(0.0, 1.0, 0.0); /* 緑 */
	glVertex2d(0.9, -0.9);
	glColor3d(0.0, 0.0, 1.0); /* 青 */
	glVertex2d(0.9, 0.9);
	glColor3d(1.0, 1.0, 0.0); /* 黄 */
	glVertex2d(-0.9, 0.9);
	glEnd();
}
void display(void) 
{
	selectFont(48, ANSI_CHARSET, "Comic Sans MS");
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0,0,-600);//move origin to 0,0,-600

	//view1
	glPushMatrix();
	{
		glViewport(0,height/5,width,height*4/5);
		float hei = height*4/5;
		float wid = width;

		glColor3f(0.2,0.2,0.2);
		glBegin(GL_QUADS);
		glVertex3f(-(width/2-1),-(height/2-1),0);
		glVertex3f(width/2-1,-(height/2-1),0);
		glVertex3f(width/2-1,height/2-1,0);
		glVertex3f(-(width/2-1),height/2-1,0);
		glEnd();
		DrawCoor();

		glColor3f(0.5,0.5,0);
		//glutWireCube(0.2);
		glutSolidTeapot(0.1);
		glRasterPos2f(0.0f, 0.0f);
		glColor3f(0.5,0.5,0.5);
		drawString("Coor1");
	}
	glPopMatrix();
	
	//view2
	glPushMatrix();
	{
		glViewport(0,0,width,height/5);

		glColor3f(0.8,0.8,0.8);
		glBegin(GL_LINE_LOOP);
		glVertex3f(-(width/2-1),-(height/2-1),0);
		glVertex3f(245,-(height/2-1),0);
		glVertex3f(width/2-1,240,0);
		glVertex3f(-(width/2-1),240,0);

		/*glVertex3f(-(width/2-1),-(height/2-1),0);
		glVertex3f(width/2-1,-(height/2-1),0);
		glVertex3f(width/2-1,height/2-1,0);
		glVertex3f(-(width/2-1),height/2-1,0);*/
		glEnd();

		DrawCoor();
		glRasterPos2f(0.0f, 0.0f);
		drawString("Coor2");
	}
	glPopMatrix();

	glutSwapBuffers();
}

void init (void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0); /* select clearing olor  */
	//  glMatrixMode(GL_PROJECTION); /* initialize viewing values  */
	//  glLoadIdentity();
	//  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); //注意该视景体的范围和几何中心
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	init ();
	//glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (width, height);  //改为glutInitWindowSize (250, 250); 可以看出变换的仅仅是物体按比例大小的变换
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("First");
	
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0; 
}

