#pragma once

#include "shader.h"
#include "myMath.h"

class GrassShader : public Shader
{
public:
	GrassShader(char* vertexFileName, char* fragmentFileName);
	~GrassShader(){};
	virtual void getUniformLocs();
	virtual void setUniforms();


private:

	GLint timeLoc;
	GLuint lightLoc;
	GLuint samplerLoc;


	GLuint diffLoc;
	GLuint specLoc;

	GLuint matDiffLoc;
	GLuint matSpecLoc;
};
