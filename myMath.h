#pragma once
#include <memory.h>
#include <math.h>

#define PI       3.14159265358979323846

struct vec3
{
	vec3();
	vec3(float x, float y, float z);
	vec3(const vec3& vec);
	float x;
	float y;
	float z;
};

struct vec4
{
	vec4();
	vec4(float x, float y, float z, float w);
	vec4(const vec4& vec);
	float x;
	float y;
	float z;
	float w;
};

struct color
{
	color();
	color(float r, float g, float b, float a);
	color(const color& c);
	float r;
	float g;
	float b;
	float a;
};

// vector opt
// res = a cross b;
void xProduct( float *a, float *b, float *res);
 
// normalize a vec3
void normalize(float *a);

// Matrix Opt. - In Opengl 3 we need to handle our own matrix.
 
// In this form : a = a * b; 
void multiplyMatrix(float *a, float *b);

// sets the square matrix mat to the ID matrix,
void setIdentMatrix( float *mat, int size);

// Generates a rotation matrix.  Angle is in radian.
float * rotationMatrix(float x, float y, float z, float angle);
// Transformation matrix mat with a translation
void setTransMatrix(float *mat, float x, float y, float z);