#pragma once
#define SEP ","
#include <iostream>
#include "glut.h"
#include "Vector3f.h"
#include <math.h> 
#include "Structs.h"
class Shape
{
	
	public:	
		Shape(){}
		virtual ~Shape(){}
		virtual Shape* findIntersectionPoint(Ray ray, Vector3f& willReturn, Vector3f& normal ) = 0;
		Vector3f* fKa;
		Vector3f* fKd;
		GLfloat fShininess;
		Vector3f* fKs;
};