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
		Color getAmbient(Point intersection) ;
	Ray generateTranRay(Point intersection, Vector3f direction, Vector3f normal);
private:
	Vector3f* fNormalToPlane;
	Vector3f* fCenterToPoint;
	Vector3f* fRight;
	Vector3f* fUp;
	
	GLfloat  fWidth;
	GLfloat	fLength;
		


};

