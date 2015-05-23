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




int main(int  argc,  char** argv) 
{
	Data* data = new Data();
	if  (argc >=  2)
		readFromFile(argv[1], data);
	else

		exit(1);


	//glutinit (& argc, argv) ;

	//std::cout << argv[2] << std::endl;

	//

	//glutinitdisplaymode ( glut_rgb) ;
	//	
	// glutcreatewindow("example") ;

	//glutdisplayfunc(display); 
 //    glutmainloop();

}



