#include "glut.h"

float fTranslate;
float fRotate;
float fScale = 1.0f;								// set inital scale value to 1.0f

//void Draw_Triangle()									// This function draws a triangle with RGB colors
//{
//	glBegin(GL_TRIANGLES);
//	// 设置顶点颜色
//	glColor3f(1.0f, 0.0f, 0.0f);
//	// 设置顶点坐标
//	glVertex3f(-1.0f, 1.0f, 0.0f);
//
//	glColor3f(0.0f, 1.0f, 0.0f);
//	glVertex3f(-1.0f, -1.0f, 0.0f);
//	
//	glColor3f(0.0f, 0.0f, 1.0f);
//	glVertex3f(1.0f, -1.0f, 0.0f);
//
//	glColor3f(1.0f, 0.0f, 0.0f);
//	glVertex3f(-1.0f, 1.0f, 0.0f);
//
//	glColor3f(0.0f, 0.0f, 1.0f);
//	glVertex3f(1.0f, -1.0f, 0.0f);
//
//	glColor3f(0.0f, 1.0f, 0.0f);
//	glVertex3f(1.0f, 1.0f, 0.0f);
//	
//	glEnd();
//}

//画三维图形 
void Draw() 
{
	glutWireCube(1.0);
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
	// Perspective view
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
	// 如果想使用线框模式
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();									// 重置变换矩阵（回到世界坐标系）

	//左边
	glPushMatrix(); // 设置一个新的坐标系
	glTranslatef(-2.0f, 0.0f, -6.0f);				// 向左平移
	//where you need to write 沿着y轴上下运动
	Draw();
	glPopMatrix(); // 回到前一个坐标系

	//中间
	glPushMatrix();
	//glRotatef(19.198f, 0.0f, 1.0f, 0.0f); // 绕轴（0, 1, 0)旋转19.198°（角度制）
	glTranslatef(0.0f, 0.0f, -6.0f);
	//where you need to write 以y轴为旋转轴
	Draw();
	glPopMatrix();

	//右边
	glPushMatrix();
	glTranslatef(2.0f, 0.0f, -6.0f);
	//where you need to write 等比例缩放
	Draw();
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

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}


