#include "Terrain.h"

Terrain::Terrain(Shader *program, const vec3 &center, const int width, const int height, float gridW, float gridH, bool isFlat):
	Renderable(program), center(center), width(width), height(height), gridW(gridW), gridH(gridH)
{
	// one grid has 6 vertices
	this->vertSize = 6 * 3 * width * height;
	this->normSize = 6 * 3 * width * height;
	this->texSize = 6 * 2 * width * height;

	this->vertices = new float[this->vertSize];
	this->normals = new float[this->normSize];
	this->texCoords = new float[this->texSize];

	this->setVertLoc( glGetAttribLocation(shaderProgram->program,"position") );
	this->setNormLoc( glGetAttribLocation(shaderProgram->program, "normal") );
	this->setTexLoc( glGetAttribLocation(shaderProgram->program, "texCoord") );

	if(isFlat) {
		this->heightGenerator = new PerlinNoise(1,1,0,1,1);
	} else {
		this->heightGenerator = new PerlinNoise(2,0.1,16,1,1);
	}

	heightMap = new double*[height + 1];

	for(int i = 0; i <= height; i++) {
		heightMap[i] = new double[width + 1];
		for(int j = 0; j <= width; j++) {
			heightMap[i][j] = this->heightGenerator->GetHeight(j, i);
		}
	}

	this->init();
}

Terrain::~Terrain()
{
	for(int i = 0; i <= height; i++) {
		delete[](heightMap[i]);
	}
	delete[](heightMap);

	delete(this->heightGenerator);
		this->heightGenerator = 0;
}

float Terrain::getHeight(int x, int y)
{
	return heightMap[y][x];
}

//fill the vertex buffer for current grid
void Terrain::fillGridVertices(int j, int i, int gridIdx, const vec3 &startPoint)
{

	//number of floats per vertex
	int numPerV = 3; 

	float height00 = this->getHeight(j, i);
	float height10 = this->getHeight(j + 1, i);
	float height01 = this->getHeight(j, i + 1);
	float height11 = this->getHeight(j + 1, i + 1);

	//(-1,-1)
	//	 |
	//(-1, 1) -- (1, 1)
	vertices[gridIdx + 0 * numPerV + 0] = startPoint.x + j * this->gridW;
	vertices[gridIdx + 0 * numPerV + 1] = startPoint.y + height00 ;
	vertices[gridIdx + 0 * numPerV + 2] = startPoint.z + i * this->gridH;

	vertices[gridIdx + 1 * numPerV + 0] = startPoint.x + j * this->gridW;
	vertices[gridIdx + 1 * numPerV + 1] = startPoint.y + height01;
	vertices[gridIdx + 1 * numPerV + 2] = startPoint.z + (i + 1) * this->gridH;

	vertices[gridIdx + 2 * numPerV + 0] = startPoint.x + (j + 1) * this->gridW;
	vertices[gridIdx + 2 * numPerV + 1] = startPoint.y + height11;
	vertices[gridIdx + 2 * numPerV + 2] = startPoint.z + (i + 1) * this->gridH;

	//(-1,-1)  (1, -1)
	//		 \    |
	//         (1, 1)
	vertices[gridIdx + 3 * numPerV + 0] = startPoint.x + j * this->gridW;
	vertices[gridIdx + 3 * numPerV + 1] = startPoint.y + height00;
	vertices[gridIdx + 3 * numPerV + 2] = startPoint.z + i * this->gridH;

	vertices[gridIdx + 4 * numPerV + 0] = startPoint.x + (j + 1) * this->gridW;
	vertices[gridIdx + 4 * numPerV + 1] = startPoint.y + height11;
	vertices[gridIdx + 4 * numPerV + 2] = startPoint.z + (i + 1) * this->gridH;

	vertices[gridIdx + 5 * numPerV + 0] = startPoint.x + (j + 1) * this->gridW;
	vertices[gridIdx + 5 * numPerV + 1] = startPoint.y + height10;
	vertices[gridIdx + 5 * numPerV + 2] = startPoint.z + i * this->gridH;
}

//fill the normals for current grid
void Terrain::fillGridNormals(int j, int i, int gridIdx)
{

	//number of floats per vertex
	int numPerV = 3; 

	vec3 p0;
	p0.x = vertices[gridIdx + 0 * numPerV + 0];
	p0.y = vertices[gridIdx + 0 * numPerV + 1];
	p0.z = vertices[gridIdx + 0 * numPerV + 2];

	vec3 p1;
	p1.x = vertices[gridIdx + 1 * numPerV + 0];
	p1.y = vertices[gridIdx + 1 * numPerV + 1];
	p1.z = vertices[gridIdx + 1 * numPerV + 2];

	vec3 p2;
	p2.x = vertices[gridIdx + 2 * numPerV + 0];
	p2.y = vertices[gridIdx + 2 * numPerV + 1];
	p2.z = vertices[gridIdx + 2 * numPerV + 2];

	float normal[3];
	float v1[3] = { p1.x - p0.x, p1.y - p0.y, p1.z - p0.z }; 
	float v2[3] = { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
	xProduct( v1 , v2, normal );
	normalize(normal);

	//(-1,-1)
	//	 |
	//(-1, 1) -- (1, 1)
	normals[gridIdx + 0 * numPerV + 0] = normal[0];
	normals[gridIdx + 0 * numPerV + 1] = normal[1];
	normals[gridIdx + 0 * numPerV + 2] = normal[2];

	normals[gridIdx + 1 * numPerV + 0] = normal[0];
	normals[gridIdx + 1 * numPerV + 1] = normal[1];
	normals[gridIdx + 1 * numPerV + 2] = normal[2];

	normals[gridIdx + 2 * numPerV + 0] = normal[0];
	normals[gridIdx + 2 * numPerV + 1] = normal[1];
	normals[gridIdx + 2 * numPerV + 2] = normal[2];


	vec3 p3;
	p3.x = vertices[gridIdx + 3 * numPerV + 0];
	p3.y = vertices[gridIdx + 3 * numPerV + 1];
	p3.z = vertices[gridIdx + 3 * numPerV + 2];

	vec3 p4;
	p4.x = vertices[gridIdx + 4 * numPerV + 0];
	p4.y = vertices[gridIdx + 4 * numPerV + 1];
	p4.z = vertices[gridIdx + 4 * numPerV + 2];

	vec3 p5;
	p5.x = vertices[gridIdx + 5 * numPerV + 0];
	p5.y = vertices[gridIdx + 5 * numPerV + 1];
	p5.z = vertices[gridIdx + 5 * numPerV + 2];

	float normal1[3];
	float v11[3] = { p1.x - p0.x, p1.y - p0.y, p1.z - p0.z }; 
	float v21[3] = { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
	xProduct( v11 , v21, normal1 );
	normalize(normal1);

	//(-1,-1)  (1, -1)
	//		 \    |
	//         (1, 1)
	normals[gridIdx + 3 * numPerV + 0] = normal1[0];
	normals[gridIdx + 3 * numPerV + 1] = normal1[1];
	normals[gridIdx + 3 * numPerV + 2] = normal1[2];

	normals[gridIdx + 4 * numPerV + 0] = normal1[0];
	normals[gridIdx + 4 * numPerV + 1] = normal1[1];
	normals[gridIdx + 4 * numPerV + 2] = normal1[2];

	normals[gridIdx + 5 * numPerV + 0] = normal1[0];
	normals[gridIdx + 5 * numPerV + 1] = normal1[1];
	normals[gridIdx + 5 * numPerV + 2] = normal1[2];
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