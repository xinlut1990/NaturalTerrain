#include <iostream>
#include "myMath.h"
#include "Renderable.h"
#include "noise.h"

using namespace std;

class Terrain : public Renderable
{
public:
	Terrain(GLint program, const vec3 &center, const int width, const int height, float gridW, float gridH);
	~Terrain();
	void init();

private:
	void fillGridVertices(int j, int i, int gridIdx, const vec3 &startPoint);
	void fillGridNormals(int j, int i, int gridIdx);
	void fillGridTexCoords(int j, int i, int gridIdx);
	vec3 center;
	int width; // unit is grid
	int height;
	float gridW;
	float gridH;
	PerlinNoise* heightGenerator;
	//data stream

};