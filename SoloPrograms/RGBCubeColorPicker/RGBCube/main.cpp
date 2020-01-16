/*
VISUAL STUDIO WARNING:
If there are any "Cannot open source file xxx.h" errors,
right click on the project and change the Windows 10 SDK to whatever option works.
*/

#include <iostream>
#include <stdio.h> // Change SDK if this is underlined in red

#include "GL/freeglut.h"

#define GRAY_INTENSITY 0.01f

enum State {SCALE_FIXED, SCALE_VARIABLE, SLIDER};

State currentState[2] = { static_cast<State>(0), static_cast<State>(2) };

// Windows
int mainWindow; // Main window
int colorWindow; // colorWindow
float sliderScale = 510.0f; // Slider length
bool isVariable = false;

// HSV Color values
float hue;
float sat;
float cVal;

// Window Title
char windowTitle[100] = "RGB Cube | RGB =(0.000, 0.000, 0.000) HSV = (0.000, 0.000, 0.000)";

// Picked color showed in slider
unsigned char pixel[4]; // Store pixel color
float intensity = 0.0f;

// Transformation of cube
float rotateY = -45.0f;
float rotateX = -45.0f;
float scale = 1.0f;

static int submenu;

void menu(int num) // num: id of the added item
{
	glutPostRedisplay();
}

// Menu for changing viewing
void viewMenu(int num)
{
	// Update cube's rotation values
	switch (num)
	{
	case 0: // +++
		rotateX = -45.0f;
		rotateY = -45.0f;
		break;
	case 1: // ++-
		rotateX = -45.0f;
		rotateY = -135.0f;
		break;
	case 2: // +-+
		rotateX = 45.0f;
		rotateY = -45.0f;
		break;
	case 3: // -++
		rotateX = -45.0f;
		rotateY = 45.0f;
		break;
	case 4: // +--
		rotateX = 45.0f;
		rotateY = -135.0f;
		break;
	case 5: // -+-
		rotateX = -45.0f;
		rotateY = 135.0f;
		break;
	case 6: // --+
		rotateX = 45.0f;
		rotateY = 45.0f;
		break;
	case 7: // ---
		rotateX = -215.0f;
		rotateY = -45.0f;
		break;
	}
	glutPostRedisplay();
}

// Menu for changing viewing
void scaleMenu(int num)
{
	currentState[0] = static_cast<State>(num);

	switch (num)
	{
	case 0: isVariable = false;
		break;
	case 1:
		isVariable = true;
		break;
	}

	//printf("State: %d\n", currentState);

	glutPostRedisplay();
}

// Find min of two floats
float findMin(float a, float b)
{
	return (a < b) ? a : b;
}

// Find max of two floats
float findMax(float a, float b)
{
	return (a > b) ? a : b;
}

// Convert RGB values to HSV
void rgbTohsv(float r, float g, float b, float *h, float *s, float *v)
{
	float min;
	float max;
	float delta;

	min = findMin(findMin(r, g), b);
	max = findMax(findMax(r, g), b);

	// Find intensity
	delta = max - min;

	if (max != 0)
	{
		*s = delta / max;
	}
	else
	{
		*s = 0;
		*h = -1;
		return;
	}

	if (r == max)
	{
		*h = (g - b) / delta;
	}
	else if (g == max)
	{
		// Between yellow and magenta

		*h = 2 + (b - r) / delta;
	}
	else
	{
		// Between cyan and yellow

		*h = 4 + (r - g) / delta;
	}

	*h *= 60;

	if (*h < 0)
	{
		*h += 360;
	}
}

// Create main menu
void createMenu(void)
{
	// Create viewing menu
	int viewingMenu = glutCreateMenu(viewMenu);
	glutAddMenuEntry("+++", 0);
	glutAddMenuEntry("++-", 1);
	glutAddMenuEntry("+-+", 2);
	glutAddMenuEntry("-++", 3);
	glutAddMenuEntry("+--", 4);
	glutAddMenuEntry("-+-", 5);
	glutAddMenuEntry("--+", 6);
	glutAddMenuEntry("---", 7);

	// Create scaling menu
	int scalingMenu = glutCreateMenu(scaleMenu);
	glutAddMenuEntry("Fixed", 0);
	glutAddMenuEntry("Variable", 1);

	// Make Submenu's and attach the above submennus
	submenu = glutCreateMenu(menu);  // create menu, assign a callback
	glutAddSubMenu("Viewing", viewingMenu); // Attach submenu
	glutAddSubMenu("Scale Mode", scalingMenu); // Attach submenu

	glutCreateMenu(menu);
	glutAddSubMenu("Menu", submenu);  // add submenus to the menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);  // bind to the event: clicking the right button
}

// Draw principal Axes
void drawAxis()
{
	// Red X-Axis
	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(-0.6f, -0.5f, 0.5f);
	glEnd();

	// Green Y-Axis
	glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, 0.6f, 0.5f);
	glEnd();

	// Blue Z-Axis
	glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, -0.6f);
	glEnd();
}

