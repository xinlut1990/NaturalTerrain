#pragma once

#include "shader.h"
#include "myMath.h"

class TerrainShader : public Shader
{
public:
	TerrainShader(char* vertexFileName, char* fragmentFileName);
	~TerrainShader(){};
	virtual void getUniformLocs();
	virtual void setUniforms();

private:

	GLuint lightLoc;
	GLuint samplerLoc;


	GLuint diffLoc;
	GLuint specLoc;

	GLuint matDiffLoc;
	GLuint matSpecLoc;
};
