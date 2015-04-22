#pragma once
#define SEP ","
#include <iostream>
#include "Structs.h"
#include "glut.h"
#include "Vector3f.h"


class Eye
{
public:
	Eye(char* args);
	~Eye(void);
	void init(void);
	Vector3f*	fCameraLocation;
	
	Vector3f*	fScreenCenter;
	Vector3f*	fUpVector;
	Vector3f*	fTowardVector;
	Vector3f*	fRightVector;
	GLfloat		fWidth;
	GLfloat		fPixelToWorld;
	GLuint		fRx;
	GLuint		fRy;
	GLuint		fRxD2;
	GLuint		fRyD2;
	Vector3f*	fAmbientLighting;
	Ray Eye::generateRay(GLuint aX, GLuint aY);
	


};

