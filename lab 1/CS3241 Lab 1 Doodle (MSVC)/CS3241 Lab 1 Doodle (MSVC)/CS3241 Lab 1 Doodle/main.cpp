//Name: Li Simeng
//Extra functions that I use: rand(), glLightfv()

// CS3241 Assignment 1: Doodle
#include <cmath>
#include <iostream>

#include <cstdlib> 
#include <ctime>  

#ifdef _WIN32
#include <Windows.h>
#include "GL/glut.h"
#define M_PI 3.141592654
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

GLfloat PI = 3.14;
float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;

void drawRhombus()
{
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f); // all vertices point out of XY plane (this part is suggested by ChatGPT TO enhance the lighting effect)
		glColor4f(0.4, 0.85, 0.9, 1.0);
		glVertex2f(0.0, 0.0);
		glColor4f(0.85, 0.85, 0.9, 1.0);
		glVertex2f(0.0, 2.0);

		glColor4f(0.4, 0.6, 0.8, 1.0);
		glVertex2f(1.4142, 3.4142);
		glVertex2f(1.4142, 1.4142);	
	glEnd();
}

void drawSnowFlakeBase()
{
	drawRhombus();

	for (int i =0; i<7; i++)
	{
		glRotatef(45, 0, 0, 1);
		drawRhombus();
	}
}

void drawBranch()
{
	glLineWidth(3.0);
	glBegin(GL_LINES);
		
		glColor4f(0.88, 0.9, 0.9, 1.0);
		glVertex2f(0.0, 0.0);

		glColor4f(0.5, 0.66, 0.9, 1.0);
		glVertex2f(0.0, 4.0);

		glVertex2f(0.0, 3.0);
		glVertex2f(0.7071, 3.7071);	

		glVertex2f(0.0, 3.0);
		glVertex2f(-0.7071, 3.7071);

		glVertex2f(0.0, 4.0);
		glVertex2f(0.7071, 4.7071);

		glVertex2f(0.0, 4.0);
		glVertex2f(-0.7071, 4.7071);

	glEnd();
}
	
void drawBranches()
{
	drawBranch();
	for (int i =0; i<7; i++)
	{
		glRotatef(45, 0, 0, 1);
		drawBranch();
	}
}


void drawInnerRhombus()
{
	glBegin(GL_POLYGON);
		glColor4f(0.2, 0.7, 0.9, 0.5);
		glVertex2f(0.0, 0.0);
		glVertex2f(-0.55785, 1.0);

		glColor4f(0.8, 0.9, 0.9, 0.5);
		glVertex2f(0.0, 2.0);
		glVertex2f(0.55789, 1.0);
	glEnd();
}

void drawInnerLayer()
{
	drawInnerRhombus();

	for (int i = 0; i < 7; i++)
	{
		glRotatef(45, 0, 0, 1);
		drawInnerRhombus();
	}
}

void drawSparkle()
{
	glBegin(GL_POINTS);
	for (int i = 0; i < 80; i++)
	{
		float randX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f; 
		float randY = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f; 
		glColor4f(1.0, 1.0, 1.0, 0.8);
		glVertex2f(randX, randY);
	}
	glEnd();

}

void display(void)
{
	glClearColor(0.1, 0.1, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);	
	glTranslatef(tx, ty, 0);	
	glRotatef(alpha, 0, 0, 1);
	
	//draw your stuff here (Erase the triangle code)
	glPushMatrix();
		glScalef(1.6, 1.6, 1.0);
		drawSnowFlakeBase();
	glPopMatrix();

	drawSnowFlakeBase();
	drawInnerLayer();
	drawBranches();

	glPointSize(2.0);
	drawSparkle();

	glPopMatrix();
	glFlush ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glShadeModel (GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat lightPos[] = { 5.0f, 5.0f, 5.0f, 1.0f };
	GLfloat lightAmbient[] = { 0.6f, 0.2f, 0.2f, 1.0f };
	GLfloat lightDiffuse[] = { 1.2f, 1.2f, 1.7f, 1.0f };
	GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}



void keyboard (unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {

		case 'a':
			alpha+=10;
			glutPostRedisplay();
		break;

		case 'd':
			alpha-=10;
			glutPostRedisplay();
		break;

		case 'q':
			k+=0.1;
			glutPostRedisplay();
		break;

		case 'e':
			if(k>0.1)
				k-=0.1;
			glutPostRedisplay();
		break;

		case 'z':
			tx-=0.1;
			glutPostRedisplay();
		break;

		case 'c':
			tx+=0.1;
			glutPostRedisplay();
		break;

		case 's':
			ty-=0.1;
			glutPostRedisplay();
		break;

		case 'w':
			ty+=0.1;
			glutPostRedisplay();
		break;

		default:
		break;
	}
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 1\n\n";
	cout<<"+++++CONTROL BUTTONS+++++++\n\n";
	cout<<"Scale Up/Down: Q/E\n";
	cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
	cout<<"Move Up/Down: W/S\n";
	cout<<"Move Left/Right: Z/C\n";

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
