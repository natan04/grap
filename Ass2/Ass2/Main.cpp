#include <stdlib.h>
#include "glut.h"
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include "World.h"

		

using std::ifstream;
using namespace std;


void readFromFile(char* value, World& world)
{
	//open file
	ifstream fin;
	fin.open(value);
	if (!fin.good()) 
		exit(1); // exit if file not found
 	
	char buf[MAX_CHARS_PER_LINE];
	while (fin.getline(buf, MAX_CHARS_PER_LINE))
	{
		
		world.addToWorld(buf);
		memset(&buf, 0, MAX_CHARS_PER_LINE);
	}
	fin.close();
}

int main(int  argc,  char** argv) 
{
	World world;
	if  (argc ==  2)
		readFromFile(argv[1], world);
	else
		exit(1);


}