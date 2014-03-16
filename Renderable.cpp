#include "Renderable.h"

void Renderable::loadIntoBuffer()
{
	GLuint buffers[3];

	glGenBuffers(3, buffers);

    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertSize * 4, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertLoc);
    glVertexAttribPointer(vertLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, normSize * 4, normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normLoc);
    glVertexAttribPointer(normLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, texSize * 4, texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texLoc);
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, 0, 0, 0);

}

void Renderable::beginRender()
{
	glUseProgram(shaderProgram);	
}


void Renderable::render()
{
	glDrawArrays(GL_TRIANGLES, 0, vertSize / 3);
	glutSwapBuffers();
}