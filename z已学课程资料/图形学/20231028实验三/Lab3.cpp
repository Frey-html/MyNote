#include <stdlib.h>
#include <gl/glut.h>

#define STEP 0.5 // set the travel distance of camera per operation

float fTranslate;
float fRotate;
float fTeapotRotate = 0.0f;
float fScale = 1.0f;	// set inital scale value to 1.0f
float teapot[] = { 0, 0, 0 };

bool bPersp = false;
bool bAnim = false;
bool bWire = false;
bool bTeaAnim = false;

int wHeight = 0;
int wWidth = 0;


//todo
//hint: some additional parameters may needed here when you operate the teapot

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void Draw_Scene()
{
	glPushMatrix();
	
	glTranslatef(teapot[0], -1 * teapot[2], 0);
	glTranslatef(0, 0, 4 + 1);
	glRotatef(90, 1, 0, 0); //notice the rotation here, you may have a TRY removing this line to see what it looks like.
	//todo; hint: when operate the teapot, you may need to change some parameters
	glRotatef(fTeapotRotate, 0, 1, 0);
	glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}

void updateView(int width, int height)
{
	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;

	if (bPersp) {
		//todo when 'p' operation, hint: use FUNCTION gluPerspective
		gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	}
	else
		glOrtho(-3, 3, -3, 3, -100, 100);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = { 0, 0, 8 };
float center[] = { 0, 0, 0 };
//todo; hint: you may need another ARRAY when you operate the teapot

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth); break; }

	case ' ': {bAnim = !bAnim; break; }
	case 'o': {bWire = !bWire; break; }

	case 'a': {//todo, hint: eye[] and center[] are the keys to solve the problems
		eye[0] -= STEP;
		break;
	}
	case 'd': {//todo
		eye[0] += STEP;
		break;
	}
	case 'w': {//todo
		eye[2] -= STEP;
		break;
	}
	case 's': {//todo
		eye[2] += STEP;
		break;
	}
	case 'z': {//todo
		eye[1] += STEP;
		break;
	}
	case 'c': {//todo
		eye[1] -= STEP;
		break;
	}

			  //�����ز���
	case 'l': {//todo, hint:use the ARRAY that you defined, and notice the teapot can NOT be moved out the range of the table.
		teapot[0] += STEP / 5;
		if (teapot[0] >= 1.5) teapot[0] = 1.5;
		break;
	}
	case 'j': {//todo
		teapot[0] -= STEP / 5;
		if (teapot[0] <= -1.5) teapot[0] = -1.5;
		break;
	}
	case 'i': {//todo
		teapot[2] -= STEP / 5;
		if (teapot[2] <= -1.5) teapot[2] = -1.5;
		break;
	}
	case 'k': {//todo
		teapot[2] += STEP / 5;
		if (teapot[2] >= 1.5) teapot[2] = 1.5;
		break;
	}
	case 'e': {//todo
		bTeaAnim = !bTeaAnim;
		break;
	}
	}
}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 5,5,5,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

	//bouns hint: you can change the light's different attribute(like color, position, characteristics, etc.) or add lights, or anything you want to explore. REMEMBER to record your process and perception in report.

//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene();						// Draw Scene

	if (bAnim) fRotate += 0.5f;

	//todo; hint: when you want to rotate the teapot, you may like to add another line here =)
	if (bTeaAnim) fTeapotRotate += 0.5f;

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


