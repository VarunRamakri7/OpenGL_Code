/*
VISUAL STUDIO WARNING:
If there are any "Cannot open source file xxx.h" errors,
right click on the project and change the Windows 10 SDK to whatever option works.
*/

#include <stdio.h> // Change SDK if this is underlined in red
#include <math.h>

#include "GL/freeglut.h"

#define WINDOW_SIZE 500.0
#define PI 3.141517

// Global variables
GLfloat aspect;
double fov = 45.0; // Field of vision of camera
double eyeCenter[3] = { 0.0, 1.0, 5.0 }; // Coordinates of eyeCenter
double eyeLooking[2] = { 0.0, 0.0 }; // Camera's pointing camera
float cameraAngle = 0.0f;
int SPEED = 1; // Cart movement speed
int trackSize = 0; // Length of Track
float trackRotate = 0.0f; // Track rotation about X-Axis
int mouseCoord[2] = { 0, 0 }; // Coordinates of mouse
int oldMouse[2] = { 0, 0 }; // Previous mouse coordinates
bool isWarped = false;
float rotateX = 0.0f;
float rotateY = 0.0f;
float deltaX = 0.0f;
float deltaY = 0.0f;
int pillarGap = 4; // Gap between pillars

// Variables for ground color
const float GREEN[3] = { 0.7f, 0.8f, 0.5f };
const float BROWN[3] = { 0.8f, 0.7f, 0.5f };
float groundColor[3] = { 0.7f, 0.8f, 0.5f };

static int submenu;
static int fovMenu;
static int speedMenu;
static int pillarMenu;
static int colorMenu;

void changeColor(int num)
{
	switch (num)
	{
		case 0: groundColor[0] = GREEN[0];
				groundColor[1] = GREEN[1];
				groundColor[2] = GREEN[2];
				break;

		case 1: groundColor[0] = BROWN[0];
				groundColor[1] = BROWN[1];
				groundColor[2] = BROWN[2];
				break;
	}

	glutPostRedisplay();
}

void changeGap(int num)
{
	switch (num)
	{
		case 0: pillarGap = 4;
			break;

		case 1: pillarGap = 8;
			break;

		case 2: pillarGap = 10;
			break;
	}

	glutPostRedisplay();
}

void changeSpeed(int num)
{
	switch (num)
	{
		case 0: SPEED = 1;
			break;

		case 1: SPEED = 4;
			break;

		case 2: SPEED = 6;
			break;
	}

	glutPostRedisplay();
}

void changeFOV(int num)
{
	switch (num)
	{
		case 0: fov = 45.0;
			break;

		case 1: fov = 60.0;
			break;

		case 2: fov = 75.0;
			break;
	}

	glutPostRedisplay();
}

void menu(int num) // num: id of the added item
{
	switch (num)
	{
		// Reset the scene
		case 0:
			break;

			// Exit the program
		case 1: exit(0);
			break;
	}

	glutPostRedisplay();
}

void createMenu(void)
{
	colorMenu = glutCreateMenu(changeColor);
	glutAddMenuEntry("Green", 0);
	glutAddMenuEntry("Brown", 1);

	pillarMenu = glutCreateMenu(changeGap);
	glutAddMenuEntry("4", 0);
	glutAddMenuEntry("8", 1);
	glutAddMenuEntry("10", 2);

	speedMenu = glutCreateMenu(changeSpeed);
	glutAddMenuEntry("5 sq/s", 0);
	glutAddMenuEntry("10 sq/s", 1);
	glutAddMenuEntry("15 sq/s", 2);

	fovMenu = glutCreateMenu(changeFOV);
	glutAddMenuEntry("45", 0);
	glutAddMenuEntry("60", 1);
	glutAddMenuEntry("75", 2);

	submenu = glutCreateMenu(menu);  // create menu, assign a callback
	glutAddSubMenu(" FOV ", fovMenu);
	glutAddSubMenu(" Cart Speed ", speedMenu);
	glutAddSubMenu(" Pillar Gap ", pillarMenu);
	glutAddSubMenu(" Ground Color ", colorMenu);
	glutAddMenuEntry("Exit", 1);

	glutCreateMenu(menu);
	glutAddSubMenu(" Menu ", submenu);  // add submenus to the menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);  // bind to the event: clicking the right button
}

// Draw a brown ground for the tracks
void drawGround()
{
	glPushMatrix();
		glColor3f(groundColor[0], groundColor[1], groundColor[2]);
		glTranslatef(0.0f, -5.0f, -7.0f);
		glScalef(200.0f, 1.0f, 1000.0f);
		glutSolidCube(0.5);
	glPopMatrix();
}

