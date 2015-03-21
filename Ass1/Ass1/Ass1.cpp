#include "glut.h"

void mydisplay1(void){
	//glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,300,300);
	glBegin(GL_QUADS);
		glColor3f(1.0f, 0.0f , 0.0f); 
		glVertex2f(1.0 , 0.0f);
		glColor3f(0.0f, 1.0f , 0.0f);
		glVertex2f(0.0 , 1.0f);
		glColor3f(0.0f, 0.0f , 1.0f);
		glVertex2f(-1.0 , 0.0f);
		glColor3f(0.0f, 1.0f , 1.0f);
		glVertex2f(0.0 , -1.0f);

	glEnd();
	glViewport(200,200,200,200);
	glColor3f(0.0f, 1.0f, 0.0f);
	glLineWidth(8.4);
	glBegin(GL_LINES);
		glVertex2f(0.0 , 0.5f);
		glVertex2f(1.0 , 0.5f);
	glEnd();
	glFlush();
}

void mydisplay2(void){
	//glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,300,300);
	glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 1.0f , 0.0f); 
		glVertex2f(1.0 , 0.0f);
		glVertex2f(0.0 , 1.0f);
		glVertex2f(-1.0 , 0.0f);

	glEnd();
	glViewport(200,0,200,200);
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(8.4);
	glBegin(GL_LINES);
		glVertex2f(0.0 , -0.5f);
		glVertex2f(-1.0 , 0.5f);
	glEnd();
	glFlush();
}

int main(int  argc,  char** argv) 
{
   glutInit (& argc, argv) ;
   glutInitDisplayMode (GLUT_SINGLE ) ;
   glutInitWindowSize (500,500) ;
   glutCreateWindow("First example") ;
   glutDisplayFunc(mydisplay1) ;

   
   glutInitDisplayMode (GLUT_SINGLE) ;
   glutInitWindowSize (500,500) ;
   glutCreateWindow("Second example") ;
   glutDisplayFunc(mydisplay2) ;
   glutPositionWindow(600, 30);
	
	 glutMainLoop () ; 

}     
