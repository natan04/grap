#pragma once
#define SEP ","
#include <iostream>
#include "glut.h"
#include "Vector3f.h"


class Light
{
public:
	Light(char* values);
	~Light(void);

private:
	Vector3f* fDirection;
	Vector3f* fIntensity; 
	Vector3f* fSpotlight;
	GLfloat	  fCutoff;
	

};

