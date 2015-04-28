#pragma once
#define SEP ","
#include <iostream>
#include "glut.h"
#include "shape.h"
#include "Vector3f.h"
#include "Structs.h"
#include <stdlib.h>
#include <vector>
#define RAD2DEG(x) (x / M_PI * 180)
class Light
{
public:
	Light(char* values);
	~Light(void);

Light* findIntersection(Vector3f& intersectionPoint,std::vector<Shape*> & shapes ,Vector3f directionOfSource);
GLboolean Light::intersact(Ray& ray, std::vector<Shape*> & shapes, Vector3f directionOfSource, Vector3f& intersection);
	Vector3f*	 fDirection;
	Vector3f* fIntensity; 
	Vector3f* fSpotlight;
	GLfloat	  fCutoff;
	
private:
};

