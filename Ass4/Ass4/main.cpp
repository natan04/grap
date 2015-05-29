#include <stdlib.h>
#include "glut.h"
#include "GL.h"
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include "data.h"
#define stringify( name ) # name
#define MAX_CHARS_PER_LINE 500

enum MODE {CAMERA, GLOBAL };

MODE sceneMode;


GLfloat modelMatrix[16];
GLfloat transCamera[16];
GLdouble rotXcam, rotYcam;
GLdouble scale = 1;

int mouseState;
GLfloat angle = 60;
GLuint rotUp = 0;
GLfloat rotRight;    
int xM,yM;
int xx,yy,ii,jj;
float zz;

float w, h, tip = 0, turn = 0;

float ORG[3] = {0,0,0};

float XP[3] = {00,0,0}, XN[3] = {-100,0,0},
	YP[3] = {0,0,0}, YN[3] = {0,-100,0},
	ZP[3] = {0,0,0}, ZN[3] = {0,0,-100};

using std::ifstream;
using namespace std;
GLubyte* picture;
GLuint tex;
Data* data ;
std::vector<ReturnedFace*> values;




void disp()
{

		glutPostRedisplay();
}

void initLight()
{
	//lightning
	GLfloat emission[] ={0,0,0.2};
	GLfloat light_direction[]={0,-1.0,-1.0,0.0};
	GLfloat light_ambient[] = {0.5, 0.5, 0.5, 0.0}; //color
	GLfloat light_diffuse[] = {0.0, 0.5, 0.5, 0.0}; //color
	GLfloat light_specular[] = {0.0, 0.0, 0.5, 0.0}; 
	GLfloat light_position[]={-0.0,-1.5,1,0,0.0};
	//GLfloat angle[] = {20.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	//glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF,angle);

	glShadeModel(GL_FLAT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	GLfloat mat_a[] = {0.3,0.4,0.5,1};
	GLfloat mat_d[] = {0.0,0.6,0.7,1};
	GLfloat mat_s[] = {0.0,0.0,0.8,1};
	GLfloat low_sh[] = {5.0};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);

	glEnable(GL_COLOR_MATERIAL);
	//	glDisable(GL_LIGHTING);
		glLoadIdentity();

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
	 glPushAttrib(GL_CURRENT_BIT);
	 glMatrixMode(GL_MODELVIEW);

	glBegin (GL_LINES);
	glColor3f (1,0,0); // X axis is red.
	glVertex3f (0,0,0);
	glVertex3f (100,0,0); 
	glBegin (GL_LINES);
	glColor3f (0,0,1); // Y axis is green.
	glVertex3f (0,0,0);
	glVertex3f (0,100,0);
	glBegin (GL_LINES);
	glColor3f (0,1,0); // z axis is blue.
	glVertex3f (0,0,0);
	glVertex3f (0,0,-100);
	glEnd();

	glPopAttrib();
	glPopMatrix ();
}

void disp(int value)
{
	glutPostRedisplay();
}

void spaciel(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP : 
		scale += 0.05;
		disp();
		break;

	case  GLUT_KEY_DOWN : 
		scale -= 0.05;
		disp();
		break;

	case GLUT_KEY_F2:
		glMatrixMode(GL_PROJECTION); /* switch matrix mode */
		glLoadIdentity();
		gluPerspective(angle += 0.5 ,1,2,200);

		disp();
		

		break;


	case GLUT_KEY_F3:
		glMatrixMode(GL_PROJECTION); /* switch matrix mode */
		glLoadIdentity();
		gluPerspective(angle -= 0.5 ,1,2,200);

		disp();
		glutPostRedisplay();

		break;



	}
}


void Keyboard (unsigned char key, int x, int y)
{
	switch (key) {

	case ' ' :  
		if (sceneMode == CAMERA)
			sceneMode = GLOBAL;
		else
			sceneMode = CAMERA;

		cout << "The current mode:" << sceneMode << endl;;
	}

	glutPostRedisplay();
}


