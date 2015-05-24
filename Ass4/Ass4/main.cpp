#include <stdlib.h>
#include "glut.h"
#include "GL.h"
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include "data.h"

#define MAX_CHARS_PER_LINE 100

enum Movenment {UP, RIGHT, LEFT, DOWN};

Movenment direction;

GLuint rotUp = 0;
GLfloat rotRight;
int xM,yM;

float w, h, tip = 0, turn = 0;

float ORG[3] = {0,0,0};

float XP[3] = {00,0,0}, XN[3] = {-10,0,0},
YP[3] = {0,10,0}, YN[3] = {0,-10,0},
ZP[3] = {0,0,10}, ZN[3] = {0,0,-10};

using std::ifstream;
using namespace std;
	GLubyte* picture;
GLuint tex;
Data* data ;
std::vector<ReturnedFace*> values;


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

void Draw_Axes (void)
{
glPushMatrix ();

glTranslatef (-2.4, -1.5, -5);
glRotatef (tip , 1,0,0);
glRotatef (turn, 0,1,0);
glScalef (0.25, 0.25, 0.25);

glLineWidth (2.0);

glBegin (GL_LINES);
glColor3f (1,0,0); // X axis is red.
glVertex3fv (ORG);
glVertex3fv (XP ); 
glColor3f (0,0,1); // Y axis is green.
glVertex3fv (ORG);
glVertex3fv (YP );
glColor3f (0,1,0); // z axis is blue.
glVertex3fv (ORG);
glVertex3fv (ZP ); 
glEnd();

glPopMatrix ();
}

void disp(int value)
{
	glutPostRedisplay();
}

void Keyboard (unsigned char key, int x, int y)
{
    switch (key) {
 
       case ' ' :  
		   				  glMatrixMode(GL_MODELVIEW);
				  glTranslatef(0,10,0);
				  glutPostRedisplay();
		   break;
       case GLUT_KEY_UP : 
		       glScalef( 1 + 0.05 ,1+ 0.05 ,1.0f + 0.05 );
			   glutPostRedisplay();
		     break;

       case  GLUT_KEY_DOWN : 
		    glScalef( 1 -0.05,1 - 0.05,1.0f - 0.05 );
		    glutPostRedisplay();
		break;

 
       default: printf ("   Keyboard %c == %d\n", key, key);
    }
 
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) 
{
   switch (button) {
	  case GLUT_LEFT_BUTTON:
		 		
		 if (state == GLUT_DOWN)
		 {
			 yM = y;
		 }
		  
		  if (yM > y &&  state == GLUT_UP	)
		  {
			  yM = y;

			rotUp +=180;
		  	glutPostRedisplay();
		  }

	  break;
	  case GLUT_MIDDLE_BUTTON:
		  if (state == GLUT_DOWN){

				  glMatrixMode(GL_MODELVIEW);
				  glTranslatef(0,0,-20);
				  glutPostRedisplay();
			}
		  break;

  case GLUT_RIGHT_BUTTON:
		  if (state == GLUT_DOWN){

				  glMatrixMode(GL_MODELVIEW);
				  glTranslatef(0,0,20);
				  glutPostRedisplay();
			}
		  break;
   }
}

void display()
{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		glRotatef(rotUp,1,0,0); //rotate scene
		rotUp = 0;
		Draw_Axes ();

	for (std::vector<ReturnedFace*>::iterator it = values.begin(); it != values.end(); ++it){
	
		int runner;

		glMaterialfv(GL_FRONT, GL_AMBIENT, Vector3f(0.3, 0.4, 0.5));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector3f(0.0, 0.6, 0.7));
		glMaterialfv(GL_FRONT, GL_SPECULAR, Vector3f(0.0, 0.0, 0.8));

		if ((*it)->count > 3)
			glBegin(GL_POLYGON);
		else		
			glBegin(GL_TRIANGLES);
		
		
		for (runner = 0; runner < (*it)->count; runner++)
		{

			glVertex3fv((GLfloat*)(*it)->vetexs[runner]);
			glNormal3fv((GLfloat*)(*it)->normals[runner]);
		}
		glEnd();


	}
	
	glFlush();

}

void init()
{
	float modelMatrix[16],projectionMatrix[16];
	glClearColor(0,0,0,1);
	
	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();		//load Identity matrix

	//defines view mode
	gluPerspective(60,1,2,200);
	//glRotatef(180,0,1,0);
	glTranslatef(0,0,-100);
	
	//glTranslatef(0,1,0);
	//gluLookAt(0,-1,-1,0,-2,-2,1,1,0);  //define view direction
	//gluLookAt(0,-1,-1,0,0,0,1,1,0);  //define view direction
	

	glEnable(GL_DEPTH_TEST);  //define in which order the scene will built
	glDepthFunc(GL_LESS);
	 /* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}





int main(int  argc,  char** argv) 
{
	 data = new Data();
	if  (argc >=  2)
		readFromFile(argv[1], data);
	else

		exit(1);

	values = data->paint();
 
		   glutInit (& argc, argv) ;
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB) ;
   glutInitWindowSize ( 500,500) ;
   glutCreateWindow("Lighting") ;
  
   
   init();

	initLight();
	
	glutDisplayFunc(display); 
	glutTimerFunc(2,disp,0);
		glutMouseFunc(mouse);
		glutKeyboardFunc(Keyboard);
		


    glutMainLoop();

}


