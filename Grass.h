
#include <iostream>
#include "Renderable.h"

using namespace std;

class Grass : public Renderable
{
public:
	Grass(Shader *program, const vec3 &pos);
	~Grass(){};
	void init();

private:
	void fillPieceVertices(int pieceIdx, const vec3 &leftPoint, const vec3 &rightPoint);
	void fillPieceTexCoords(int pieceIdx);
	const float height;
	const float width;
	vec3 pos;
};