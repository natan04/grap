#pragma once
#include "shape.h"
#define SEP ","
#include <iostream>
#include "glut.h"
#include "Vector3f.h"
#include "Structs.h"

class Plane :
	public Shape
{
public:
	Plane(char* arg);
	~Plane(void);
	Shape*  findIntersectionPoint(Ray ray, Vector3f& willReturn, Vector3f& normal );

private:
	Vector3f* fNormalToPlane;
	Vector3f* fCenterToPoint;
	GLfloat  fWidth;
	GLfloat	fLength;

	GLboolean fMirror;
};

