

// CS3241 Assignment 3: Let there be light
#include <cmath>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include "GL/glut.h"
#define M_PI 3.141592654
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

// global variable

bool m_Smooth = FALSE;
bool m_Highlight = FALSE;
GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
int mouseButton = 0;
int moving, startx, starty;

#define NO_OBJECT 4;
int current_object = 0;

using namespace std;

//to adjust camera
float camPosX = 0.0f, camPosY = 0.0f, camPosZ = 10.0f;
float camCenterX = 0.0f, camCenterY = 0.0f, camCenterZ = 0.0f;
float camUpX = 0.0f, camUpY = 1.0f, camUpZ = 0.0f;

//viewing fractum
float nearPlane = 1.0f;
float farPlane = 80.0f;
float fovy = 40.0f;
float aspect = 1.0f;


void setupLighting()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	// Lights, material properties
	GLfloat	ambientProperties[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat	diffuseProperties[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat	specularProperties[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPosition[] = { -100.0f,100.0f,100.0f,1.0f };

	glClearDepth(1.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


void drawSphere(double r)
{
	glScalef(r, r, r);
	int i, j;
	int n = 20;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (i = 0; i < 2 * n; i++)
		for (j = 0; j < n; j++)
		{
			glBegin(GL_POLYGON);
			//normal at each vertex is the vector from the center of the sphere to the vertex, which is the position vector
			double x1 = sin(i * M_PI / n) * sin(j * M_PI / n);
			double y1 = cos(i * M_PI / n) * sin(j * M_PI / n);
			double z1 = cos(j * M_PI / n);
			glNormal3d(x1, y1, z1);
			glVertex3d(x1, y1, z1);

			double x2 = sin((i + 1) * M_PI / n) * sin(j * M_PI / n);
			double y2 = cos((i + 1) * M_PI / n) * sin(j * M_PI / n);
			double z2 = cos(j * M_PI / n);
			glNormal3d(x2, y2, z2);
			glVertex3d(x2, y2, z2);

			double x3 = sin((i + 1) * M_PI / n) * sin((j + 1) * M_PI / n);
			double y3 = cos((i + 1) * M_PI / n) * sin((j + 1) * M_PI / n);
			double z3 = cos((j + 1) * M_PI / n);
			glNormal3d(x3, y3, z3);
			glVertex3d(x3, y3, z3);

			double x4 = sin(i * M_PI / n) * sin((j + 1) * M_PI / n);
			double y4 = cos(i * M_PI / n) * sin((j + 1) * M_PI / n);
			double z4 = cos((j + 1) * M_PI / n);
			glNormal3d(x4, y4, z4);
			glVertex3d(x4, y4, z4);

			glEnd();
		}

}

void drawTruncatedCone(double topRadius, double baseRadius, double height)
{
	int slices = 50;
	double slope = atan((baseRadius - topRadius) / height);

	for (int i = 0; i < slices; ++i) {
		double theta1 = i * 2 * M_PI / slices;
		double theta2 = (i + 1) * 2 * M_PI / slices;

		double x1b = baseRadius * cos(theta1), z1b = baseRadius * sin(theta1);
		double x2b = baseRadius * cos(theta2), z2b = baseRadius * sin(theta2);
		double x1t = topRadius * cos(theta1), z1t = topRadius * sin(theta1);
		double x2t = topRadius * cos(theta2), z2t = topRadius * sin(theta2);

		double nx1 = cos(theta1) * cos(slope), ny = sin(slope), nz1 = sin(theta1) * cos(slope);
		double nx2 = cos(theta2) * cos(slope), nz2 = sin(theta2) * cos(slope);


		glBegin(GL_QUADS);
		glNormal3d(nx1, ny, nz1);
		glVertex3d(x1b, 0, z1b);
		glNormal3d(nx2, ny, nz2);
		glVertex3d(x2b, 0, z2b);
		glNormal3d(nx2, ny, nz2);
		glVertex3d(x2t, height, z2t);
		glNormal3d(nx1, ny, nz1);
		glVertex3d(x1t, height, z1t);
		glEnd();
	}

}


void drawFlower4()
{
	//stem
	glColor4f(0.741, 0.925, 0.714, 1.0);
	drawTruncatedCone(0.05, 0.05, 0.3);

	//body
	glPushMatrix();
	glColor4f(0.823, 0.539, 0.623, 1.0);
	glTranslatef(0, 0.3, 0);
	drawTruncatedCone(0.5, 0.3, 0.8);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0.723, 0.569, 0.623, 1.0);
	glTranslatef(0, 0.5, 0);
	drawSphere(0.4);
	glPopMatrix();

	//stamen
	glPushMatrix();
	glColor4f(0.641, 0.925, 0.714, 1.0);
	glTranslatef(0, 0.3, 0);
	drawTruncatedCone(0.05, 0.05, 1.2);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0.9, 0.837, 0.721, 1.0);
	glTranslatef(0, 1.5, 0);
	drawSphere(0.06);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0.641, 0.925, 0.714, 1.0);
	glTranslatef(0, 0.3, 0);
	glRotatef(20, 1, 0, 0);
	drawTruncatedCone(0.05, 0.05, 1.2);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0.9, 0.837, 0.721, 1.0);
	glRotatef(20, 1, 0, 0);
	glTranslatef(0, 1.5, 0);
	drawSphere(0.07);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0.641, 0.925, 0.714, 1.0);
	glTranslatef(0, 0.3, 0);
	glRotatef(-20, 1, 0, 0);
	drawTruncatedCone(0.05, 0.05, 1.2);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0.9, 0.837, 0.721, 1.0);
	glRotatef(-20, 1, 0, 0);
	glTranslatef(0, 1.5, 0);
	drawSphere(0.07);
	glPopMatrix();

}

