#include "myMath.h"

class Camera 
{
public:
	Camera();
	~Camera();
	void placeCam(float posX, float posY, float posZ, float lookX, float lookY, float lookZ);
	void rotateCam(float xAngle, float yAngle);
	void zoomCam(float zoom);
	float* getMatrix();
private:

	float viewMatrix[16];
};