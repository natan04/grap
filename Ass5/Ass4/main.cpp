#include <stdlib.h>
#include "glut.h"
#include "GL.h"
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include "data.h"
float zValue[4];

#define stringify( name ) # name
#define MAX_CHARS_PER_LINE 500
#define GLUPERS 	gluPerspective(60,1,2,200);

#define bufSize 512
Face* parseFaces(char* buf);
enum MODE {CAMERA, GLOBAL, PICKING	};
MODE sceneMode;
enum MODE_PICKING {TRANSLATION, ROTATION, SCALE	};
MODE_PICKING pickingMode;

GLfloat transCamera[16];
GLfloat rotateCamera[16];
GLfloat rotateGlobalMatrix[16];
GLfloat transGlobalMatrix[16];
bool finishPicking;
bool * pickingArray;
GLdouble scale = 1;
bool press;
GLint hits;
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
	Vector3f COM;
	GLuint name;
	std::vector<ReturnedFace*>* values;
	GLfloat rotation[16];
	GLfloat trans[16];
} CalObjects;
CalObjects* objects;

GLuint howManyObjects ;

void paintSphere()
{
	GLUquadricObj*  Sphere=gluNewQuadric();
   gluSphere(Sphere,0.3,1,1);
	
}

void drawAll(GLenum mode) //draws square
{

	for (GLuint runner = 0; runner < howManyObjects; runner++)
	{
		

		if(mode==GL_SELECT)
			glLoadName( runner);
		glPushMatrix();
		glLoadIdentity();


		glMultMatrixf(transGlobalMatrix);
		glMultMatrixf(objects[runner].trans);
		glMultMatrixf(rotateCamera);	
		glMultMatrixf(transCamera);
		
		glMultMatrixf(rotateGlobalMatrix);
	
			glTranslatef(objects[runner].COM.x, objects[runner].COM.y, objects[runner].COM.z);
		glMultMatrixf(objects[runner].rotation);
		glTranslatef(-objects[runner].COM.x, -objects[runner].COM.y, -objects[runner].COM.z);

		
		std::vector<ReturnedFace*>* values = objects[runner].values;
		for (std::vector<ReturnedFace*>::iterator it = values->begin(); it != values->end(); ++it)
		{
	
			if (pickingArray[runner])
			{	
				GLfloat mat_s[] = {1,0,0};
				glMaterialfv(GL_FRONT, GL_EMISSION, mat_s);

			}
			else{
				glMaterialfv(GL_FRONT, GL_AMBIENT, Vector3f(0.3, 0.4, 0.5));
				glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector3f(0.0, 0.6, 0.7));
				glMaterialfv(GL_FRONT, GL_SPECULAR, Vector3f(0.0, 0.0, 0.8));
			}
				if ((*it)->count > 3)
					glBegin(GL_POLYGON);
				else		
					glBegin(GL_TRIANGLES);	

				for (GLuint runnerInside = 0; runnerInside < (*it)->count; runnerInside++)
				{
					glNormal3fv((GLfloat*)(*it)->normals[runnerInside]);
					glVertex3fv((GLfloat*)(*it)->vetexs[runnerInside]);
				}
				glEnd();

				
			
		}
			
			GLfloat mat_s[] = {0.0, 0.0, 0.4, 1.0};
			glMaterialfv(GL_FRONT, GL_EMISSION, mat_s);


		glPopMatrix();
		
	}
}

void startPicking(GLuint *selectionBuf)
{
	glSelectBuffer(bufSize,selectionBuf); //declare buffer for input in selection mode
	glRenderMode(GL_SELECT); //change to selecting mode
	glInitNames();			//initialize names stack
	glPushName(-1);			//push name
}

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
	case 't':
		printf("Changing to picking mode translation\n");
		sceneMode = PICKING;
		pickingMode = TRANSLATION;
		break;

	case 'r':
				
		printf("Changing to picking mode rotation\n");
		sceneMode = PICKING;
		pickingMode = ROTATION;
		break;

	case 's':

		printf("Changing to picking mode translatio\n");
		sceneMode = PICKING;
		pickingMode = SCALE;
		break;

	case 'g' :  
			sceneMode = GLOBAL;
		break;
	case 'c':
			sceneMode = CAMERA;
		break;
	
		cout << "The current mode:" << sceneMode << endl;;
	}

	glutPostRedisplay();
}

void processHits(GLint hits, GLuint *buffer)
{
	printf("\nbuffer\n");
	float z1,z2;

	for (GLint runner = 0; runner < hits*4; runner += 4)
	{

		z1=buffer[runner+1]/4294967295.0;
		z2=buffer[runner+2]/4294967295.0;
		printf("object number without filter: %d \n",buffer[runner+3]);

		if((zValue[0]>=z1-0.0001 && zValue[0]<=z2+0.0001))
		{	//try to locate which name is correlated with the pressed pixel according to z value 

			printf("filtered: %d \n",buffer[runner+3]);
			pickingArray[buffer[runner+3]] = !pickingArray[buffer[runner+3]] ;
		}

	}
}

