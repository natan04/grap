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

GLuint texture;
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
	glClear(GL_COLOR_BUFFER_BIT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
	
	glBindTexture(GL_TEXTURE_2D, texture); 
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); //adapt texture to shape
		glVertex3f(-1.0 ,0.0 ,1.0);
		
		glTexCoord2f(1, 0);  //adapt texture to shape
		glVertex3f(0.0f ,0.0f, 1.0);
		
		glTexCoord2f(1, 1);//adapt texture to shape
		glVertex3f(0.0 , 1.0f,1.0);
		
		glTexCoord2f(0, 1);//adapt texture to shape
		glVertex3f(-1 , 1.0f,1.0);
	glEnd();
	

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
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB) ;
	glutInitWindowSize (world.fEye->fRx, world.fEye->fRy) ;
	 glutCreateWindow("First example") ;
	glGenTextures(1, &texture);		
	glBindTexture(GL_TEXTURE_2D, texture);	
	//texture properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//build texture
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, world.fEye->fRx,  world.fEye->fRy, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, world.paint());
	
	 // glutIdleFunc(display);
	  glutDisplayFunc(display); 
     glutMainLoop();

}



