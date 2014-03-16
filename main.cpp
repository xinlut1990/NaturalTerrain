#include <stdio.h>
#include <stdlib.h>
#include "glew\include\GL\glew.h"
#include "glut\glut.h"



#include "fileAccessor.h"
#include "myMath.h"
#include "SOIL.h"
#include "data.h"
#include "Camera.h"
#include "Light.h"
#include "bitmap.h"

// shader names
char *vertexFileName = "vertex.txt";
char *fragmentFileName = "frag.txt";

char *vertPickingFileName = "vert_picking.txt";
char *fragPickingFileName = "frag_picking.txt";
unsigned char *image;

GLuint pickingProgram;
GLuint p;

int xPick;
int yPick;

GLubyte pixel[3];

// vert attrib locations
GLuint vertexLoc,vertex1Loc = 9, colorLoc, normalLoc, texLoc;
 
// uniform var locations
GLuint projMatrixLoc, viewMatrixLoc;
GLuint projMatrix1Loc , viewMatrix1Loc ;
GLuint timeLoc;
GLuint lightLoc;
GLuint samplerLoc;
GLuint samplerNormalLoc;


GLuint diffLoc;
GLuint specLoc;

GLuint matDiffLoc;
GLuint matSpecLoc;

GLuint idLoc = 8;
GLuint id1Loc;
GLuint curIdLoc;

int curId = 0;
 
// vert array obj Id
GLuint vert[3];
 
// storage for matrices
float projMatrix[16];
float worldMatrix[16];

Camera cam;

Light light(vec4(4.0, 5.0, 5.0, 1.0), color(0.5, 0.5, 0.5, 1.0), color(0.5, 0.5, 0.5, 1.0));

	GLuint texName[2];


int frame=0,time,timebase=0;

float xAngle = 0.0f;
float yAngle = 0.0f;

float zoom = 0.0f;

char s[50];
 
void initTex() {
	
	glActiveTexture(GL_TEXTURE1);
	texName[0] = SOIL_load_OGL_texture
		(
		"creeper.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);
	

	glActiveTexture(GL_TEXTURE2);
	texName[1] = SOIL_load_OGL_texture
		(
		"normal.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);


	//glBindTexture(GL_TEXTURE_2D, texName[0]);
	

	
	//glBindTexture(GL_TEXTURE_2D, texName[1]);
	
}
 

void Init2DTextures(UINT textureArray[], char *bmpFileName, int ID)
{
	BITMAPINFO* TexInfo;
	GLubyte* TexBits = LoadDIBitmap(bmpFileName, &TexInfo);
	if(TexBits == NULL)	exit(0);
	
	glGenTextures(1, &textureArray[ID]);
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, TexInfo->bmiHeader.biWidth,TexInfo->bmiHeader.biHeight, 0, GL_BGR_EXT,GL_UNSIGNED_BYTE,TexBits);
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
    buildProjMatrix(53.13f, ratio, 1.0f, 30.0f);
}
 
void setupBuffers() {


    GLuint buffers[6];
 
    //glGenVertexArrays(1, vert);

    // first triangle
    //glBindVertexArray(vert[0]);
    // generate 2 buffers for vert and color
    glGenBuffers(7, buffers);




    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
 
    // bind buffer for colors and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal), normal, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normalLoc);
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texLoc);
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(id), id, GL_STATIC_DRAW);
    glEnableVertexAttribArray(id1Loc);
    glVertexAttribPointer(id1Loc, 1, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(id), id, GL_STATIC_DRAW);
    glEnableVertexAttribArray(idLoc);
    glVertexAttribPointer(idLoc, 1, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertex1Loc);
    glVertexAttribPointer(vertex1Loc, 3, GL_FLOAT, 0, 0, 0);

}
 
void setUniforms() {
 
    // must be called after glUseProgram
	// set the variables for the shader
    glUniformMatrix4fv(projMatrixLoc,  1, false, projMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, false, cam.getMatrix());
	glUniform1f(timeLoc, time);


	glUniform1i(samplerLoc , 1);
		glUniform1i(samplerNormalLoc , 2);
	glUniform1i(curIdLoc, curId);

	vec4 lightPos = light.getPosition();
	glUniform4f(lightLoc, lightPos.x, lightPos.y, lightPos.z, lightPos.w);

	color diff = light.getIntensityDiff();
	glUniform4f(diffLoc, diff.r, diff.g, diff.b, diff.a);

	color spec = light.getIntensitySpec();
	glUniform4f(specLoc, spec.r, spec.g, spec.b, spec.a);

	color matDiff(1.0, 1.0, 1.0, 1.0);
	glUniform4f(matDiffLoc, matDiff.r, matDiff.g, matDiff.b, matDiff.a);

	color matSpec(1.0, 1.0, 1.0, 1.0);
	glUniform4f(matSpecLoc, matSpec.r, matSpec.g, matSpec.b, matSpec.a);



}