// Draw color spectrum cube
void drawCube() 
{
	// Draw all faces of the cube

	glBegin(GL_POLYGON);
		glColor3f(0.0f + intensity, 0.0f + intensity, 1.0f + intensity);
		glVertex3f(0.5f, -0.5f, -0.5f); // P1 is red
		glColor3f(0.0f + intensity, 1.0f + intensity, 1.0f + intensity);
		glVertex3f(0.5f, 0.5f, -0.5f); // P2 is green
		glColor3f(1.0f + intensity, 1.0f + intensity, 1.0f + intensity);
		glVertex3f(-0.5f, 0.5f, -0.5f); // P3 is blue
		glColor3f(1.0f + intensity, 0.0f + intensity, 1.0f + intensity);
		glVertex3f(-0.5f, -0.5f, -0.5f); // P4 is purple
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.0f + intensity, 0.0f + intensity, 0.0f + intensity);
		glVertex3f(0.5f, -0.5f, 0.5f); // P8
		glColor3f(0.0f + intensity, 1.0f + intensity, 0.0f + intensity);
		glVertex3f(0.5f, 0.5f, 0.5f); // P7
		glColor3f(1.0f + intensity, 1.0f + intensity, 0.0f + intensity);
		glVertex3f(-0.5f, 0.5f, 0.5f); // P6
		glColor3f(1.0f + intensity, 0.0f + intensity, 0.0f + intensity);
		glVertex3f(-0.5f, -0.5f, 0.5f); // P5
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.0f + intensity, 0.0f + intensity, 1.0f + intensity);
		glVertex3f(0.5f, -0.5f, -0.5f); // P1
		glColor3f(0.0f + intensity, 1.0f + intensity, 1.0f + intensity);
		glVertex3f(0.5f, 0.5f, -0.5f); // P2
		glColor3f(0.0f + intensity, 1.0f + intensity, 0.0f + intensity);
		glVertex3f(0.5f, 0.5f, 0.5f); // P7
		glColor3f(0.0f + intensity, 0.0f + intensity, 0.0f + intensity);
		glVertex3f(0.5f, -0.5f, 0.5f); // P8
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(1.0f + intensity, 0.0f + intensity, 0.0f + intensity);
		glVertex3f(-0.5f, -0.5f, 0.5f); // P5
		glColor3f(1.0f + intensity, 1.0f + intensity, 0.0f + intensity);
		glVertex3f(-0.5f, 0.5f, 0.5f); // P6
		glColor3f(1.0f + intensity, 1.0f + intensity, 1.0f + intensity);
		glVertex3f(-0.5f, 0.5f, -0.5f); // P3
		glColor3f(1.0f + intensity, 0.0f + intensity, 1.0f + intensity);
		glVertex3f(-0.5f, -0.5f, -0.5f); // P4
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.0f + intensity, 1.0f + intensity, 0.0f + intensity);
		glVertex3f(0.5f, 0.5f, 0.5f); // P7
		glColor3f(0.0f + intensity, 1.0f + intensity, 1.0f + intensity);
		glVertex3f(0.5f, 0.5f, -0.5f); // P2
		glColor3f(1.0f + intensity, 1.0f + intensity, 1.0f + intensity);
		glVertex3f(-0.5f, 0.5f, -0.5f); // P3
		glColor3f(1.0f + intensity, 1.0f + intensity, 0.0f + intensity);
		glVertex3f(-0.5f, 0.5f, 0.5f); // P6
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.0f + intensity, 0.0f + intensity, 1.0f + intensity);
		glVertex3f(0.5f, -0.5f, -0.5f); // P1
		glColor3f(0.0f + intensity, 0.0f + intensity, 0.0f + intensity);
		glVertex3f(0.5f, -0.5f, 0.5f); // P8
		glColor3f(1.0f + intensity, 0.0f + intensity, 0.0f + intensity);
		glVertex3f(-0.5f, -0.5f, 0.5f); // P5
		glColor3f(1.0f + intensity, 0.0f, 1.0f + intensity);
		glVertex3f(-0.5f, -0.5f, -0.5f); // P4
	glEnd();
}

// Draw slider for brightness
void drawSlider()
{
	// Set color for rectangular slider
	glColor3f(1.0f, 1.0f, 1.0f);
	glOrtho(0.1f, 500, 500, 0.0f, 0.0f, 1.0); // Set orthographic viewing
	glScalef(sliderScale / 510.0f, 1.0f, 1.0f); // Scale rectangle for slider
	glRectf(0.5f, 490.0f, 490.0f, 500.0f);

	//printf("Rect Scale: (%.2f)\n", sliderScale);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();  // reset the current matrix to identity matrix

	// create a viewing matrix defined by an eye point and a reference point
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Draw all components
	glPushMatrix();
		glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
		glRotatef(rotateY, 0.0f, 1.0f, 0.0f);

		drawAxis();

		glScalef(scale, scale, scale);
		drawCube();
	glPopMatrix();

	// Draw slider at bottom of screen
	drawSlider();

	glFlush();
	glutSwapBuffers();
}

