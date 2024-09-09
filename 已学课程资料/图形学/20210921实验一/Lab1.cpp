/*
	@ Course: Expriment One, 2021 Computer Graphics Course, Hongxin Zhang, Zhejiang University
	@ Author: Shanchen Zou, State Key Laboratory of CAD & CG
	@ Only for Experiment Course
*/

#include <gl/glut.h>

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//绘制背景
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectf(-1.0f, -1.0f, 1.0f, 1.0f);

	//定义字体高1.0f，也即显示高度的一半，长0.5f，绘制线条粗细0.1f, 边间距0.15f
	
	//绘制数字1
	glBegin(GL_QUADS);
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-0.5f, -0.5f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(-0.4f, -0.5f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-0.4f, 0.5f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.5f, 0.5f);
	}
	glEnd();

	//绘制数字9
	glBegin(GL_QUADS);
	{
		//9的下底边
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-0.25f, -0.5f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.25f, -0.5f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.25f, -0.4f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.25f, -0.4f);

		//9的中底边
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-0.25f, 0.4f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.25f, 0.4f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.25f, 0.5f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.25f, 0.5f);


		//9的上底边
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-0.25f, -0.05f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.25f, -0.05f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.25f, 0.05f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.25f, 0.05f);


		//9的左侧边
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-0.25f, -0.05f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(-0.15f, -0.05f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-0.15f, 0.5f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.25f, 0.5f);


		//9的右侧边
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.15f, -0.5f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.25f, -0.5f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.25f, 0.5f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.15f, 0.5f);
	}
	glEnd();

	//绘制数字6
	glBegin(GL_QUADS);
	{
		//6的下底边
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.4f, -0.5f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.9f, -0.5f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.9f, -0.4f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.4f, -0.4f);

		//6的上底边
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.4f, 0.4f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.9f, 0.4f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.9f, 0.5f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.4f, 0.5f);


		//6的中底边
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.4f, -0.05f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.9f, -0.05f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.9f, 0.05f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.4f, 0.05f);


		//6的左侧边
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.4f, -0.5f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.5f, -0.5f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.5f, 0.5f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.4f, 0.5f);


		//6的右侧边
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.8f, -0.5f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.9f, -0.5f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.9f, 0.05f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.8f, 0.05f);
	}
	glEnd();

	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);//初始化glut窗口
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //设置显示模式
	glutInitWindowPosition(100, 100);//设置窗口的左上角在屏幕上的位置
	glutInitWindowSize(640, 480);//设置窗口的大小
	glutCreateWindow("Experiment_1");//创建标题为“Experiment_1”的窗口
	glutDisplayFunc(display);//注册一个名为display的绘图函数
	glutMainLoop();//进入GLUT事件处理循环，处理事件，如键盘事件、鼠标事件等
}