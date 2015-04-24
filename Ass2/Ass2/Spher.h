#pragma once
#include "Shape.h"
#define SEP ","
#include <iostream>
#include "glut.h"
#include "Vector3f.h"
#include "Structs.h"

class Spher:
	public Shape
{
public:
	Spher(char* arg);
	~Spher(void);
	Shape*  findIntersectionPoint(Ray ray, Vector3f& willReturn, Vector3f& normal );
	Color getAmbient(Point intersection) ;
private:
	Vector3f* fCenterCoordinate;

	GLfloat fRadius;
	GLfloat fRadiusSquare;
	
	

};

