#include "GrassShader.h"

GrassShader::GrassShader(char* vertexFileName, char* fragmentFileName)
	:Shader(vertexFileName, fragmentFileName)
{
	this->getUniformLocs();
}

void GrassShader::getUniformLocs()
{
	projMatrixLoc = glGetUniformLocation(program, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(program, "viewMatrix");

	timeLoc = glGetUniformLocation(program, "time");

	lightLoc = glGetUniformLocation(program, "lightPos");
	samplerLoc = glGetUniformLocation(program, "tex");

	diffLoc = glGetUniformLocation(program, "intensityDiff");
	specLoc = glGetUniformLocation(program, "intensitySpec");

	matDiffLoc = glGetUniformLocation(program, "materialDiff");
	matSpecLoc = glGetUniformLocation(program, "materialSpec");
}

void GrassShader::setUniforms()
{
	glUniform1i(samplerLoc , this->sampler);

	glUniform1f(timeLoc, this->time);
	vec4 lightPos = light.getPosition();
	glUniform4f(lightLoc, lightPos.x, lightPos.y, lightPos.z, lightPos.w);

	color diff = light.getIntensityDiff();
	glUniform4f(diffLoc, diff.r, diff.g, diff.b, diff.a);

	color spec = light.getIntensitySpec();
	glUniform4f(specLoc, spec.r, spec.g, spec.b, spec.a);

	color matDiff(1.0, 1.0, 1.0, 1.0);
	glUniform4f(matDiffLoc, matDiff.r, matDiff.g, matDiff.b, matDiff.a);

	color matSpec(1.0, 1.0, 1.0, 1.0);
	glUniform4f(matSpecLoc, matSpec.r, matSpec.g, matSpec.b, matSpec.a);
}