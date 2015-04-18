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

private:
	Vector3f* fCenterCoordinate;
	Vector3f* fKa;
	Vector3f* fKd;
	Vector3f* fKs;
	GLfloat fRadius;
	GLfloat fShininess;
	GLboolean fMirror;

};

