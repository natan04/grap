#pragma once
#include <stdlib.h>
#include "glut.h"
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include <vector>
#include "Light.h"
#include "Eye.h"
#include "Shape.h"
#include "Spher.h"
#include "Plane.h"


#define MAX_CHARS_PER_LINE	200
#define MAX_TOKENS_PER_LINE 18
#define MAX_NAME_SIZE		10
#define DELIMITER			","
#define SPACE				" "

class World
{


public:

	GLubyte* World::paint();
	World(void);
	~World(void);
	void addToWorld(char* lineArg);
	GLboolean intersact(Ray& ray);
	GLuint ftexture;
	Eye* fEye;
	Vector3f	powerVector(Vector3f& vector, unsigned int pow);
	
private:
	Shape* World::FindIntersection(Ray ray,Vector3f& intersectionPoint, Vector3f &normal);
	Color World::getColor( Ray ray,  Vector3f &intersection, const Vector3f &normal,  Shape &shape);
	std::vector<Light*> fLights;
	std::vector<Shape*> fShapes;
	
	
};

