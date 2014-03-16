#include "glew\include\GL\glew.h"
#include "glut\glut.h"

class Renderable 
{
public:
	Renderable(GLint program) : shaderProgram(program)
	{ 
		vertLoc = -1; 
		normLoc = -1; 
		texLoc = -1;

		vertSize = 0;
		normSize = 0;
		texSize = 0;

		this->vertices = 0;
		this->colors = 0;
		this->normals = 0;
		this->texCoords = 0;
	};
	~Renderable(){};
	void loadIntoBuffer();
	void beginRender();
	void render();
	
protected:

	void setVertLoc(GLint vertLoc) { this->vertLoc = vertLoc;}; //set attribute location of vertex
	void setNormLoc(GLint normLoc) { this->normLoc = normLoc;}; //set attribute location of normal
	void setTexLoc(GLint texLoc) { this->texLoc = texLoc;}; //set attribute location of texture coord

	GLint shaderProgram;

	GLint vertLoc;
	GLint normLoc;
	GLint texLoc;
	//size of buffer
	int vertSize;
	int normSize;
	int texSize;

	float* vertices;
	float* normals;
	float* colors;
	float* texCoords;

};