#include <stdio.h>
#include <time.h>
#include "glut.h"

clock_t tStartTime;
float fTranslate;
float fRotate;
float fScale = 1.0f;								// set inital scale value to 1.0f

void Draw1() {
	//画1前方外边框，边厚度0.2，长度1, 高度为2, 前后厚度0.2
	glBegin(GL_LINE_LOOP);

	glVertex3f(-0.1, -1, -0.1);
	glVertex3f(-0.1, 1, -0.1);
	glVertex3f(0.1, 1, -0.1);
	glVertex3f(0.1, -1, -0.1);

	glEnd();
	

	//画1后方外边框
	glBegin(GL_LINE_LOOP);

	glVertex3f(-0.1, -1, 0.1);
	glVertex3f(-0.1, 1, 0.1);
	glVertex3f(0.1, 1, 0.1);
	glVertex3f(0.1, -1, 0.1);

	glEnd();

	//用线段连接内外边框
	glBegin(GL_LINES);

	glVertex3f(-0.1, -1, -0.1);
	glVertex3f(-0.1, -1, 0.1);

	glVertex3f(-0.1, 1, -0.1);
	glVertex3f(-0.1, 1, 0.1);

	glVertex3f(0.1, 1, -0.1);
	glVertex3f(0.1, 1, 0.1);

	glVertex3f(0.1, -1, -0.1);
	glVertex3f(0.1, -1, 0.1);

	glEnd();

}


void Draw9() {
	//画9前方外边框，宽度0.2，高度为1
	glBegin(GL_LINE_LOOP);

	glVertex3f(-0.5, -1, 0.1);
	glVertex3f(-0.5, -0.8, 0.1);
	glVertex3f(0.3, -0.8, 0.1);
	glVertex3f(0.3, 0, 0.1);
	glVertex3f(-0.5, 0, 0.1);
	glVertex3f(-0.5, 1, 0.1);
	glVertex3f(0.5, 1, 0.1);
	glVertex3f(0.5, -1, 0.1);

	glEnd();

	//画9后方外边框
	glBegin(GL_LINE_LOOP);

	glVertex3f(-0.5, -1, -0.1);
	glVertex3f(-0.5, -0.8, -0.1);
	glVertex3f(0.3, -0.8, -0.1);
	glVertex3f(0.3, 0, -0.1);
	glVertex3f(-0.5, 0, -0.1);
	glVertex3f(-0.5, 1, -0.1);
	glVertex3f(0.5, 1, -0.1);
	glVertex3f(0.5, -1, -0.1);

	glEnd();


	//画9前方内边框
	glBegin(GL_LINE_LOOP);

	glVertex3f(-0.3, 0.2, -0.1);
	glVertex3f(-0.3, 0.8, -0.1);
	glVertex3f(0.3, 0.8, -0.1);
	glVertex3f(0.3, 0.2, -0.1);

	glEnd();


	//画9后方内边框
	glBegin(GL_LINE_LOOP);

	glVertex3f(-0.3, 0.2, 0.1);
	glVertex3f(-0.3, 0.8, 0.1);
	glVertex3f(0.3, 0.8, 0.1);
	glVertex3f(0.3, 0.2, 0.1);

	glEnd();


	//用线段连接前后边框
	//外边框
	glBegin(GL_LINES);
	glVertex3f(-0.5, -1, -0.1);
	glVertex3f(-0.5, -1, 0.1);

	glVertex3f(-0.5, -0.8, -0.1);
	glVertex3f(-0.5, -0.8, 0.1);

	glVertex3f(0.3, -0.8, -0.1);
	glVertex3f(0.3, -0.8, 0.1);

	glVertex3f(0.3, 0, -0.1);
	glVertex3f(0.3, 0, 0.1);

	glVertex3f(-0.5, 0, -0.1);
	glVertex3f(-0.5, 0, 0.1);

	glVertex3f(-0.5, 1, -0.1);
	glVertex3f(-0.5, 1, 0.1);

	glVertex3f(0.5, 1, -0.1);
	glVertex3f(0.5, 1, 0.1);

	glVertex3f(0.5, -1, -0.1);
	glVertex3f(0.5, -1, 0.1);


	//内边框
	glVertex3f(-0.3, 0.2, -0.1);
	glVertex3f(-0.3, 0.2, 0.1);

	glVertex3f(-0.3, 0.8, -0.1);
	glVertex3f(-0.3, 0.8, 0.1);

	glVertex3f(0.3, 0.8, -0.1);
	glVertex3f(0.3, 0.8, 0.1);

	glVertex3f(0.3, 0.2, -0.1);
	glVertex3f(0.3, 0.2, 0.1);

	glEnd();
}

