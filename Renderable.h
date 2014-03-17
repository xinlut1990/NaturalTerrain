#pragma once
#include "shader.h"

class Renderable 
{
public:
	Renderable(Shader* program) : shaderProgram(program)
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
	void loadIntoBuffer(GLint vertObj, GLint normObj, GLint texObj);
	void beginRender();
	void render();

	Shader* getShader() const;
	
protected:

	void setVertLoc(GLint vertLoc) { this->vertLoc = vertLoc;}; //set attribute location of vertex
	void setNormLoc(GLint normLoc) { this->normLoc = normLoc;}; //set attribute location of normal
	void setTexLoc(GLint texLoc) { this->texLoc = texLoc;}; //set attribute location of texture coord

	Shader* shaderProgram;

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