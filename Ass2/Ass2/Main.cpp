#include <stdlib.h>
#include "glut.h"
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include "World.h"

World world;

using std::ifstream;
using namespace std;
	GLubyte* picture;
GLuint tex;
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


void display()
{

		
	glDrawPixels( world.WIDTH, world.HEIGHT, GL_RGB , GL_FLOAT, picture);
	
	glFlush();

}
void bla()
{
	  glutDisplayFunc(display); 
}

int main(int  argc,  char** argv) 
{

	if  (argc ==  2)
		readFromFile(argv[1], world);
	else
		exit(1);


	glutInit (& argc, argv) ;

	picture = world.paint();
	

	glutInitWindowSize ( world.WIDTH, world.HEIGHT) ;

	glutInitDisplayMode ( GLUT_RGB) ;
		
	 glutCreateWindow("Example") ;

	glutDisplayFunc(display); 
     glutMainLoop();

}



