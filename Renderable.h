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


		this->vertices = 0;
		this->colors = 0;
		this->normals = 0;
		this->texCoords = 0;
	};
	~Renderable(){

		if(this->vertices != NULL) {
			delete(this->vertices);
			this->vertices = NULL;
		}
		if(this->normals != NULL) {
			delete(this->normals);
			this->normals = NULL;
		}
		if(this->texCoords != NULL) {
			delete(this->texCoords);
			this->texCoords = NULL;
		}
		if(this->shaderProgram != NULL) {
			delete(this->shaderProgram);
			this->shaderProgram = NULL;
		}
	};
	void loadIntoBuffer(GLint vertObj, GLint normObj, GLint texObj);
	void beginRender();
	void render();
	void setMatrices(const float *viewMatrix, const float *projMatrix);

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