void mouse(int button, int state, int x, int y) 
{

	if (button == GLUT_LEFT_BUTTON)
	{
		if (finishPicking)
			memset((void*)pickingArray, 0, howManyObjects* sizeof(bool));
		finishPicking = true;
	}
	if (button == GLUT_RIGHT_BUTTON)
		finishPicking = false;

	xx=x;
	yy=y;
	mouseState = button;
	GLuint selectionBuf[bufSize];
	GLint viewport[4];

	memset((void*) selectionBuf, 0, sizeof(GLuint)*bufSize);

	glGetIntegerv(GL_VIEWPORT,viewport); //reading viewport parameters
	press=!press;

	if(press)
	{   //use selection mode to pick

		glMatrixMode(GL_PROJECTION);

		glReadPixels((GLdouble) x,(GLdouble) viewport[3]-y,2,2,GL_DEPTH_COMPONENT,GL_FLOAT,zValue);

		glPushMatrix();	//saves current projection matrix

		glLoadIdentity(); 
		startPicking(selectionBuf); //preper selection mode

		gluPickMatrix(x, (GLdouble) viewport[3] - y, 1, 1, viewport);
	
		GLUPERS;

		glMatrixMode(GL_MODELVIEW); 

		drawAll(GL_SELECT);

		hits=glRenderMode(GL_RENDER); //gets hits number 

		glMatrixMode(GL_PROJECTION);
		glPopMatrix(); //restores projection matrix


		glMatrixMode(GL_MODELVIEW);
		processHits(hits,selectionBuf); //check hits
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glDisable(GL_NORMALIZE);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();


	//glScaled(scale, scale, scale);
	//Draw_Axes();
	drawAll(GL_RENDER);
	glFlush();
}


void init()
{


	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();		//load Identity matrix

	//defines view mode

	GLUPERS;
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


void	pickMatrixsRotate(GLfloat moveX, GLfloat moveY)
{
	
	glMatrixMode(GL_MODELVIEW);
	for (GLuint runner = 0; runner < howManyObjects; runner++)
	{
		if (pickingArray[runner])
		{

			glPushMatrix();
			glLoadMatrixf(objects[runner].rotation);
			

			glRotated(moveX,  rotateCamera[1],rotateCamera[5],rotateCamera[9]);
			glGetFloatv(GL_MODELVIEW_MATRIX, objects[runner].rotation);

			glRotated(moveY,  1,0, 0);


			glGetFloatv(GL_MODELVIEW_MATRIX, objects[runner].rotation);


			glPopMatrix();
		}
	}

}


void	pickMatrixsTrans(GLfloat moveX, GLfloat moveY)
{
	
	glMatrixMode(GL_MODELVIEW);
	for (GLuint runner = 0; runner < howManyObjects; runner++)
	{
		if (pickingArray[runner])
		{

			glPushMatrix();
			glLoadMatrixf(objects[runner].trans);
			glTranslatef(moveX, moveY, 0 );
			glGetFloatv(GL_MODELVIEW_MATRIX, objects[runner].trans);
			
			glPopMatrix();
		}
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
			glRotatef( dy*180/m_viewport[3], rotateGlobalMatrix[0] ,rotateGlobalMatrix[4],rotateGlobalMatrix[8] );

			glGetFloatv(GL_MODELVIEW_MATRIX, rotateGlobalMatrix);
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

	case PICKING:

		switch (pickingMode)
		{
			case TRANSLATION:
				pickMatrixsTrans(dx*180/m_viewport[2],  dy*180/m_viewport[3]); //TODO: check if need to sub (view port - y)
			break;

			case SCALE:
				pickMatrixsTrans(dx*180/m_viewport[2],  dy*180/m_viewport[3]); //TODO: check if need to sub (view port - y)
			break;

			case ROTATION:
				pickMatrixsRotate(dx*180/m_viewport[2],  dy*180/m_viewport[3]); //TODO: check if need to sub (view port - y)
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

Vector3f getCOM(std::vector<ReturnedFace*>* v)
{
		Vector3f com; com.x = 0; com.y = 0; com.z = 0;
		GLuint count = 0;
		for (std::vector<ReturnedFace*>::iterator it = v->begin(); it != v->end(); ++it)
		{
	
			for (GLuint runnerInside = 0; runnerInside < (*it)->count; runnerInside++)
				{
					count ++;
					com += *(*it)->vetexs[runnerInside];
				}
			
		}
			
		com /= count;
		return com;
	
}
int main(int  argc,  char** argv) 
{
	finishPicking = false;
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
		objects[index].COM = getCOM(objects[index].values);
		index ++ ;
	}	

	pickingArray = (bool*) malloc(sizeof(bool) * howManyObjects);

	memset((void*) pickingArray, 0, sizeof(bool) * howManyObjects);

	glutInit (& argc, argv) ;
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH) ;
	glutInitWindowSize ( 1024,768) ;
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