void drawCurledStem()
{
	glRotatef(90, 0, 0, 1);
	glTranslatef(0, 0, -7);
	glPushMatrix();
	for (int i = 0; i < 15; i++)
	{
		glColor4f(0.133, 0.345, 0.133, 1.0);
		drawTruncatedCone(0.05, 0.05, 0.5);

		if(i % 2 == 0 && i > 5)
		{
			glPushMatrix();
			glRotatef(90, 0, 0, 1);
			drawFlower4();
			glPopMatrix();
		}

		glRotatef(1, 0, 0, 1);
		glTranslatef(0, 0.46, 0);
	}
	glPopMatrix();
}


void drawFlower1()
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glPushMatrix();
	//flower petals
	glColor4f(0.823, 0.569, 0.823, 1.0);
	for (int i = 0; i < 12; i++)
	{
		glRotatef(30, 1, 0, 0);
		drawTruncatedCone(0.35, 0.2, 0.9);
		glRotatef(-30, 1, 0, 0);
		glRotatef(30, 0, 0, 1);
		
	}

	//flower center
	glColor4f(0.9, 0.8, 0.0, 1.0);
	drawSphere(0.35);

	//flower stem
	glColor4f(0.133, 0.345, 0.133, 1.0);
	glRotatef(-90, 1, 0, 0);
	drawTruncatedCone(0.1, 0.1, 30);
	glPopMatrix();

}

void drawFlower2()
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glPushMatrix();
	//flower petals
	glColor4f(0.678, 0.774, 0.902, 0.8);
	for (int i = 0; i < 18; i++)
	{
		glRotatef(30, 1, 0, 0);
		drawTruncatedCone(0.2, 0.2, 0.7);
		glRotatef(-30, 1, 0, 0);
		glRotatef(20, 0, 0, 1);

	}
	glColor4f(0.678, 0.874, 0.902, 0.9);
	for(int i = 0; i < 12; i++)
	{
		drawTruncatedCone(0.2, 0.2, 0.8);
		glRotatef(30, 0, 0, 1);
	}

	//flower center
	glColor4f(0.9, 0.8, 0.5, 1.0);
	drawSphere(0.35);

	//flower stem
	glColor4f(0.133, 0.345, 0.133, 1.0);
	glRotatef(-90, 1, 0, 0);
	drawTruncatedCone(0.1, 0.1, 30);
	glPopMatrix();

}

void drawFlower3()
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glPushMatrix();
	//flower petals
	glColor4f(1.0, 0.713, 0.756, 1.0);
	for (int i = 0; i < 18; i++)
	{
		glRotatef(30, 1, 0, 0);
		drawTruncatedCone(0.3, 0.2, 0.8);
		glRotatef(-30, 1, 0, 0);
		glRotatef(20, 0, 0, 1);

	}

	//flower center
	glColor4f(0.9, 0.8, 0.5, 1.0);
	drawSphere(0.4);

	//flower stem
	glColor4f(0.133, 0.345, 0.133, 1.0);
	glRotatef(-90, 1, 0, 0);
	drawTruncatedCone(0.1, 0.1, 30);
	glPopMatrix();

}

