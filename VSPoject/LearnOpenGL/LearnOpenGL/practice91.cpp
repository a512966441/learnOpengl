#include<glew.h>
#include<glut.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
//4个顶点
static const GLfloat vertex_positons[] =
{
	-0.9,-0.9,0.0, //Triangle 1
	0.85,-0.90,0.0,
	-0.90,0.85 ,0.0
};
//每个顶点的颜色
static const GLfloat vertex_colors[] =
{
	1.0,1.0,1.0,1.0,
	1.0,1.0,0.0,1.0,
	1.0,0.0,1.0,1.0,
	0.0,1.0,1.0,1.0
};
//三个索引值
static const GLushort vertex_indices[] =
{ 0,1,2 };
enum VAO_IDs
{
	Triangles, NumVAOs
};
enum Buffer_IDs
{
	ArrayBuffer, NumBuffers
};
enum IndexBuffer_IDs
{
	IndexBuffer, NumIndexBuffers
};
enum Attrib_IDs
{
	vPosition = 0
};

extern GLuint VAOs[NumVAOs];
extern GLuint Buffers[NumBuffers];
GLuint IndexBuffers[NumIndexBuffers];
extern const GLuint NumVertices;
 const char* ReadShader(const char* filename);
void init1()
{
	//Vao
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);
	//vbo
	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positons), vertex_positons, GL_STATIC_DRAW);
	//ebo
	glGenBuffers(NumIndexBuffers, IndexBuffers);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffers[IndexBuffer]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
	//

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(vPosition);

	const char* source_vert=ReadShader("./triangles.vert");
	const char* source_frag= ReadShader("./triangles.frag");
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &source_vert,0);
	glCompileShader(vertexShader);
	int compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		std::cerr << "compile failed!" << std::endl;
		return;
	}
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &source_frag, nullptr);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		std::cerr << "compile failed!" << std::endl;
		return;
	}
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::cerr << "link error!" << std::endl;
		return;
	}
	//
	glUseProgram(shaderProgram);

}
void display1()
{
	glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,0);
	glFlush();
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutCreateWindow("GLUT");
//	glutInitWindowSize(512, 512);
	if (glewInit())
	{
		std::cerr << "Uable to initialize" << std::endl;
	}
	init1();
	glutDisplayFunc(display1);
	glutMainLoop();
}