#include "../../include/sb7.h"

GLuint compile_shaders(void)
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	// Source code for Vertex Shader
	static const GLchar * vertex_shader_source[] =
	{
		"#version 430 core													\n"
		"																	\n"
		"void main(void)													\n"
		"{																	\n"
		"	//Decalre a hardcoded array of positions						\n"
		"	const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),	\n"
		"									 vec4(-0.25, -0.25, 0.5, 1.0),	\n"
		"									 vec4(0.25, 0.25, 0.5, 1.0));	\n"
		"	//Index into our array using gl_VertexID						\n"
		"	gl_Position = vertices[gl_VertexID];							\n"
		"}																	\n"
	};

	// Source code for Fragment Shader
	static const GLchar * fragment_shader_source[] =
	{
		"#version 430 core									\n"
		"													\n"
		"out vec4 color;									\n"
		"													\n"
		"void main(void)									\n"
		"{													\n"
		"	color = vec4(0.0, 0.8, 1.0, 1.0);				\n"
		"}													\n"
	};

	/*const char* vert = R"GLSL(
	#version 450 core                                                
	void main(void)                                                  
	{                                                                
		//Decalre a hardcoded array of positions                      
		const vec4 vertices[3] = vec4[3]
			(
			vec4(0.25, -0.25, 0.5, 1.0), 
			vec4(-0.25, -0.25, 0.5, 1.0),
			vec4(0.25, 0.25, 0.5, 1.0)
			);
		//Index into our array using gl_VertexID                      
		gl_Position = vertices[gl_VertexID];                          
	}                                                                
	)GLSL";

	const char* frag = R"GLSL(
	#version 450 core                           
	out vec4 color;                             
	void main(void)                             
	{                                           
		color = vec4(0.0, 0.8, 1.0, 1.0);        
	}                                           
	)GLSL";*/


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

class DrawTriangle : public sb7::application
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

DECLARE_MAIN(DrawTriangle);