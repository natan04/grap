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
Face* parseFaces(char* buf);
enum MODE {CAMERA, GLOBAL };
MODE sceneMode;

GLfloat transCamera[16];
GLfloat rotateCamera[16];
GLfloat rotateGlobalMatrix[16];
GLfloat transGlobalMatrix[16];

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

typedef struct calculated_object
{
	GLuint name;
	std::vector<ReturnedFace*>* values;
	GLfloat rotation[16];
	GLfloat trans[16];
} CalObjects;
CalObjects* objects;

GLuint howManyObjects ;

void disp()
{

}

void initLight()
{

	//lightning
    GLfloat light_direction[]={0,-1,-1,0};
    GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1.0}; //ambient color
    GLfloat light_diffuse[] = {0.0, 0.5, 0.5, 1.0}; //diffuse color
    GLfloat light_specular[] = {0.0, 0.0, 0.5, 1.0};
	//GLfloat angle[] = {20.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_direction);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	 GLfloat mat_a[] = {0.4, 0.4, 0.5, 1.0};
    GLfloat mat_d[] = {0.0, 0.6, 0.7, 1.0};
    GLfloat mat_s[] = {0.0, 0.0, 0.4, 1.0};
    GLfloat low_sh[] = {5.0};

	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_s);

	  glEnable(GL_COLOR_MATERIAL);

}

void readFromFile(char* value, Data &objects, vector<vector<Face*>*>& faces)
{
	////open file
	ifstream fin;
	fin.open(value);
	if (!fin.good()) 
		exit(1); // exit if file not found

	char buf[MAX_CHARS_PER_LINE];

	vector<Face*>* facesObject;  
	while (fin.getline(buf, MAX_CHARS_PER_LINE))
	{

		if (buf[0] == '#' || std::strlen(buf) == 0)
			continue;

		char* name =  strtok(buf, SEP); // first token	
		char* rest = strtok(NULL, "");
		
		if (strcmp(name, "o") == 0)
		{
			facesObject = new vector<Face*>();
			faces.push_back(facesObject);
			continue;
		}
		if (strcmp(name, "f") == 0)
		{
			facesObject->push_back(parseFaces(rest));
			continue;
		}
		
		objects.add(name, rest);
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
		
	glLoadIdentity();
	
	glMultMatrixf(rotateCamera);
	glMultMatrixf(transCamera);
	//glMultMatrixf(modelMatrix);
	//glMultMatrixf(rotateModelMatrix);
	//glScaled(scale, scale, scale);
	//Draw_Axes();
	for (GLuint runner = 0; runner < howManyObjects; runner++)
	{
		glPushMatrix();
		
		glMultMatrixf(objects[runner].rotation);
		glMultMatrixf(objects[runner].trans);
		std::vector<ReturnedFace*>* values = objects[runner].values;
		for (std::vector<ReturnedFace*>::iterator it = values->begin(); it != values->end(); ++it)
		{
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
				glNormal3fv((GLfloat*)(*it)->normals[runner]);
				glVertex3fv((GLfloat*)(*it)->vetexs[runner]);
			}
			glEnd();


		glPopMatrix();
		}
			//Draw_Axes();

}
	glFlush();
}


void init()
{
	

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();		//load Identity matrix

	//defines view mode
	gluPerspective(60,1,2,200);
	
	glEnable(GL_DEPTH_TEST);  //define in which order the scene will built

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
	glTranslatef(0,0,-100);
	glGetFloatv(GL_MODELVIEW_MATRIX, transCamera);
	glPopMatrix();

	glLoadIdentity();

	glGetFloatv(GL_MODELVIEW_MATRIX, rotateCamera);
	glGetFloatv(GL_MODELVIEW_MATRIX, rotateGlobalMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, transGlobalMatrix);

	//initilize matrix for each object
	for (GLuint runner = 0; runner < howManyObjects; runner++)
	{
		glGetFloatv(GL_MODELVIEW_MATRIX, objects[runner].rotation);
		glGetFloatv(GL_MODELVIEW_MATRIX, objects[runner].trans);
	}

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
				glLoadMatrixf(rotateGlobalMatrix);
				glRotatef( dy*180/m_viewport[3], 1 ,0,0 );
				glRotatef( dx*180/m_viewport[2], 0 ,0,1 );
		
				glGetFloatv(GL_MODELVIEW_MATRIX, rotateGlobalMatrix);
				glPopMatrix();
	
				disp();
				break;
	


			case GLUT_MIDDLE_BUTTON:	
				
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();		
				glLoadMatrixf(transGlobalMatrix);
				glTranslatef( 0 ,0,  dy*180/m_viewport[3] );
	
				glGetFloatv(GL_MODELVIEW_MATRIX, transGlobalMatrix);
				glPopMatrix();
	
				disp();
				break;

			case GLUT_RIGHT_BUTTON:	
			
				glMatrixMode(GL_MODELVIEW);

				glPushMatrix();		
				glLoadMatrixf(rotateGlobalMatrix);
				glTranslatef( 0,  dy*180/m_viewport[3], 0 );
				glTranslatef( dx*180/m_viewport[2], 0,0 );

		
				glGetFloatv(GL_MODELVIEW_MATRIX, rotateGlobalMatrix);
				glPopMatrix();
	
				disp();
				break;
			}
break;

case CAMERA:
	switch (mouseState){
		
	case GLUT_LEFT_BUTTON:
	
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();	
				glLoadMatrixf(rotateCamera);
			
				
				glRotated(dx*180/m_viewport[2],  rotateCamera[1],rotateCamera[5],rotateCamera[9]);
				glGetFloatv(GL_MODELVIEW_MATRIX, rotateCamera);

				glRotated(dy*180/m_viewport[3],  1,0, 0);

				glGetFloatv(GL_MODELVIEW_MATRIX, rotateCamera);


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

void dispTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1,dispTimer,0);
}
int main(int  argc,  char** argv) 
{

	vector<vector<Face*>*> fFaces;

	Data ObjectsData;
	if  (argc >=  2)
		readFromFile(argv[1], ObjectsData, fFaces);
	else
		exit(1);
	
	objects = (CalObjects*) malloc(fFaces.size() * sizeof(CalObjects))   ;
	howManyObjects = fFaces.size() ;
	GLuint index = 0;
	for (vector<vector<Face*>*>::iterator it = fFaces.begin(); it != fFaces.end(); ++it)
	{
		objects[index].name = index;
		objects[index].values = ObjectsData.paint(*it);
		index ++ ;
	}	

	glutInit (& argc, argv) ;
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH) ;
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
	  
	   glutTimerFunc(2,dispTimer,0);

	glutMainLoop();

}




Face* parseFaces(char* buf)
{
	GLfloat t1, t2;
	Face* face = new Face();
	char* group = NULL;
	char* rest = NULL;
	int count  = 0;
	do {
		 if (count == 0)
			group =  strtok(buf, SEP); // first token	
		 else
			group =  strtok(rest, SEP); // first token	
	 
			count++;
			
			rest = strtok(NULL, "");
		
			char* first = strtok(group,"//");
			char* second = strtok(NULL,"");
			
			t1 = atof(first);
			t2 = atof(++second);
			face->pairs.push_back(new std::pair<GLfloat, GLfloat> (t1, t2));
	}
	while (rest);

	face->count = count;

	return face;

}