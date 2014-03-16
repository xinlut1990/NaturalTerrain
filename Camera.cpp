#include "Camera.h"

Camera::Camera()
{
	setIdentMatrix(viewMatrix, 4);
}

Camera::~Camera()
{
}

// View Matrix
// just like glulookat
void Camera::placeCam(float posX, float posY, float posZ, float lookX, float lookY, float lookZ) 
{
 
    float dir[3], right[3], up[3];
 
    up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;
 
    dir[0] =  (lookX - posX);
    dir[1] =  (lookY - posY);
    dir[2] =  (lookZ - posZ);
    normalize(dir);

	//zoom
	//posX += dir[0]
 
	// this order matters
    xProduct(dir,up,right);
    normalize(right);
 
    xProduct(right,dir,up);
    normalize(up);
 
    float aux[16];
 
    viewMatrix[0]  = right[0];
	viewMatrix[1]  = up[0];
	viewMatrix[2]  = -dir[0];
	viewMatrix[3]  = 0.0f;
    viewMatrix[4]  = right[1];
	viewMatrix[5]  = up[1];
	viewMatrix[6]  = -dir[1];
	viewMatrix[7]  = 0.0f;
    viewMatrix[8]  = right[2];
	viewMatrix[9]  = up[2];
	viewMatrix[10] = -dir[2];
	viewMatrix[11] = 0.0f;
    viewMatrix[12] = 0.0f;
    viewMatrix[13] = 0.0f;
    viewMatrix[14] =  0.0f;
    viewMatrix[15] = 1.0f;
    setTransMatrix(aux, -posX, -posY, -posZ);
    multiplyMatrix(viewMatrix, aux);
	
	// you should do this instead. If you want to apply rotation to your viewMatrix.
	//multiplyMatrix(viewMatrix, rotationMatrix(0.0,0.0,1.0, 0.785)); 
}

void Camera::rotateCam(float xAngle, float yAngle) 
{
	float* yRMat = rotationMatrix(0.0f, 1.0f, 0.0f, xAngle);
	float* xRMat = rotationMatrix(1.0f, 0.0f, 0.0f, yAngle);
	multiplyMatrix(viewMatrix, yRMat);
	multiplyMatrix(viewMatrix, xRMat);
}

void Camera::zoomCam(float zoom) 
{
	float zoomMat[16];
	setTransMatrix(zoomMat, 0.0f, 0.0f, zoom);
	multiplyMatrix(zoomMat, viewMatrix);
	setIdentMatrix(viewMatrix, 4);
	multiplyMatrix(viewMatrix, zoomMat);
}

float* Camera::getMatrix()
{
	return viewMatrix;
}