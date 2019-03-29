/**
	First program from OpenGL Superbible
**/
#include "../../include/sb7.h"

class my_application : public sb7::application
{
public:

	// Overrides parent's render()
	void render(double currentTime)
	{
		//Clear the window to red
		static const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
										 (float)cos(currentTime) * 0.5f + 0.5f,
										 0.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, color);
	}
};

// Only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);