void drawBouquet()
{
	glPushMatrix();
	drawFlower1();

	glPushMatrix();
	glTranslatef(1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -0.7);
	glRotatef(20, 0, 1, 0);
	drawFlower2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -0.8);
	glRotatef(-20, 0, 1, 0);
	drawFlower2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.8);
	glRotatef(-50, 1, 0, 0);
	drawFlower3();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.8);
	glRotatef(50, 1, 0, 0);
	drawFlower3();
	glPopMatrix();


	glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0, 0, -6);

	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glScalef(zoom, zoom, zoom);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, nearPlane, farPlane);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPosX, camPosY, camPosZ,
		camCenterX, camCenterY, camCenterZ,
		camUpX, camUpY, camUpZ);


	switch (current_object) {
	case 0:
		drawSphere(1);
		break;
	case 1:
		// draw your second primitive object here
		drawTruncatedCone(1, 0.5, 2.0);
		break;
	case 2:
		// draw your first composite object here
		//drawSparklingWater();
		glTranslatef(5, 0, 0);
		glTranslatef(0, 5, 0);
		drawCurledStem();

		break;
	case 3:
		// draw your second composite object here
		drawBouquet();
		break;
	default:
		break;
	};
	glPopMatrix();
	glutSwapBuffers();
}




void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'p':
	case 'P':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'w':
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'v':
	case 'V':
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 's':
	case 'S':
		m_Smooth = !m_Smooth;
		if (m_Smooth)
			glShadeModel(GL_SMOOTH);
		else
			glShadeModel(GL_FLAT);
		break;
	case 'h':
	case 'H':
		m_Highlight = !m_Highlight;
		if (m_Highlight)
		{
			GLfloat mat_diffuse[] = { 0.3f, 0.7f, 0.6f, 1.0f };
		    GLfloat mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
		    GLfloat mat_shininess = 20.0f;

		    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

		}
			
		else
		{
			GLfloat mat_diffuse2[] = { 0.3f, 0.7f, 0.6f, 1.0f };
			GLfloat mat_specular2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
		}	
		break;

	case '1':
	case '2':
	case '3':
	case '4':
		current_object = key - '1';
		break;


	case 'n': 
		nearPlane -= 0.1f; 
		if (nearPlane < 0.1f)
			nearPlane = 0.1f; 
		break;
	case 'N':
		nearPlane += 0.1f; 
		break;
	case 'f': 
		farPlane -= 0.5f; 
		if (farPlane <= nearPlane + 0.1f) 
			farPlane = nearPlane + 0.1f; 
		break;
	case 'F': 
		farPlane += 0.5f; 
		break;
	case 'o': 
		fovy -= 1.0f; 
		if (fovy < 10.0f) 
			fovy = 10.0f; 
		break;
	case 'O': 
		fovy += 1.0f; 
		if (fovy > 170.0f) 
			fovy = 170.0f; 
		break;
	case 'r':
		camPosX = 0.0f; camPosY = 0.0f; camPosZ = 10.0f;
		camCenterX = camCenterY = camCenterZ = 0.0f;
		camUpX = 0.0f; camUpY = 1.0f; camUpZ = 0.0f;
		nearPlane = 1.0f; farPlane = 80.0f; fovy = 40.0f;
		break;
	case 'R': 
		camPosX = 2.0f; camPosY = -6.0f; camPosZ = 10.0f;
		camCenterX = 0.0f; camCenterY = 0.0f; camCenterZ = 0.0f;
		camUpX = 0.0f; camUpY = 1.0f; camUpZ = 0.0f;
		nearPlane = 1.0f; farPlane = 50.0f; fovy = 30.0f;
		break;

	case 'Q':
	case 'q':
		exit(0);
		break;

	default:
		break;
	}

	glutPostRedisplay();
}



void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		mouseButton = button;
		moving = 1;
		startx = x;
		starty = y;
	}
	if (state == GLUT_UP) {
		mouseButton = button;
		moving = 0;
	}
}

void motion(int x, int y)
{
	if (moving) {
		if (mouseButton == GLUT_LEFT_BUTTON)
		{
			angle = angle + (x - startx);
			angle2 = angle2 + (y - starty);
		}
		else zoom += ((y - starty) * 0.001);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}

}

int main(int argc, char** argv)
{
	cout << "CS3241 Lab 3" << endl << endl;

	cout << "1-4: Draw different objects" << endl;
	cout << "S: Toggle Smooth Shading" << endl;
	cout << "H: Toggle Highlight" << endl;
	cout << "W: Draw Wireframe" << endl;
	cout << "P: Draw Polygon" << endl;
	cout << "V: Draw Vertices" << endl;
	cout << "Q: Quit" << endl << endl;

	cout << "Left mouse click and drag: rotate the object" << endl;
	cout << "Right mouse click and drag: zooming" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("CS3241 Assignment 3");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	setupLighting();
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glMatrixMode(GL_PROJECTION);
	gluPerspective( /* field of view in degree */ 40.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 1.0, /* Z far */ 80.0);
	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();

	return 0;
}
