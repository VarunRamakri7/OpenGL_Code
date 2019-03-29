#include "../../include/sb7.h"

GLuint compile_shaders(void)
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	// Source code for Vertex Shader
	static const GLchar * vertex_shader_source[] =
	{
		"#version 450 core									\n"
		"													\n"
		"void main(void)									\n"
		"{													\n"
		"	gl_Position = vec4(0.0, 0.0, 0.5, 1.0);			\n"
		"}													\n"
	};

	// Source code for Fragment Shader
	static const GLchar * fragment_shader_source[] =
	{
		"#version 450 core									\n"
		"													\n"
		"out vec4 color;									\n"
		"													\n"
		"void main(void)									\n"
		"{													\n"
		"	color = vec4(0.0, 0.8, 1.0, 1.0);				\n"
		"}													\n"
	};

	// Create and compiler Vertex Shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	// Create and compiler Fragment Shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	// Create program, attach shaders to it, and link it
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	// Delete shaders as program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
};

class my_application : public sb7::application
{
public:

	void startup()
	{
		rendering_program = compile_shaders();
		glCreateVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
		glDeleteVertexArrays(1, &vertex_array_object);
	}

	// Our rendering function
	void render(double currentTime)
	{
		//Clear the window to red
		static const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
			(float)cos(currentTime) * 0.5f + 0.5f,
			0.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, color);

		// Use program object we created for rendering
		glUseProgram(rendering_program);

		// Draws one point and changes point size
		glPointSize(20);
		glDrawArrays(GL_POINTS, 0, 1);
	}

private:

	GLuint rendering_program;
	GLuint vertex_array_object;
};