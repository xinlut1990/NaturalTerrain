#include "myMath.h"

class Terrain
{
public:
	Terrain(const vec3 &center, int width, int height, float gridW, float gridH);
	~Terrain();

private:
	vec3 center;
	int width; // unit is grid
	int height;
	float gridW;
	float gridH;
	//data stream
	float *vertices;
	float *normals;
	float *texCoords;
};