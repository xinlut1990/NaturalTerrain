#include "Terrain.h"

Terrain::Terrain(GLint program, const vec3 &center, const int width, const int height, float gridW, float gridH):
	Renderable(program), center(center), width(width), height(height), gridW(gridW), gridH(gridH)
{
	// one grid has 6 vertices
	this->vertSize = 6 * 3 * width * height;
	this->normSize = 6 * 3 * width * height;
	this->texSize = 6 * 2 * width * height;

	this->vertices = new float[this->vertSize];
	this->normals = new float[this->normSize];
	this->texCoords = new float[this->texSize];

	this->setVertLoc( glGetAttribLocation(shaderProgram,"position") );
	this->setNormLoc( glGetAttribLocation(shaderProgram, "normal") );
	this->setTexLoc( glGetAttribLocation(shaderProgram, "texCoord") );

	this->init();
}

Terrain::~Terrain()
{
	if(this->vertices != 0) {
		delete(this->vertices);
		this->vertices = 0;
	}
	if(this->normals != 0) {
		delete(this->normals);
		this->normals = 0;
	}
	if(this->texCoords != 0) {
		delete(this->texCoords);
		this->texCoords = 0;
	}

}

//fill the vertex buffer for current grid
void Terrain::fillGridVertices(int j, int i, int gridIdx, const vec3 &startPoint)
{

	//number of floats per vertex
	int numPerV = 3; 

	//(-1,-1)
	//	 |
	//(-1, 1) -- (1, 1)
	vertices[gridIdx + 0 * numPerV + 0] = startPoint.x + j * this->gridW;
	vertices[gridIdx + 0 * numPerV + 1] = startPoint.y;
	vertices[gridIdx + 0 * numPerV + 2] = startPoint.z + i * this->gridH;

	vertices[gridIdx + 1 * numPerV + 0] = startPoint.x + j * this->gridW;
	vertices[gridIdx + 1 * numPerV + 1] = startPoint.y;
	vertices[gridIdx + 1 * numPerV + 2] = startPoint.z + (i + 1) * this->gridH;

	vertices[gridIdx + 2 * numPerV + 0] = startPoint.x + (j + 1) * this->gridW;
	vertices[gridIdx + 2 * numPerV + 1] = startPoint.y;
	vertices[gridIdx + 2 * numPerV + 2] = startPoint.z + (i + 1) * this->gridH;

	//(-1,-1)  (1, -1)
	//		 \    |
	//         (1, 1)
	vertices[gridIdx + 3 * numPerV + 0] = startPoint.x + j * this->gridW;
	vertices[gridIdx + 3 * numPerV + 1] = startPoint.y;
	vertices[gridIdx + 3 * numPerV + 2] = startPoint.z + i * this->gridH;

	vertices[gridIdx + 4 * numPerV + 0] = startPoint.x + (j + 1) * this->gridW;
	vertices[gridIdx + 4 * numPerV + 1] = startPoint.y;
	vertices[gridIdx + 4 * numPerV + 2] = startPoint.z + (i + 1) * this->gridH;

	vertices[gridIdx + 5 * numPerV + 0] = startPoint.x + (j + 1) * this->gridW;
	vertices[gridIdx + 5 * numPerV + 1] = startPoint.y;
	vertices[gridIdx + 5 * numPerV + 2] = startPoint.z + i * this->gridH;
}

//fill the normals for current grid
void Terrain::fillGridNormals(int j, int i, int gridIdx)
{

	//number of floats per vertex
	int numPerV = 3; 

	//(-1,-1)
	//	 |
	//(-1, 1) -- (1, 1)
	normals[gridIdx + 0 * numPerV + 0] = 0.0f;
	normals[gridIdx + 0 * numPerV + 1] = 1.0f;
	normals[gridIdx + 0 * numPerV + 2] = 0.0f;

	normals[gridIdx + 1 * numPerV + 0] = 0.0f;
	normals[gridIdx + 1 * numPerV + 1] = 1.0f;
	normals[gridIdx + 1 * numPerV + 2] = 0.0f;

	normals[gridIdx + 2 * numPerV + 0] = 0.0f;
	normals[gridIdx + 2 * numPerV + 1] = 1.0f;
	normals[gridIdx + 2 * numPerV + 2] = 0.0f;

	//(-1,-1)  (1, -1)
	//		 \    |
	//         (1, 1)
	normals[gridIdx + 3 * numPerV + 0] = 0.0f;
	normals[gridIdx + 3 * numPerV + 1] = 1.0f;
	normals[gridIdx + 3 * numPerV + 2] = 0.0f;

	normals[gridIdx + 4 * numPerV + 0] = 0.0f;
	normals[gridIdx + 4 * numPerV + 1] = 1.0f;
	normals[gridIdx + 4 * numPerV + 2] = 0.0f;

	normals[gridIdx + 5 * numPerV + 0] = 0.0f;
	normals[gridIdx + 5 * numPerV + 1] = 1.0f;
	normals[gridIdx + 5 * numPerV + 2] = 0.0f;
}

//fill the normals for current grid
void Terrain::fillGridTexCoords(int j, int i, int gridIdx)
{

	//number of floats per vertex
	int numPerV = 2; 

	//(-1,-1)
	//	 |
	//(-1, 1) -- (1, 1)
	texCoords[gridIdx + 0 * numPerV + 0] = 0.0f;
	texCoords[gridIdx + 0 * numPerV + 1] = 0.0f;

	texCoords[gridIdx + 1 * numPerV + 0] = 0.0f;
	texCoords[gridIdx + 1 * numPerV + 1] = 1.0f;

	texCoords[gridIdx + 2 * numPerV + 0] = 1.0f;
	texCoords[gridIdx + 2 * numPerV + 1] = 1.0f;

	//(-1,-1)  (1, -1)
	//		 \    |
	//         (1, 1)
	texCoords[gridIdx + 3 * numPerV + 0] = 0.0f;
	texCoords[gridIdx + 3 * numPerV + 1] = 0.0f;

	texCoords[gridIdx + 4 * numPerV + 0] = 1.0f;
	texCoords[gridIdx + 4 * numPerV + 1] = 1.0f;

	texCoords[gridIdx + 5 * numPerV + 0] = 1.0f;
	texCoords[gridIdx + 5 * numPerV + 1] = 0.0f;
}


void Terrain::init()
{
	//start from top left
	vec3 startPoint;
	startPoint.y = this->center.y;
	startPoint.x = this->center.x - this->width / 2 * this->gridW;
	startPoint.z = this->center.z - this->height / 2 * this->gridH;

	if(this->width % 2 == 1) 
		startPoint.x -= this->gridW / 2;
	if(this->height % 2 == 1) 
		startPoint.z -= this->gridH / 2;

	//number of floats per vertex
	int numPerV = 3; 
	int numPerVTex = 2;
	//number of floats per grid
	int numPerGrid = 6 * numPerV;
	int numPerGridTex = 6 * numPerVTex;
	//number of floats per row
	int numPerRow = width * numPerGrid;
	int numPerRowTex = width * numPerGridTex;

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {

			int curGridIdx = i * numPerRow + j * numPerGrid;
			int curGridTexIdx = i * numPerRowTex + j * numPerGridTex;

			fillGridVertices(j, i, curGridIdx, startPoint);
			fillGridNormals(j, i, curGridIdx);
			fillGridTexCoords(j, i, curGridTexIdx);

		}
	}
}