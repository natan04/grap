#pragma once
#include "shape.h"
#define SEP ","
#include <iostream>
#include "glut.h"
#include "Vector3f.h"

class Plane :
	public Shape
{
public:
	Plane(char* arg);
	~Plane(void);

private:
	Vector3f* fNormalToPlane;
	Vector3f* fCenterToPoint;
	GLfloat  fWidth;
	GLfloat	fLength;
	Vector3f* fKa;
	Vector3f* fKd;
	Vector3f* fKs;
	GLfloat fShininess;
	GLboolean fMirror;
};