// Draw pillars beside track
void drawPillars(int isRight, int zInc)
{
	// Draw pillar
	glPushMatrix();
		glColor3f(1.0f, 0.6f, 0.6f);
		glTranslatef(-2.0f * isRight, 4.0f, -1.0f - pillarGap * zInc);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glutSolidCylinder(0.5, 5.0, 10, 10);
	glPopMatrix();
}

// Draw an arc of a circle with center at (x, y, z) and an arc angle
void drawCurveTrack(float centerX, float centerY, float centerZ, float radius, float angle, int segments)
{
	angle = PI / 3.5f;

	glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
			glVertex3f(centerX, centerY, centerZ);
			for (int i = 0; i <= segments; i++) 
			{
				float currentAngle = angle * i / segments;
				glVertex3f(centerX + cos(currentAngle) * centerX, centerY, -1 * (centerZ + sin(currentAngle) * centerZ));
			}
		glEnd();
	glPopMatrix();
}

// Draw the straight/level track with bars across
void levelTrack(int size, int gap)
{
	// Set track color
	glColor3f(0.9f, 0.4f, 0.0f);

	// Left track
	glPushMatrix();
		glTranslatef(-0.5f, -0.3f, - 1 * (size / 10.0f));
		glScalef(0.5f, 1.0f, -3.0f * size);
		glutSolidCube(0.2);
	glPopMatrix();

	// Right track
	glPushMatrix();
		glTranslatef(0.5f, -0.3f, - 1 * (size / 10.0f));
		glScalef(0.5f, 1.0f, -3.0f * size);
		glutSolidCube(0.2);
	glPopMatrix();

	// Draw one square section curLength times
	for (int i = 0; i < gap; i++)
	{
		// Bar across tracks
		glPushMatrix();
			glTranslatef(0.0f, -0.3f, - i * 2.1f);
			glScalef(6.0f, 1.0f, 1.0f);
			glutSolidCube(0.2);
		glPopMatrix();

		// Draw pillars
		if (i < gap / 2)
		{
			drawPillars(1, i); // Left row
			drawPillars(-1, i); // Right row
		}
	}
}

void drawTracks()
{
	glPushMatrix();

		// Level track
		glPushMatrix();
			levelTrack(100, 20); // Draw straigt track of length 100
		glPopMatrix();

		// Downward curved track
		glPushMatrix();
			glPushMatrix();
				glTranslatef(-0.4f, 0.0f, -40.0f);
				glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
				drawCurveTrack(0.3f, 0.0f, 0.3f, 20, 30, 200); // Draw the left curved track with radius 10 and angle 30
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.4f, 0.0f, -40.0f);
				glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
				drawCurveTrack(0.3f, 0.0f, 0.3f, 20, 30, 200); // Draw the right curved track with radius 10 and angle 30
			glPopMatrix();
		glPopMatrix();

		// Downwards sloping track
		glPushMatrix();
			glTranslatef(0.0f, -1.5f, -43.5f);
			glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
			levelTrack(10, 3);
		glPopMatrix();

		// Upward curved track
		glPushMatrix();
			glPushMatrix();
				glTranslatef(-0.4f, -2.5f, -45.0f);
				glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
				glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
				drawCurveTrack(0.3f, 0.0f, 0.3f, 20, 30, 200); // Draw the left curved track with radius 10 and angle 30
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.4f, -2.5f, -45.0f);
				glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
				glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
				drawCurveTrack(0.3f, 0.0f, 0.3f, 20, 30, 200); // Draw the right curved track with radius 10 and angle 30
			glPopMatrix();
		glPopMatrix();

		// Upward curved track
		glPushMatrix();
			glPushMatrix();
				glTranslatef(-0.4f, -2.2f, -45.5f);
				glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
				glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
				drawCurveTrack(0.3f, 0.0f, 0.3f, 20, 30, 200); // Draw the left curved track with radius 10 and angle 30
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.4f, -2.2f, -45.5f);
				glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
				glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
				drawCurveTrack(0.3f, 0.0f, 0.3f, 20, 30, 200); // Draw the right curved track with radius 10 and angle 30
			glPopMatrix();
		glPopMatrix();

		// Upward sloping track
		glPushMatrix();
			glTranslatef(0.0f, -1.0f, -46.0f);
			glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
			levelTrack(10, 3);
		glPopMatrix();

		// Downward curved track
		glPushMatrix();
			glPushMatrix();
				glTranslatef(-0.4f, 0.5f, -49.5f);
				glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
				drawCurveTrack(0.3f, 0.0f, 0.3f, 25, 30, 200); // Draw the left curved track with radius 10 and angle 30
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.4f, 0.5f, -49.5f);
				glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
				drawCurveTrack(0.3f, 0.0f, 0.3f, 25, 30, 200); // Draw the right curved track with radius 10 and angle 30
			glPopMatrix();
		glPopMatrix();

		// Level track
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -50.0f);
			levelTrack(100, 20);// Draw straigt track of length 100
		glPopMatrix();

	glPopMatrix();
}

