#include "Grass.h"

Grass::Grass(Shader *program, const vec3 &pos): Renderable(program), pos(pos), height(1.0f), width(1.0f)
{

	this->vertSize = 3 * 6 * 3;
	this->normSize = 3 * 6 * 3;
	this->texSize = 3 * 6 * 2;

	this->vertices = new float[this->vertSize];
	this->normals = new float[this->normSize];
	this->texCoords = new float[this->texSize];

	this->setVertLoc( glGetAttribLocation(shaderProgram->program,"position") );
	this->setNormLoc( glGetAttribLocation(shaderProgram->program, "normal") );
	this->setTexLoc( glGetAttribLocation(shaderProgram->program, "texCoord") );


	this->init();
}

//fill the vertex buffer for current grid
void Grass::fillPieceVertices(int pieceIdx, const vec3 &leftPoint, const vec3 &rightPoint)
{
	int numPerV = 3; 

	//(-1,-1)
	//	 |
	//(-1, 1) -- (1, 1)
	vertices[pieceIdx + 0 * numPerV + 0] = leftPoint.x;
	vertices[pieceIdx + 0 * numPerV + 1] = leftPoint.y + height;
	vertices[pieceIdx + 0 * numPerV + 2] = leftPoint.z;

	vertices[pieceIdx + 1 * numPerV + 0] = leftPoint.x;
	vertices[pieceIdx + 1 * numPerV + 1] = leftPoint.y;
	vertices[pieceIdx + 1 * numPerV + 2] = leftPoint.z;

	vertices[pieceIdx + 2 * numPerV + 0] = rightPoint.x;
	vertices[pieceIdx + 2 * numPerV + 1] = rightPoint.y;
	vertices[pieceIdx + 2 * numPerV + 2] = rightPoint.z;

	//(-1,-1)  (1, -1)
	//		 \    |
	//         (1, 1)
	vertices[pieceIdx + 3 * numPerV + 0] = leftPoint.x;
	vertices[pieceIdx + 3 * numPerV + 1] = leftPoint.y + height;
	vertices[pieceIdx + 3 * numPerV + 2] = leftPoint.z;

	vertices[pieceIdx + 4 * numPerV + 0] = rightPoint.x;
	vertices[pieceIdx + 4 * numPerV + 1] = rightPoint.y;
	vertices[pieceIdx + 4 * numPerV + 2] = rightPoint.z;

	vertices[pieceIdx + 5 * numPerV + 0] = rightPoint.x;
	vertices[pieceIdx + 5 * numPerV + 1] = rightPoint.y + height;
	vertices[pieceIdx + 5 * numPerV + 2] = rightPoint.z;

}

//fill the vertex buffer for current grid
void Grass::fillPieceTexCoords(int pieceIdx)
{
	int numPerV = 2; 

	//(-1,-1)
	//	 |
	//(-1, 1) -- (1, 1)
	texCoords[pieceIdx + 0 * numPerV + 0] = 0.0f;
	texCoords[pieceIdx + 0 * numPerV + 1] = 1.0f;

	texCoords[pieceIdx + 1 * numPerV + 0] = 0.0f;
	texCoords[pieceIdx + 1 * numPerV + 1] = 0.0f;

	texCoords[pieceIdx + 2 * numPerV + 0] = 1.0f;
	texCoords[pieceIdx + 2 * numPerV + 1] = 0.0f;

	//(-1,-1)  (1, -1)
	//		 \    |
	//         (1, 1)
	texCoords[pieceIdx + 3 * numPerV + 0] = 0.0f;
	texCoords[pieceIdx + 3 * numPerV + 1] = 1.0f;

	texCoords[pieceIdx + 4 * numPerV + 0] = 1.0f;
	texCoords[pieceIdx + 4 * numPerV + 1] = 0.0f;

	texCoords[pieceIdx + 5 * numPerV + 0] = 1.0f;
	texCoords[pieceIdx + 5 * numPerV + 1] = 1.0f;

}

void Grass::init()
{
	//number of floats per vertex
	int numPerV = 3; 
	int numPerVTex = 2;
	//number of floats per grid
	int numPerPiece = 6 * numPerV;
	int numPerPieceTex = 6 * numPerVTex;

	float sqrt3 = 1.732;
	//    bl  br
	//  l        r
	//    fl  fr
	vec3 l = vec3(pos.x - width/2, pos.y, pos.z);
	vec3 r = vec3(pos.x + width/2, pos.y, pos.z);
	vec3 fl = vec3(pos.x - width/4, pos.y, pos.z + sqrt3 * width/4);
	vec3 fr = vec3(pos.x + width/4, pos.y, pos.z + sqrt3 * width/4);
	vec3 bl = vec3(pos.x - width/4, pos.y, pos.z - sqrt3 * width/4);
	vec3 br = vec3(pos.x + width/4, pos.y, pos.z - sqrt3 * width/4);

	fillPieceVertices(0 * numPerPiece, l, r);
	fillPieceVertices(1 * numPerPiece, bl, fr);
	fillPieceVertices(2 * numPerPiece, fl, br);

	for(int i = 0; i < 18; i++) {
		normals[i * numPerV + 0] = 0.0f;
		normals[i * numPerV + 1] = 1.0f;
		normals[i * numPerV + 2] = 0.0f;
	}

	fillPieceTexCoords(0 * numPerPieceTex);
	fillPieceTexCoords(1 * numPerPieceTex);
	fillPieceTexCoords(2 * numPerPieceTex);


}


	