/*
VISUAL STUDIO WARNING:
If there are any "Cannot open source file xxx.h" errors,
right click on the project and change the Windows 10 SDK to whatever option works.
*/

#include <stdio.h> // Change SDK if this is underlined in red

#include "GL/freeglut.h"

#define NORM_CONST 0.001f

float prevX = 0.0f; // Difference in X position of mouse
float prevY = 0.0f; // Difference in Y position of mouse
float deltaX = 0.0f; // Difference in X position of mouse
float deltaY = 0.0f; // Difference in Y position of mouse
float angleX = 0.0f; // X angle to rotate by
float angleY = 0.0f; // Y angle to rotate by
float diffX = 0.0f; // X difference between new location and old while moving
float diffY = 0.0f; // Y difference between new locations and old while moving
bool mouseLeftDown = false;
bool shiftDown = false;
bool reset = false;
static int submenu;

void menu(int num) // num: id of the added item
{
	switch (num)
	{
		// Reset the scene
	case 0: reset = true;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();  // reset the current matrix to identity matrix
		glutSolidTeapot(0.5);
		printf("Clear Screen\n");
		break;

		// Exit the program
	case 1: exit(0);
		break;
	}

	glutPostRedisplay();
}

void createMenu(void) 
{
	submenu = glutCreateMenu(menu);  // create menu, assign a callback
	glutAddMenuEntry("Clear", 0);
	glutAddMenuEntry("Exit", 1);

	glutCreateMenu(menu);
	glutAddSubMenu(" Menu ", submenu);  // add submenus to the menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);  // bind to the event: clicking the right button
}

void drawTeapot(float delX, float delY, float angX, float angY) {
	// Complete transformations
	if (!reset) {
		glPushMatrix();
			glTranslatef(delX * NORM_CONST, -delY * NORM_CONST, 0.0f);
			glRotatef(angX, 1.0f, 0.0f, 0.0f);
			glRotatef(angY, 0.0f, 1.0f, 0.0f);
			glutSolidTeapot(0.5);
		glPopMatrix();
	}
	else {
		// Reset values if player clears scene
		deltaX = 0.0f;
		deltaY = 0.0f;
		angleX = 0.0f;
		angleY = 0.0f;
		reset = false;
	}
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();  // reset the current matrix to identity matrix

	float eye[] = { 0, 0, 8 };
	float center[] = { 0, 0, 0 };
	// create a viewing matrix defined by an eye point and a reference point
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 5, 5, 5, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);  // define the position of the light
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);  // specify the ambient RGBA intensity of the light
	glEnable(GL_LIGHT0);

	// Draw Teapot
	drawTeapot(deltaX, deltaY, angleX, angleY);

	glutSwapBuffers();
}

void idle()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();  // reset the current matrix to identity matrix

	glutSolidTeapot(0.5);

	glutPostRedisplay();
}

// Mouse event callback function
void mouse(int button, int state, int x, int y)
{
	// Check if mouse left button is clicked and respond accordingly
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		mouseLeftDown = true;

		diffX = x - angleY;
		diffY = -y + angleX;
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
	{
		mouseLeftDown = false;
	}

	if (button == GLUT_RIGHT_BUTTON) 
	{
		createMenu();
	}

	printf("Differences: (%f, %f)\n", diffX, diffY);

	glutPostRedisplay();
}

// Mouse motion callback function
void motion(int x, int y)
{
	// Get the coordinates of the mouse moving
	if (mouseLeftDown)
	{
		// Multiple by -1 to change the orientation
		angleY = -x + diffX;
		angleX = -y - diffY;

		// Get difference in position if the mouse moves
		if (shiftDown) {
			// Updates positition values
			deltaX = x + prevX;
			deltaY = y + prevY;
			printf("Deltas: (%f, %f)\n", deltaX, deltaY);
		}
	}

	printf("motion (%d, %d)\n", x, y);
	printf("Angles: (%f, %f)\n", angleX, angleY);

	// Update the previous coordinates
	prevX = (float)x;
	prevY = (float)y;

	glutPostRedisplay();
}

// Tracks keyboard pressed
void keyboardPress(unsigned char key, int x, int y)
{
	// GLUT recognizes modifier keys only if another keyboard button is pressed
	// Press SHIFT first and then another non-special key
	int mod = glutGetModifiers();
	if (mod == GLUT_ACTIVE_SHIFT)
	{
		shiftDown = true;
		printf("Shift pressed\n");
	}
	else 
	{
		shiftDown = false;
	}

	printf("keyboard click: \"%c\" at (%d, %d)\n", key, x, y);

	glutPostRedisplay();
}

// GLUT reshape function
void reshape(int width, int height) 
{
	glViewport(0, 0, width, height);
}

// Initialise window
void init()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);  // define size of the window
	glutInitWindowPosition(0, 0);  // define position of the window
	glutCreateWindow("Hello teapot - Rotate and Translate");
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  // initialize the library
	init();

	glutDisplayFunc(display);  // display callback

	glutMouseFunc(mouse); // Set mouse motion callback function
	glutMotionFunc(motion); // Set mouse passive motion callback function
	glutKeyboardFunc(keyboardPress); // Set keyboard function
	//glutIdleFunc(idle);
	glutReshapeFunc(reshape);

	glViewport(0, 0, 480, 480);  // define a viewport and display in this viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(0.0, 1, 1.0, 20.0);  // specify a viewing frustum

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  // define a viewing matrix

	glutMainLoop();  // enter event processing loop

	return 0;
}
