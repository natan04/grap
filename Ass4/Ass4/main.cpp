#include <stdlib.h>
#include "glut.h"
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include "data.h"

#define MAX_CHARS_PER_LINE 100

using std::ifstream;
using namespace std;
	GLubyte* picture;
GLuint tex;
Data* data ;

void initLight()
{
	//lightning
	GLfloat emission[] ={0,0,0.2};
	GLfloat light_direction[]={0,-1.0,-1.0,0.0};
	GLfloat light_ambient[] = {0.5, 0.5, 0.5, 0.0}; //color
	GLfloat light_diffuse[] = {0.0, 0.5, 0.5, 0.0}; //color
	GLfloat light_specular[] = {0.0, 0.0, 0.5, 0.0}; 
	//GLfloat light_position[]={-0.0,-1.5,1,0,0.0};
	//GLfloat angle[] = {20.0};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	//glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF,angle);
	
	glShadeModel(GL_FLAT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	GLfloat mat_a[] = {0.1, 0.5, 0.5, 1.0};
	GLfloat mat_d[] = {0.1, 0.7, 0.7, 1.0};
	GLfloat mat_s[] = {1.0, 0.0, 0.9, 1.0};
	GLfloat low_sh[] = {5.0};
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);
	
	glEnable(GL_COLOR_MATERIAL);
//	glDisable(GL_LIGHTING);

}

void readFromFile(char* value, Data* data)
{
	////open file
	ifstream fin;
	fin.open(value);
	if (!fin.good()) 
		exit(1); // exit if file not found
 	
	char buf[MAX_CHARS_PER_LINE];
	while (fin.getline(buf, MAX_CHARS_PER_LINE))
	{

		data->add(buf);
		memset(&buf, 0, MAX_CHARS_PER_LINE);
	}
	fin.close();
}



void display()
{

		
	data->paint();
	glFlush();

}

int main(int  argc,  char** argv) 
{
	 data = new Data();
	if  (argc >=  2)
		readFromFile(argv[1], data);
	else

		exit(1);

 glutInit (& argc, argv) ;
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB) ;
   glutInitWindowSize ( 512,512) ;
   glutCreateWindow("Lighting") ;
glMatrixMode(GL_PROJECTION);

glLoadIdentity();
glEnable(GL_DEPTH_TEST);

initLight();
	glutDisplayFunc(display); 
    glutMainLoop();

}


