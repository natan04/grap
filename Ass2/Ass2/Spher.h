#pragma once
#include "Shape.h"
#define SEP ","
#include <iostream>
#include "glut.h"
#include "Vector3f.h"

class Spher:
	public Shape
{
public:
	Spher(char* arg);
	~Spher(void);
	GLboolean  findIntersectionPoint(Vector3f& startPoint, Vector3f& direction, Vector3f& willReturn, Vector3f& normal );
private:
	Vector3f* fCenterCoordinate;
	Vector3f* fKa;
	Vector3f* fKd;
	Vector3f* fKs;
	GLfloat fRadius;
	GLfloat fRadiusSquare;
	GLfloat fShininess;
	GLboolean fMirror;

};

