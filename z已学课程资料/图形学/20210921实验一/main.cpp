/*
	@ Course: Expriment One, 2021 Computer Graphics Course, Hongxin Zhang, Zhejiang University
	@ Author: Shanchen Zou, State Key Laboratory of CAD & CG
	@ Only for Experiment Course
*/

#include <gl/glut.h>

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	//绘制方法1
	glColor3f(0.0f, 1.0f, 0.0f);
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	
	//绘制方法2
	glBegin(GL_POLYGON);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.2f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.2f, 0.2f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.0f, 0.2f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
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