#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "glew\include\GL\glew.h"
#include "glut\glut.h"
#include <vector>
#include <map>


#include "myMath.h"
#include "SOIL.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "TerrainShader.h"
#include "Grass.h"
#include "GrassShader.h"

// shader names
char *vertexFileName = "vertex.txt";
char *fragmentFileName = "frag.txt";

char *vertGrassFileName = "vertGrass.txt";
char *fragGrassFileName = "fragGrass.txt";

int xPick;
int yPick;

Terrain * pTerrain = NULL;
Terrain * pWater = NULL;
vector<Grass *> grasses;
const int numGrass = 1000;
 
 
// storage for matrices
float projMatrix[16];
float worldMatrix[16];

Camera cam;

Light light(vec4(4.0, 5.0, 5.0, 1.0), color(0.5, 0.5, 0.5, 1.0), color(0.5, 0.5, 0.5, 1.0));

GLuint texName[3];

	GLuint buffers[3];
int frame=0,time1,timebase=0;

float xAngle = 0.0f;
float yAngle = 0.0f;

float zoom = 0.0f;

char s[50];
 
void initTex() {
	
	glActiveTexture(GL_TEXTURE1);
	texName[0] = SOIL_load_OGL_texture
		(
		"Grass.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);
	pTerrain->getShader()->setSampler(1);

	glActiveTexture(GL_TEXTURE2);
	texName[1] = SOIL_load_OGL_texture
		(
		"water.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);
	pWater->getShader()->setSampler(2);

	glActiveTexture(GL_TEXTURE3);
	texName[2] = SOIL_load_OGL_texture
		(
		"tallgrass.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);

	for(int i = 0; i < numGrass; i++) {
		grasses[i]->getShader()->setSampler(3);
	}
	

	//glBindTexture(GL_TEXTURE_2D, texName[0]);
	

	
	//glBindTexture(GL_TEXTURE_2D, texName[1]);
	
}

 
// Projection Matrix
void buildProjMatrix(float fov, float ratio, float nearP, float farP) {
 
    float f = 1.0f / tan (fov * (PI / 360.0));
    setIdentMatrix(projMatrix,4);
    projMatrix[0] = f / ratio;
    projMatrix[1 * 4 + 1] = f;
    projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    projMatrix[2 * 4 + 3] = -1.0f;
    projMatrix[3 * 4 + 3] = 0.0f;
}

void changeSize(int w, int h) {
 
    float ratio;

    // place viewport to be the entire window
    glViewport(0, 0, w, h);
    ratio = (1.0f * w) / h;
    buildProjMatrix(53.13f, ratio, 1.0f, 200.0f);
}
 
void setupBuffers() {



	glGenBuffers(3, buffers);

	
	
}
 
void renderScene(void) {

	frame++;
	time1=glutGet(GLUT_ELAPSED_TIME);
	if (time1 - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",
			frame*1000.0/(time1-timebase));
		timebase = time1;
		frame = 0;
	}
    glutSetWindowTitle(s);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    //placeCam(10,2,10,0,2,-5);

	cam.placeCam(0,10,10,0,0,0);
	cam.zoomCam(zoom);
	cam.rotateCam(xAngle, yAngle);
	
	pTerrain->loadIntoBuffer(buffers[0], buffers[1], buffers[2]);
	pTerrain->getShader()->setLight(light);
	pTerrain->beginRender();
	pTerrain->setMatrices(cam.getMatrix(), projMatrix);
	pTerrain->render();

	pWater->loadIntoBuffer(buffers[0], buffers[1], buffers[2]);
	pWater->getShader()->setLight(light);
	pWater->beginRender();
	pWater->setMatrices(cam.getMatrix(), projMatrix);
	pWater->render();

	for(int i = 0; i < numGrass; i++) {
		grasses[i]->getShader()->setSampler(3);
		grasses[i]->loadIntoBuffer(buffers[0], buffers[1], buffers[2]);
		grasses[i]->getShader()->setLight(light);
		grasses[i]->getShader()->setTime(time1);
		grasses[i]->beginRender();
		grasses[i]->setMatrices(cam.getMatrix(), projMatrix);
		grasses[i]->render();
	}
	

	glutSwapBuffers();
}
 
void initShaders() {
 

	Shader *p = new TerrainShader(vertexFileName, fragmentFileName);
	Shader *p1 = new TerrainShader(vertexFileName, fragmentFileName);
	Shader *pG = new GrassShader(vertGrassFileName, fragGrassFileName);

	float gridSize = 1.0f;
	int size = 100;
	pTerrain = new Terrain(p, vec3(0.0f, 0.0f, 0.0f), size, size, gridSize, gridSize, false);
	pWater = new Terrain(p1, vec3(0.0f, -4.0f, 0.0f), size, size, gridSize, gridSize, true);

	map<pair<int,int>, bool> exist;
	for(int i = 0; i < numGrass; i++) {
		//srand (1);
		int x = rand() % size;
		
		//srand (1);
		int z = rand() % size;

		while(exist.find(pair<int,int>(x, z)) != exist.end()) {
			x = rand() % size;
			z = rand() % size;
		}
		exist[pair<int,int>(x, z)] = true;


		float xMid = - size * gridSize / 2 + x * gridSize + gridSize / 2;
		float zMid = - size * gridSize / 2 + z * gridSize + gridSize / 2;

		float yMid = pTerrain->getHeight(x, z);
		//float y = pTerrain->heightGenerator()
		grasses.push_back( new Grass(pG, vec3(xMid, yMid, zMid)));
	}

}

float deltaAngle = 0.0f;
int xOrigin = -1;



int xBase = 0;
int yBase = 0;
int curState = GLUT_UP;

void mouseMove(int x, int y) 
{
	xPick = x;
	yPick = y;
}

//This event will trigger when you have a mouse button pressed down.
void mouseDownMove(int x, int y) 
{

	// x and y is the mouse position.
	if(curState != GLUT_UP) {

		xAngle -= ( x - xBase ) / 100.0;
		yAngle -= ( y - yBase ) / 100.0;

		xBase = x;
		yBase = y;
	}
}

//This event occur when you press a mouse button.
void mouseButton(int button, int state, int x, int y) 
{
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) 
	{
		// when the button is released
		if (state == GLUT_UP) 
		{

		}
		else  // state = GLUT_DOWN	
		{

			xBase = x;
			yBase = y;
		}
		curState = state;
	}
}

void keyDown(unsigned char key, int x, int y) {
	if(key == 'z') 
	{
		zoom += 0.1;
	} 
	else if(key == 'x') 
	{
		zoom -= 0.1;
	}
	else if(key == 'a')
	{

	}
	else if(key == 'd')
	{
	}
}
 GLenum errorCode;
int main(int argc, char **argv) 
{

	// sets up glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(640,480);
    glutCreateWindow("ICS Graphics");
    glutSetWindowTitle(s);
	// call back functions
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
	changeSize(640,480);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseDownMove);
	glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc(keyDown);




	// check if a particular extension is available on your platform
    glewInit();
    if (glewIsSupported("GL_VERSION_3_3"))
        printf("OpenGL 3.3 is supported\n");
    else 
	{
        printf("OpenGL 3.3 not supported\n");
        //exit(1);
    }
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0,1.0,1.0,1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//glBlendEquation(GL_FUNC_SUBTRACT);


    initShaders();

	initTex();

    setupBuffers(); 
    glutMainLoop();
 
	delete(pTerrain);
	delete(pWater);
	for(int i = 0; i < numGrass; i++) {
		delete(grasses[i]);
	}

    return(0); 
}