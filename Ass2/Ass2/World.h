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
#define MAX_LEVEL			3

class World
{


public:
	GLuint WIDTH;
	GLuint HEIGHT;
	GLubyte* World::paint();
	GLubyte* World::paintFish();
	World(void);
	~World(void);
	void addToWorld(char* lineArg);
	GLboolean intersact(Ray& ray);
	GLuint ftexture;
	Eye* fEye;
	Vector3f	powerVector(Vector3f& vector, unsigned int pow);
	
private:
	GLfloat handleSpecular(Vector3f normal, Vector3f l, Vector3f v, GLfloat shininess);
	Ray World::generateRayReflecttion(Vector3f normal, Ray ray, Vector3f &intersection);
	Shape* World::FindIntersection(Ray ray,Vector3f& intersectionPoint, Vector3f &normal);
	Color World::getColor( Ray ray,  Vector3f &intersection, const Vector3f &normal,  Shape &shape, GLuint level);
	std::vector<Light*> fLights;
	std::vector<Shape*> fShapes;
	Color sanity(Color& color);
	
};

