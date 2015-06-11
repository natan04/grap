#include "Callbacks.h"



void spaciel(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP : 
		glScalef( 1 + 0.05 ,1+ 0.05 ,1.0f + 0.05 );
		glutPostRedisplay();
		break;

	case  GLUT_KEY_DOWN : 
		glScalef( 1 -0.05,1 - 0.05,1.0f - 0.05 );
		glutPostRedisplay();
		break;

	case GLUT_KEY_F2:
		glPushMatrix();
		glMatrixMode(GL_PROJECTION); /* switch matrix mode */
		glLoadIdentity();		//load Identity matrix

		gluPerspective(angle += 0.5 ,1,2,200);
		glTranslatef(0,0,-100);

		glutPostRedisplay();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		break;


	case GLUT_KEY_F3:
		glPushMatrix();
		glMatrixMode(GL_PROJECTION); /* switch matrix mode */
		glLoadIdentity();		//load Identity matrix

		gluPerspective(angle -= 0.5 ,1,2,200);
		glTranslatef(0,0,-100);

		glutPostRedisplay();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		break;



	}
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
	/*glBegin(GL_POLYGON);
	Draw_Axes ();

	Draw_Teapot();
	glEnd();
	*/
	//Draw_Teapot();

	for (std::vector<ReturnedFace*>::iterator it = values->begin(); it != values->end(); ++it){

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

	glFlush();

}





//void spaciel(int key, int x, int y)
//{
//	switch(key)
//	{
//	case GLUT_KEY_UP : 
//		glScalef( 1 + 0.05 ,1+ 0.05 ,1.0f + 0.05 );
//		glutPostRedisplay();
//		break;
//
//	case  GLUT_KEY_DOWN : 
//		glScalef( 1 -0.05,1 - 0.05,1.0f - 0.05 );
//		glutPostRedisplay();
//		break;
//
//	case GLUT_KEY_F2:
//		glPushMatrix();
//		glMatrixMode(GL_PROJECTION); /* switch matrix mode */
//		glLoadIdentity();		//load Identity matrix
//
//		gluPerspective(angle += 0.5 ,1,2,200);
//		glTranslatef(0,0,-100);
//
//		glutPostRedisplay();
//
//		glMatrixMode(GL_MODELVIEW);
//		glPopMatrix();
//		break;
//
//
//	case GLUT_KEY_F3:
//		glPushMatrix();
//		glMatrixMode(GL_PROJECTION); /* switch matrix mode */
//		glLoadIdentity();		//load Identity matrix
//
//		gluPerspective(angle -= 0.5 ,1,2,200);
//		glTranslatef(0,0,-100);
//
//		glutPostRedisplay();
//		glMatrixMode(GL_MODELVIEW);
//		glPopMatrix();
//		break;
//
//
//
//	}
//}
