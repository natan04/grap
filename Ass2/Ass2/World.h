#pragma once
#include <stdlib.h>
#include "glut.h"
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#define MAX_CHARS_PER_LINE	200
#define MAX_TOKENS_PER_LINE 18
#define MAX_NAME_SIZE		10
#define DELIMITER			","
#define SPACE				" "

class World
{


public:
	World(void);
	~World(void);

	void addToWorld(char* lineArg);
};

