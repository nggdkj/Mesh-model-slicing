#include "ObjLoader.h"
#include<iostream>
using namespace std;

string filePath = "data/monkey.obj";
ObjLoader objModel = ObjLoader(filePath);
static double c = 3.1415926 / 180.0f;
static double r = 1.0f;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;

int layer_scan = 0;
int num_of_layers = 101;  //切割的总层数
string mode;

//安置光源
void setLightRes() {
	GLfloat lightPosition[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING); //启动光源
	glEnable(GL_LIGHT0);  //使用指定光
}

//初始化
void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("ObjLoader");
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	setLightRes();
	glEnable(GL_DEPTH_TEST);
}

//扫描模式
void display_scan()
{
	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);
	setLightRes();
	glPushMatrix();
	gluLookAt(r * cos(c * degree), 0, r * sin(c * degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	objModel.slice(num_of_layers, layer_scan);
	if (layer_scan < num_of_layers-1) layer_scan++;
	glPopMatrix();
	glutSwapBuffers();
}

//显示模型模式
void display_model()
{
	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);
	setLightRes();
	glPushMatrix();
	gluLookAt(r * cos(c * degree), 0, r * sin(c * degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	objModel.Draw();
	glPopMatrix();
	glutSwapBuffers();
}

//选择层次模式
void display_select()
{
	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);
	setLightRes();
	glPushMatrix();
	gluLookAt(r * cos(c * degree), 0, r * sin(c * degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	int layer;
	cout << "Enter the number of layers you want to view: ";
	cin >> layer;
	objModel.slice(num_of_layers, layer);
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
}

void moseMove(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		oldPosX = x; oldPosY = y;
	}
}

void changeViewPoint(int x, int y)
{
	int temp = x - oldPosX;
	degree += temp;
	oldPosX = x;
	oldPosY = y;
}

void myIdle()
{
	glutPostRedisplay();  //重绘
}

int main(int argc, char* argv[])
{
	cout << "Choose the mode from 3Dmodel/scan/select: ";
	cin >> mode;
	if (mode == "scan")
	{
		for (int i = 0; i < num_of_layers-1; i++)
		{
			glutInit(&argc, argv);
			init();
			glutDisplayFunc(display_scan);  //重绘
			glutReshapeFunc(reshape);  //改变窗口大小时保持图形比例
			glutMouseFunc(moseMove);
			glutMotionFunc(changeViewPoint);  //鼠标按下并且移动
			glutIdleFunc(myIdle);
			glutMainLoop();
			return 0;
		}
	}
	else if (mode == "3Dmodel")
	{
		glutInit(&argc, argv);
		init();
		glutDisplayFunc(display_model);
		glutReshapeFunc(reshape);
		glutMouseFunc(moseMove);
		glutMotionFunc(changeViewPoint);
		glutIdleFunc(myIdle);
		glutMainLoop();
		return 0;
	}
	else if (mode == "select")
	{
		glutInit(&argc, argv);
		init();
		glutDisplayFunc(display_select);
		glutReshapeFunc(reshape);
		glutMouseFunc(moseMove);
		glutMotionFunc(changeViewPoint);
		glutIdleFunc(myIdle);
		glutMainLoop();
		return 0;
	}
}