void setUniformsPick()
{
	glUniformMatrix4fv(projMatrix1Loc,  1, false, projMatrix);
    glUniformMatrix4fv(viewMatrix1Loc,  1, false, cam.getMatrix());
}
 
void renderScene(void) {

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",
			frame*1000.0/(time-timebase));
		timebase = time;
		frame = 0;
	}
    glutSetWindowTitle(s);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    //placeCam(10,2,10,0,2,-5);

	cam.placeCam(0,0,5,0,0,0);
	cam.zoomCam(zoom);
	cam.rotateCam(xAngle, yAngle);
    


    //glBindVertexArray(vert[0]);
	//glDrawArrays(GL_TRIANGLES,0,36);
    //glutSwapBuffers();

	glUseProgram(pickingProgram);
	setUniformsPick();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glutSwapBuffers();


	//printf("X: %d, Y: %d\n",xPick,yPick);
	glReadPixels(xPick, yPick,1,1,GL_RGB, GL_UNSIGNED_BYTE, (void*)pixel);
	//printf("R: %d, G: %d, B: %d\n",pixel[0],pixel[1],pixel[2]);

	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glUseProgram(p);	
	setUniforms();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glutSwapBuffers();
}
 

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
 
void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

GLuint buildShader(char* vertexFileName, char* fragmentFileName) {
	// program and shader Id
	GLuint p;
	GLuint v,f;
	char *vertShader = NULL,*fragShader = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    vertShader = getTxtFile(vertexFileName);
    fragShader = getTxtFile(fragmentFileName);
    const char * vv = vertShader;
    const char * ff = fragShader;
    glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);
    free(vertShader);
	free(fragShader);
    glCompileShader(v);
    glCompileShader(f);
    printShaderInfoLog(v);
    printShaderInfoLog(f);
    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);
    glLinkProgram(p);
    printProgramInfoLog(p);

	return p;
}
 
void initShaders() {
 

	p = buildShader(vertexFileName, fragmentFileName);

	pickingProgram = buildShader(vertPickingFileName, fragPickingFileName);
	glBindAttribLocation(pickingProgram, idLoc, "id");
    glBindAttribLocation(pickingProgram, vertex1Loc, "position"); 

	glLinkProgram(pickingProgram);

	//idLoc = glGetAttribLocation(pickingProgram,"id");
    //vertex1Loc = glGetAttribLocation(pickingProgram, "position"); 
	
	projMatrix1Loc = glGetUniformLocation(pickingProgram, "projMatrix1");
    viewMatrix1Loc = glGetUniformLocation(pickingProgram, "viewMatrix1");
	
	
	id1Loc = glGetAttribLocation(p, "id");
	vertexLoc = glGetAttribLocation(p,"position");
    colorLoc = glGetAttribLocation(p, "color"); 
	normalLoc = glGetAttribLocation(p, "normal");
	texLoc = glGetAttribLocation(p, "texCoord");

    projMatrixLoc = glGetUniformLocation(p, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
	timeLoc = glGetUniformLocation(p, "time");
	lightLoc = glGetUniformLocation(p, "lightPos");
	samplerLoc = glGetUniformLocation(p, "tex");
	samplerNormalLoc = glGetUniformLocation(p, "texNormal");

	diffLoc = glGetUniformLocation(p, "intensityDiff");
	specLoc = glGetUniformLocation(p, "intensitySpec");

	matDiffLoc = glGetUniformLocation(p, "materialDiff");
	matSpecLoc = glGetUniformLocation(p, "materialSpec");

	curIdLoc = glGetUniformLocation(p, "curId");

		


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

			curId = (ceil((float)pixel[1] / 64.0) - 1) * 4 + (ceil((float)pixel[2] / 64.0) - 1);
			printf("curId: %d, %d,%d,%d\n",curId, pixel[0], pixel[1], pixel[2]);

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
    glClearColor(0.0,0.0,0.0,1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//glBlendEquation(GL_FUNC_SUBTRACT);
    initShaders();

	initTex();

    setupBuffers(); 
    glutMainLoop();
 
    return(0); 
}