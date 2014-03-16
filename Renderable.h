class Renderable 
{
public:
	Renderable();
	~Renderable();

private:
	float* vertices;
	float* normals;
	float* colors;
	float* texCoords;

};