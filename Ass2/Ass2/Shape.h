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
		virtual Ray generateTranRay(Point intersection, Vector3f direction, Vector3f normal) = 0;
		virtual Color getAmbient(Point intersection) = 0;

		Color* fKa;
		Color* fKd;
		GLfloat fShininess;
		Color* fKs;
		Color* fKt;
		Color* fKr;
		GLboolean fMirror;
};