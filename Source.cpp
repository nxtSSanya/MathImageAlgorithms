//#pragma comment(linker, "/STACK:36777216")
#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "Glaux.lib")
#include <cstdlib>
#include <iostream>
#include "glut.h"
#include "glaux.h"
#include <gl/GLU.h>
#include <stdlib.h>
#include <cmath>
#include <windows.h>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

int a = 0, n = 500;
double ro1, ro2;
float b = 0;
std::vector<std::vector<double> > x_past(1000, std::vector<double>(1000, 0)), x_current(1000, std::vector<double>(1000, 0)),
n_x(1000, std::vector<double>(1000, 0)), x(1000, std::vector<double>(1000, 0)), ksi(1000, std::vector<double>(1000, 0)), 
temp_exp(1000, std::vector<double>(1000, 0));
std::vector<double> R(1000, 0), r(1000, 0);

double getMin(const std::vector<std::vector<double> >& vec) {
	double result = 1 << 20;
	for (int i = 0; i < 750; i++)
		for (int j = 0; j < 750; j++)
			if (vec[i][j] < result) result = vec[i][j];
	return result;
}

void standartization(std::vector<std::vector< double > > &m)
{
	int i, j;
	double max, min;
	max = 0;
	min = 0;
	for (i = 0; i < 750; i++)
		for (j = 0; j < 750; j++)
			if (m[i][j] < min) min = m[i][j];

	for (i = 0; i < 750; i++)
		for (j = 0; j < 750; j++)
			m[i][j] += abs(min);

	for (i = 0; i < 750; i++)
		for (j = 0; j < 750; j++)
			if (m[i][j] > max) max = m[i][j];


	for (i = 0; i < 750; i++)
		for (j = 0; j < 750; j++)
			n_x[i][j] = m[i][j] / max;
}

void AlgorithmHabibi(){
	int i, j, k = 0, l, h = 0, g = 0;;
	double ksi_l[100], sigm, ro, t, max = 0;
	//ro1 = 0.9;
	//ro2 = 0.9;
	sigm = 1;

	double sum, temp;
	srand((unsigned)(time(NULL)));
	for (i = 0; i < 750; i++)
		for (j = 0; j < 750; j++)
		{
			sum = 0;
			for (k = 0; k < 12; k++)
			{
				temp = rand() % 100;
				sum = sum + temp / 100;
			}

			ksi[i][j] = sum - 6;


		}
	x[0][0] = sigm * ksi[0][0];
	for (i = 1; i < 750; i++)
		x[i][0] = ro1 * x[i - 1][0] + sigm * ksi[i][0] * sqrt(1 - ro1 * ro1);
	for (j = 1; j < 750; j++)
		x[0][j] = ro2 * x[0][j - 1] + sigm * ksi[0][j] * sqrt(1 - ro2 * ro2);


	for (i = 1; i < 750; i++)
		for (j = 1; j < 750; j++)
			x[i][j] = ro1 * x[i - 1][j] + ro2 * x[i][j - 1] - ro1 * ro2 * x[i - 1][j - 1] + sigm * ksi[i][j] * sqrt((1 - ro1 * ro1) * (1 - ro2 * ro2));

	standartization(x);

}

void display()
{
	AlgorithmHabibi();
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	glLineWidth(2);
	glPointSize(800);
	int i, j, k = 0;
	for (i = 0; i < 750; i++)
		for (j = 0; j < 750; j++)
		{
			glBegin(GL_POINTS);
			glColor3f(n_x[i][j], n_x[i][j], n_x[i][j]);
			glVertex2f(0.006 * i, 0.006 * j);
			glEnd();
		};
	glutSwapBuffers();
}

void timer(int = 0)
{
	++a;
	++b;
	display();
	glutTimerFunc(5000, timer, 0);
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-0.992, 0.998, -0.992, 0.998);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	cout << "ro1: ";
	cin >> ro1;
	cout << "ro2: ";
	cin >> ro2;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(750, 750);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Lab2");
	glClearColor(0, 0, 0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100, 100, -100, 100, -100, 100);
	gluPerspective(45, 1, 0, 100);
	glutReshapeFunc(Reshape);
	glutDisplayFunc(display);
	timer();
	glutMainLoop();
}