// Get the color from the mouse position
void getColor(int x, int y)
{
	// Pick color
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	unsigned char pixel[4]; // Store the pixel color
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	//printf("RGB: (%d, %d, %d)\n", (int)pixel[0], (int)pixel[1], (int)pixel[2]);

	// Convert picked color to HSV
	rgbTohsv((float)pixel[0], (float)pixel[1], (float)pixel[2], &hue, &sat, &cVal);

	// Update title
	snprintf(windowTitle, sizeof(windowTitle), "RGB Cube | RGB: (%.3f, %.3f, %.3f) | HSV: (%.3f, %.3f, %.3f)", (int)pixel[0] / 255.0f, (int)pixel[1] / 255.0f, (int)pixel[2] / 255.0f, hue, sat, cVal);

	// Update second window
	glutSetWindow(colorWindow);
	glutSetWindowTitle(windowTitle);
	glClearColor((int)pixel[0] / 255.0f, (int)pixel[1] / 255.0f, (int)pixel[2] / 255.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();

	// Update main window
	glutSetWindow(mainWindow);
	glutSetWindowTitle(windowTitle);
}

// Mouse event callback function
void mouse(int button, int state, int x, int y)
{
	//printf("Coordinates: (%d, %d)\n", x, y);

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		if ((currentState[0] == static_cast<State>(1)) || (currentState[0] == static_cast<State>(0) && (y < 490)))
		{
			getColor(x, y);
		}
		else if (currentState[0] == static_cast<State>(2))
		{
			// If the mouse is inside the slider

			// Redraw rectangle
			sliderScale = (float)x;
		}
	}
	else
	{
		// Reset state if mouse leaves the slider area
		currentState[0] = static_cast<State>(1);
	}

	// Reset window to window2
	glutSetWindow(colorWindow);
	glutPostRedisplay();

	// Reset window to main window
	glutSetWindow(mainWindow);
	glutPostRedisplay();	
}

// GlUT Mouse Function
void motion(int x, int y)
{
	float prevScale = sliderScale;

	if ((currentState[0] == static_cast<State>(1)) || (currentState[0] == static_cast<State>(0) && (y < 490)))
	{
		getColor(x, y);
	}
	else
	{
		sliderScale = (float)x / 1.0f;

		if (prevScale > sliderScale)
		{
			// Peel Layers
			if (intensity > -1.0f)
			{
				// Reduce brightness if it is not completely black
				intensity -= GRAY_INTENSITY / 2.0f;
			}

			if ((isVariable) && (scale > 0.1f))
			{
				// Reduce scale only till 0.1 while the program is in Variable state
				scale -= GRAY_INTENSITY / 2.0f;
			}
		}
		else
		{
			// Increase layers

			if (intensity < 0.0f)
			{
				// Increase brightness if it is not normal level
				intensity += GRAY_INTENSITY / 2.0f;
			}
			if ((isVariable) && (scale < 1.0f))
			{
				// Increase scale only till 1.0 while the program is in Variable state
				scale += GRAY_INTENSITY / 2.0f;
			}
		}

		glutPostRedisplay();
	}
}

// Set state of program if it's inside the slider
void passiveMotion(int x, int y)
{
	//printf("Before states: (%d, %d)\n", currentState[0], currentState[1]);

	if (y > 490.0f)
	{
		currentState[1] = currentState[0];
		currentState[0] = static_cast<State>(2);
		//printf("After states: (%d, %d)\n", currentState[0], currentState[1]);
	}

	glutPostRedisplay();
}

// GLUT reshape function
void reshape(int width, int height)
{
	// Compute aspect ratio of the new window
	if (height == 0) height = 1; // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	//printf("Width, Height: (%d, %d)\n", width, height);
}

void colorDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glFlush();
	glutSwapBuffers();
}

void initColorWindow()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(500, 100);
	glutInitWindowPosition(0, 600);  // define position of the window
	colorWindow = glutCreateWindow(windowTitle);
	glEnable(GL_DEPTH_TEST);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);

	glutDisplayFunc(colorDisplay);
}

// Initialise window
void init()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);  // define size of the window
	glutInitWindowPosition(0, 0);  // define position of the window
	mainWindow = glutCreateWindow(windowTitle);
	glEnable(GL_DEPTH_TEST);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  // initialize the library
	init();

	glClearColor(0.5, 0.5, 0.5, 1); // Give a gray background

	glutDisplayFunc(display);  // display callback

	glutMouseFunc(mouse); // Set mouse motion callback function
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutReshapeFunc(reshape);

	createMenu();

	initColorWindow(); // Make second window

	glutMainLoop();  // enter event processing loop

	return 0;
}
