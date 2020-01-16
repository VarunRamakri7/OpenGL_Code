#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	//GLenum error = glGetError(); Get the error code
	while (GLenum error = glGetError()) // While error is never 0
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " "
			<< file << ": " << line << std::endl;
		return false;
	}

	return true;
}
