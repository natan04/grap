#pragma once

#include <stdlib.h>
#include "glut.h"
#include "GL.h"
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include "data.h"

static GLuint globalView = 0;
enum Movenment {UP, RIGHT, LEFT, DOWN};

static Movenment direction;
static GLfloat angle = 60;
static GLuint rotUp = 0;
static GLfloat rotRight;
static  int xM,yM;

static float w, h, tip = 0, turn = 0;

static float ORG[3] = {0,0,0};

static float XP[3] = {00,0,0}, XN[3] = {-10,0,0},
	YP[3] = {0,10,0}, YN[3] = {0,-10,0},
	ZP[3] = {0,0,10}, ZN[3] = {0,0,-10};



using std::ifstream;
using namespace std;

static Data* data ;						//data that will hold anything
static std::vector<ReturnedFace*>* values;	//value returned
