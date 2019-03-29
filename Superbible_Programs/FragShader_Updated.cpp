#include "../../include/sb7.h"

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
		"// vs_color is an output that will be sent to the next shader stage	\n"
		"out vec4 vs_color;														\n"
		"																		\n"
		"void main(void)														\n"
		"{																		\n"
		"	const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),		\n"
		"									 vec4(-0.25, -0.25, 0.5, 1.0),		\n"
		"									 vec4(0.25, 0.25, 0.5, 1.0));		\n"
		"																		\n"
		"	const vec4 colors[] = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),				\n"
		"								  vec4(0.0, 1.0, 0.0, 1.0),				\n"
		"								  vec4(0.0, 0.0, 1.0, 1.0));			\n"
		"																		\n"
		"	//Add offset to our hard-coded vertex position						\n"
		"	gl_Position = vertices[gl_VertexID];								\n"
		"																		\n"
		"	// Ouput a fixed value for vs_color									\n"
		"	vs_color = colors[gl_VertexID];										\n"
		"}																		\n"
	};

	// Source code for Fragment Shader
	static const GLchar * fragment_shader_source[] =
	{
		"#version 450 core														\n"
		"																		\n"
		"// Colour produced by the vertex shader in the previous shader stage	\n"
		"in vec4 vs_color;														\n"
		"																		\n"
		"out vec4 color;														\n"
		"																		\n"
		"void main(void)														\n"
		"{																		\n"
		"	//color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,				\n"
		"			//cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,					\n"
		"			//sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),	\n"
		"			//1.0);														\n"
		"																		\n"
		"	color = vs_color;													\n"
		"}																		\n"
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

class TriangleWFragShaderTakeInputFromVertShader : public sb7::application
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
		static const GLfloat color[] = { 0.0f, 0.2f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

		// Use program object we created for rendering
		glUseProgram(rendering_program);

		// Draw one triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:

	GLuint rendering_program;
	GLuint vertex_array_object;
};

DECLARE_MAIN(TriangleWFragShaderTakeInputFromVertShader);