/**
		Program to draw a triangle with tesselation.
**/
#include <sb7.h>

class TesselatedTriangle : public sb7::application
{
	void init()
	{
		static const char title[] = "Tessellated Triangle";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}

	virtual void startup()
	{
		static const char * vertex_shader_source[] =
		{
			"#version 450 core                                                 \n"
			"                                                                  \n"
			"void main(void)                                                   \n"
			"{                                                                 \n"
			"    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
			"                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
			"                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
			"                                                                  \n"
			"    gl_Position = vertices[gl_VertexID];                          \n"
			"}                                                                 \n"
		};

		static const char * tesselation_control_shader_source[] =
		{
			"#version 450 core                                                                 \n"
			"                                                                                  \n"
			"layout (vertices = 3) out;                                                        \n"
			"                                                                                  \n"
			"void main(void)                                                                   \n"
			"{                                                                                 \n"
			"    if (gl_InvocationID == 0)                                                     \n"
			"    {                                                                             \n"
			"        gl_TessLevelInner[0] = 5.0;                                               \n"
			"        gl_TessLevelOuter[0] = 5.0;                                               \n"
			"        gl_TessLevelOuter[1] = 5.0;                                               \n"
			"        gl_TessLevelOuter[2] = 5.0;                                               \n"
			"    }                                                                             \n"
			"    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;     \n"
			"}                                                                                 \n"
		};

		static const char * tesselation_evaluation_shader_source[] =
		{
			"#version 450 core                                                                 \n"
			"                                                                                  \n"
			"layout (triangles, equal_spacing, cw) in;                                         \n"
			"                                                                                  \n"
			"void main(void)                                                                   \n"
			"{                                                                                 \n"
			"    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                       \n"
			"                  (gl_TessCoord.y * gl_in[1].gl_Position) +                       \n"
			"                  (gl_TessCoord.z * gl_in[2].gl_Position);                        \n"
			"}                                                                                 \n"
		};

		static const char * fragment_shader_source[] =
		{
			"#version 450 core                                                 \n"
			"                                                                  \n"
			"out vec4 color;                                                   \n"
			"                                                                  \n"
			"void main(void)                                                   \n"
			"{                                                                 \n"
			"    color = vec4(0.0, 0.8, 1.0, 1.0);                             \n"
			"}                                                                 \n"
		};

		rendering_program = glCreateProgram();

		// Compile shaders
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, vertex_shader_source, NULL);
		glCompileShader(vs);

		GLuint tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tcs, 1, tesselation_control_shader_source, NULL);
		glCompileShader(tcs);

		GLuint tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tes, 1, tesselation_evaluation_shader_source, NULL);
		glCompileShader(tes);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, fragment_shader_source, NULL);
		glCompileShader(fs);

		// Attach shaders to the program
		glAttachShader(rendering_program, vs);
		glAttachShader(rendering_program, tcs);
		glAttachShader(rendering_program, tes);
		glAttachShader(rendering_program, fs);

		// Link the program
		glLinkProgram(rendering_program);

		// Generate vertex arrays
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);

		// Declare the drawing mode for the polygons
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void render(double currentTime)
	{
		static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, green);

		glUseProgram(rendering_program);

		glDrawArrays(GL_PATCHES, 0, 3);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vertex_array_object);

		glDeleteProgram(rendering_program);
	}


private:

	GLuint          rendering_program;
	GLuint          vertex_array_object;

};

// One and only instance of DECLARE_MAIN
DECLARE_MAIN(TesselatedTriangle)