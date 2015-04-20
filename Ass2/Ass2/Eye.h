#pragma once
#define SEP ","
#include <iostream>
#include "glut.h"
#include "Vector3f.h"


class Eye
{
public:
	Eye(char* args);
	~Eye(void);
	void init(void);
	Vector3f*	fScreenCenter;
	Vector3f*	fUpVector;
	GLfloat		fWidth;
	GLuint		fRx;
	GLuint		fRy;
	Vector3f*	fAmbientLighting;

	


};

