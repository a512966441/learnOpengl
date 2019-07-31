#include<glew.h>
#include<glut.h>
#include<windows.h>
#include <cstdlib>
#include <iostream>
enum VAO_IDs
{
	Triangles,NumVAOs
};
enum Buffer_IDs
{
	ArrayBuffer,NumBuffers
};
enum Attrib_IDs
{
	vPosition = 0
};
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 6;
//
typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;
//
const char* ReadShader(const char* filename);
void init()
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	GLfloat vertices[NumVertices][3] = {
		{-0.9,-0.9,0.5}, //Triangle 1
		{0.85,-0.90,0.8},
		{-0.90,0.85,0.7},
		{0.90,-0.85,0.6},//Triangle 2
		{0.90,0.90,0.6},
		{-0.85,0.90,0.9}
	};
	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//
	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "./triangles.vert" },
		{ GL_FRAGMENT_SHADER, "./triangles.frag" },
		{ GL_NONE, NULL }
	};

	//
	GLuint program = glCreateProgram();
	ShaderInfo *entry = shaders;
	while (entry->type!=GL_NONE)
	{
		GLuint shader = glCreateShader(entry->type);
		entry->shader = shader;
		const char* source = ReadShader(entry->filename);
		if (source == NULL)
		{
			for (entry = shaders; entry->type != GL_NONE; ++entry)
			{
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}
			return;
		}
		glShaderSource(shader, 1, &source, NULL);
		delete[] source;
		glCompileShader(shader);
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			std::cout << "compile failed!" << std::endl;
			return;
		}
		glAttachShader(program, shader);
		++entry;
	}
	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		std::cout << "link failed!" << std::endl;
		return;
	}

	glUseProgram(program);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,0);
	glEnableVertexAttribArray(vPosition);
}
void display();
//void main(int argc,char** argv)
//{
//	glutInit(&argc, argv);
//	//glutInitDisplayMode(GLUT_RGBA);
//	glutCreateWindow("GLUT");
//	glutInitWindowSize(512, 512);
//	//glutInitContextProfile(GLUT_CORE_PROFILE);
//	if (glewInit())
//	{
//		std::cerr << "Uable to initialize" << std::endl;
//	}
//	init();
//	glutDisplayFunc(display);
//	glutMainLoop();
//
//}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//
	//glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glFlush();
}
const char* ReadShader(const char* filename)
{
#ifdef WIN32
	FILE* infile;
	fopen_s(&infile, filename, "rb");
#else
	FILE* infile = fopen(filename, "rb");
#endif // WIN32

	if (!infile) {
#ifdef _DEBUG
		std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);
	char* source = new char[len + 1];
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;
	return const_cast<const char*>(source);
}