void mouse(int button, int state, int x, int y) 
{

		xx=x;
		yy=y;
		mouseState = button;
}

void display()
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glDisable(GL_NORMALIZE);
			glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(modelMatrix);
		glRotated(rotXcam, 1, 0 ,0);
		glRotated(rotYcam, 0, 1 ,0);
		glMultMatrixf(transCamera);
		Draw_Axes();
		
		glScaled(scale, scale, scale);

	for (std::vector<ReturnedFace*>::iterator it = values.begin(); it != values.end(); ++it){

		GLuint runner;

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
		//Draw_Axes();

	glFlush();

}



void init()
{
	
	glClearColor(0,0,0,1);

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();		//load Identity matrix

	//defines view mode
	gluPerspective(60,1,2,200);
	

	glEnable(GL_DEPTH_TEST);  //define in which order the scene will built
	glEnable(GL_NORMALIZE);

	glDepthFunc(GL_LESS);
	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
	glTranslatef(0,0,-100);
	glGetFloatv(GL_MODELVIEW_MATRIX, transCamera);
	glPopMatrix();
//		
	
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);


}



void motion(int x,int y)
{
	GLint m_viewport[4];
	glGetIntegerv( GL_VIEWPORT, m_viewport );
	int dy = y - yy;
	int dx = -x + xx;
	xx = x;
	yy = y;   

switch (sceneMode){

case  GLOBAL:
			switch (mouseState){

			case GLUT_LEFT_BUTTON:
	
				glMatrixMode(GL_MODELVIEW);

				glPushMatrix();		
				glLoadMatrixf(modelMatrix);
				glRotatef( dy*180/m_viewport[3], 1 ,0,0 );
				glRotatef( dx*180/m_viewport[2], 0 ,0,1 );
		
				glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
				glPopMatrix();
	
				disp();
				break;
	


			case GLUT_MIDDLE_BUTTON:	
				
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();		
				glLoadMatrixf(modelMatrix);
				glTranslatef( 0 ,0,  dy*180/m_viewport[3] );
	
				glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
				glPopMatrix();
	
				disp();
				break;

			case GLUT_RIGHT_BUTTON:	
			
				glMatrixMode(GL_MODELVIEW);

				glPushMatrix();		
				glLoadMatrixf(modelMatrix);
				glTranslatef( 0,  dy*180/m_viewport[3], 0 );
				glTranslatef( dx*180/m_viewport[2], 0,0 );

		
				glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
				glPopMatrix();
	
				disp();
				break;
			}
break;

case CAMERA:
	switch (mouseState){
		
	case GLUT_LEFT_BUTTON:
	
				glMatrixMode(GL_MODELVIEW);

				rotXcam +=  dy*180/m_viewport[3];
				rotYcam +=  dx*180/m_viewport[2];
		
				glPopMatrix();
	
				disp();
				break;

	case GLUT_RIGHT_BUTTON:	
			
				glMatrixMode(GL_MODELVIEW);

				glPushMatrix();		
				glLoadMatrixf(transCamera);
				glTranslatef( 0,  dy*50/m_viewport[3], 0 );
				glTranslatef( dx*50/m_viewport[2], 0,0 );

		
				glGetFloatv(GL_MODELVIEW_MATRIX, transCamera);
				glPopMatrix();
	
				disp();
				break;

		case GLUT_MIDDLE_BUTTON:	
							
			glMatrixMode(GL_MODELVIEW);

			glPushMatrix();		
			glLoadMatrixf(transCamera);
			glTranslatef( 0,0,  -dy*50/m_viewport[3] );
				
		
			glGetFloatv(GL_MODELVIEW_MATRIX, transCamera);
			glPopMatrix();
	
				disp();
				break;

	}

			


break;

}
	
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
	   glutMotionFunc(motion);

	glutMouseFunc(mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(spaciel);
	disp();
	  

	glutMainLoop();

}


