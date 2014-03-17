#pragma once

#include "fileAccessor.h"
#include "glew\include\GL\glew.h"



#include "Light.h"

class Shader
{
public:
	Shader(char* vertexFileName, char* fragmentFileName);
	~Shader() {};

	virtual void getUniformLocs() = 0;
	virtual void setUniforms() = 0;
	void setLight(const Light &light);
	void setSampler(GLint sampler);

	GLuint projMatrixLoc;
	GLuint viewMatrixLoc;

	GLuint program;

protected:

	GLuint buildShader(char* vertexFileName, char* fragmentFileName);

	void printShaderInfoLog(GLuint obj);
	void printProgramInfoLog(GLuint obj);

	
	GLint sampler;
	Light light;


};