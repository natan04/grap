#pragma once
#define SEP ","
#include <iostream>
#include "glut.h"
#include "Vector3f.h"
#include <math.h> 

class Shape
{
	public:	
		Shape(){}
		virtual ~Shape(){}
		virtual GLboolean findIntersectionPoint(Vector3f& startPoint, Vector3f& direction, Vector3f& willReturn, Vector3f& normal ) = 0;

};