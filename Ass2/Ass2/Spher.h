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
	Ray generateTranRay(Point intersection, Vector3f direction, Vector3f normal);
	Color getAmbient(Point intersection) ;
	Shape*  Spher::findIntersectionTrans(Ray ray, Vector3f& willReturn, Vector3f& normal );

private:
	Vector3f* fCenterCoordinate;

	GLfloat fRadius;
	GLfloat fRadiusSquare;
	GLfloat fRefractiveIndex;
	Ray Spher::generateRayReflecttion(Vector3f normal, Ray ray, Vector3f &intersection);
	

};