void display()
{
	glClearColor(0.5f, 0.9f, 0.9f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Update camera fov and position
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, 0.01, 1000.0);

	gluLookAt(eyeCenter[0], eyeCenter[1], eyeCenter[2], rotateX, rotateY, -1000.0, 0.0, 1.0, 0.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// Draw Ground
	drawGround();

	// Draw Track
	drawTracks();

	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}

// Animate the camera while idle
void idle()
{
	if (eyeCenter[2] > -38.0f)
	{
		eyeCenter[2] -= 0.1 * SPEED;
	}

	// Move camera along Y-Axis
	if (eyeCenter[2] < -38.0f)
	{
		eyeCenter[2] -= 0.2 * SPEED;

		// Move along different sections of the track
		if (eyeCenter[2] > -42.0f)
		{
			// Move along first downward curve
			eyeCenter[1] -= 0.1 * SPEED;
		}
		else if (eyeCenter[2] > -48.0f)
		{
			// Move along upward slope
			eyeCenter[1] += 0.1 * SPEED;
		}
		else if (eyeCenter[2] > 50.0f)
		{
			// Move along upward curves
			eyeCenter[1] -= 0.1 * SPEED;
		}
		else
		{
			if (eyeCenter[1] != 1.0f)
			{
				// Move along second downward curve
				eyeCenter[1] -= 0.1 * SPEED;
			}
		}
	}

	if (eyeCenter[2] < -60)
	{
		// Reset camera's position
		eyeCenter[2] = 5.0f;
		eyeCenter[1] = 1.0f;
	}

	glutPostRedisplay();
}

// Change point that the camera looks at
void rotateMouse(int x, int y)
{
	if (!isWarped)
	{
		deltaY = x - oldMouse[0];
		deltaX = -(y - oldMouse[1]);

		//keep camera from rotating beyond vertically up/down
		if (rotateX >= -90.0f && deltaY < 0.0f)
		{
			rotateX = rotateX + 2.0f * deltaY;
		}
		else if (rotateX <= 90.0f && deltaY > 0.0f)
		{
			rotateX = rotateX + 2.0f * deltaY;
		}

		rotateY = rotateY + 2.0f * deltaX;
		isWarped = true;
	}
	else
	{
		isWarped = false;
	}

	glutPostRedisplay();
}

// Mouse event callback function
void mouse(int button, int state, int x, int y)
{
	// Update previous coordinates
	oldMouse[0] = mouseCoord[0];
	oldMouse[1] = mouseCoord[1];

	// Update mouse positions
	mouseCoord[0] = x;
	mouseCoord[1] = y;

	//rotateMouse(x, y);

	glutPostRedisplay();
}

// Mouse motion callback function
void motion(int x, int y)
{
	// Update previous coordinates
	oldMouse[0] = mouseCoord[0];
	oldMouse[1] = mouseCoord[1];

	// Update mouse positions
	mouseCoord[0] = x;
	mouseCoord[1] = y;

	rotateMouse(x, y);

	glutPostRedisplay();
}

// Passive mouse movement function
void passive(int x, int y)
{
	// Update previous coordinates
	oldMouse[0] = mouseCoord[0];
	oldMouse[1] = mouseCoord[1];

	// Update mouse positions
	mouseCoord[0] = x;
	mouseCoord[1] = y;
}

// GLUT reshape function
void reshape(int w, int h)
{
	aspect = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, 0.01, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeCenter[0], eyeCenter[1], eyeCenter[2], eyeLooking[0], eyeLooking[1], -1000.0, 0.0, 1.0, 0.0);
}

// Setup the lights for the scene
void setLight()
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 25.0 };
	glClearColor(0.6f, 0.4f, 0.4f, 0.0f);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 0, 10, 5, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);  // define the position of the light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);  // specify the diffuse RGBA intensity of the light
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
}

// Initialise window
void init()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);  // define size of the window
	glutInitWindowPosition(0, 0);  // define position of the window
	glutCreateWindow("Rollercoaster");
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  // initialize the library
	init();
	setLight();

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);

	// All callback functions
	glutDisplayFunc(display);  // display callback
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(passive);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutWarpPointer(oldMouse[0], oldMouse[1]);
	glutIdleFunc(idle);

	createMenu();

	glutMainLoop();  // enter event processing loop

	return 0;
}