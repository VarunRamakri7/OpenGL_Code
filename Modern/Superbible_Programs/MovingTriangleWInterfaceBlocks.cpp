#include "../../include/sb7.h"

/*
This program transfers data from vertex shader to fragment shader using 'in' and 'out' variables
*/

GLuint compile_shaders(void)
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	// Source code for Vertex Shader
	static const GLchar * vertex_shader_source[] =
	{
		"#version 450 core														\n"
		"																		\n"
		"// offset and color are input vertex attribute							\n"
		"layout (location = 0) in vec4 offset;									\n"
		"layout (location = 1) in vec4 color;									\n"
		"																		\n"
		"//Declare VS_OUT as an output interface block							\n"
		"out VS_OUT																\n"
		"{																		\n"
		"	vec4 color; //Send color to next stage								\n"
		"}vs_out;																\n"
		"																		\n"
		"void main(void)														\n"
		"{																		\n"
		"	//Decalre a hardcoded array of positions							\n"
		"	const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),		\n"
		"									 vec4(-0.25, -0.25, 0.5, 1.0),		\n"
		"									 vec4(0.25, 0.25, 0.5, 1.0));		\n"
		"																		\n"
		"	//Index into our array using gl_VertexID							\n"
		"	gl_Position = vertices[gl_VertexID] + offset;						\n"
		"																		\n"
		"//color = vec4(1.0, 0.0, 0.0, 1.0);									\n"
		"//Output fixed value for vs_color										\n"
		"vs_out.color = color;													\n"
		"}																		\n"
	};

	// Source code for Fragment Shader
	static const GLchar * fragment_shader_source[] =
	{
		"#version 450 core																\n"
		"																				\n"
		"//Declare VS_OUT as an input interface block									\n"
		"in VS_OUT																		\n"
		"{																				\n"
		"	vec4 color; //Send color to next stage										\n"
		"}fs_in;																		\n"
		"																				\n"
		"//Ouput to the framebuffer														\n"
		"out vec4 color;																\n"
		"																				\n"
		"void main(void)																\n"
		"{																				\n"
		"// Simply assign the color we were given by the vertex shader to our output	\n"
		"	color = fs_in.color;														\n"
		"}																				\n"
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

class DrawMovingTriangleWInterfaceBlocks : public sb7::application
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
		// Sets colour
		static const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
			(float)cos(currentTime) * 0.5f + 0.5f,
			0.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, color);

		// Use program object we created for rendering
		glUseProgram(rendering_program);

		GLfloat attrib[] = { (float)sin(currentTime) * 0.5f,
			(float)cos(currentTime) * 0.6f,
			0.0f, 0.0f };

		// Update value of input attribute 0
		glVertexAttrib4fv(0, attrib);

		// Draw one triangle	
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:

	GLuint rendering_program;
	GLuint vertex_array_object;
};

// Only instance of DECLARE_MAIN to state entry point
DECLARE_MAIN(DrawMovingTriangleWInterfaceBlocks);