void Draw6() {
	//画6前方外边框，宽度0.1，高度为1
	glBegin(GL_LINE_LOOP);

	glVertex3f(-0.5, -1, -0.1);
	glVertex3f(-0.5, 1, -0.1);
	glVertex3f(0.5, 1, -0.1);
	glVertex3f(0.5, 0.8, -0.1);
	glVertex3f(-0.3, 0.8, -0.1);
	glVertex3f(-0.3, 0, -0.1);
	glVertex3f(0.5, 0, -0.1);
	glVertex3f(0.5, -1, -0.1);

	glEnd();

	//画6后方外边框
	glBegin(GL_LINE_LOOP);

	glVertex3f(-0.5, -1, 0.1);
	glVertex3f(-0.5, 1, 0.1);
	glVertex3f(0.5, 1, 0.1);
	glVertex3f(0.5, 0.8, 0.1);
	glVertex3f(-0.3, 0.8, 0.1);
	glVertex3f(-0.3, 0, 0.1);
	glVertex3f(0.5, 0, 0.1);
	glVertex3f(0.5, -1, 0.1);

	glEnd();

	//画6前方内边框
	glBegin(GL_LINE_LOOP);

	glVertex3f(-0.3, -0.2, -0.1);
	glVertex3f(-0.3, -0.8, -0.1);
	glVertex3f(0.3, -0.8, -0.1);
	glVertex3f(0.3, -0.2, -0.1);

	glEnd();


	//画6后方内边框
	glBegin(GL_LINE_LOOP);

	glVertex3f(-0.3, -0.2, 0.1);
	glVertex3f(-0.3, -0.8, 0.1);
	glVertex3f(0.3, -0.8, 0.1);
	glVertex3f(0.3, -0.2, 0.1);

	glEnd();


	//用线段连接内外边框
	//外边框
	glBegin(GL_LINES);
	glVertex3f(0.5, 1, -0.1);
	glVertex3f(0.5, 1, 0.1);

	glVertex3f(0.5, 0.8, -0.1);
	glVertex3f(0.5, 0.8, 0.1);

	glVertex3f(-0.3, 0.8, -0.1);
	glVertex3f(-0.3, 0.8, 0.1);

	glVertex3f(-0.3, 0, -0.1);
	glVertex3f(-0.3, 0, 0.1);

	glVertex3f(0.5, 0, -0.1);
	glVertex3f(0.5, 0, 0.1);

	glVertex3f(0.5, -1, -0.1);
	glVertex3f(0.5, -1, 0.1);

	glVertex3f(-0.5, -1, -0.1);
	glVertex3f(-0.5, -1, 0.1);

	glVertex3f(-0.5, 1, -0.1);
	glVertex3f(-0.5, 1, 0.1);

	//内边框
	glVertex3f(-0.3, -0.2, -0.1);
	glVertex3f(-0.3, -0.2, 0.1);

	glVertex3f(-0.3, -0.8, -0.1);
	glVertex3f(-0.3, -0.8, 0.1);

	glVertex3f(0.3, -0.8, -0.1);
	glVertex3f(0.3, -0.8, 0.1);

	glVertex3f(0.3, -0.2, -0.1);
	glVertex3f(0.3, -0.2, 0.1);

	glEnd();
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	//Perspective view
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	// Ortho view
	//glOrtho(-4, 4, -4, 4, 0, 100);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

void redraw()
{	
	//通过经过时间计算图形的变换信息
	//以一秒为1个周期 最左边中心从-1移动至1 中间绕y轴旋转90° 左右边图形从0.5缩放至1
	clock_t duration = clock() - tStartTime;
	float sec = (float)(duration % (CLOCKS_PER_SEC))  / (CLOCKS_PER_SEC) ;
	fTranslate = -1 + sec * 2;
	fRotate = (duration / 6) % 360;
	fScale = 0.5 + 0.5 * sec;

	// 如果想使用线框模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();									// 重置变换矩阵（回到世界坐标系）

	//左边
	glPushMatrix(); // 设置一个新的坐标系
	//where you need to write 沿着y轴上下运动
	glTranslatef(0, fTranslate, 0);
	glTranslatef(-2.0f, 0.0f, -6.0f);				// 向左平移
	Draw1();
	glPopMatrix(); // 回到前一个坐标系

	//中间
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -6.0f);
	//where you need to write 以y轴为旋转轴
	glRotatef(fRotate, 0.0f, 1.0f, 0.0f);
	Draw9();
	glPopMatrix();

	//右边
	glPushMatrix();
	glTranslatef(2.0f, 0.0f, -6.0f);
	//where you need to write 等比例缩放
	glScalef(fScale, fScale, fScale);
	Draw6();
	glPopMatrix();

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	int windowHandle
		= glutCreateWindow("Simple GLUT App");

	tStartTime = clock(); //获取程序开始运行的时间

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}


