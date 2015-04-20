#include "Eye.h"


Eye::Eye(char* args)
{
	GLfloat t1, t2, t3;


	//screen coordinate
	t1 = atof(strtok(args, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fScreenCenter = new Vector3f(t1, t2, t3);

	//up vector
	t1 = atof(strtok(NULL, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fUpVector = new Vector3f(t1, t2, t3);

	//Width of the scene
	fWidth = atof(strtok(NULL, SEP));

	//image resolution
	fRx = atoi(strtok(NULL, SEP));
	fRy = atoi(strtok(NULL, SEP));

	//ambient vector
	t1 = atof(strtok(NULL, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fAmbientLighting = new Vector3f(t1, t2, t3);


}

void Eye::init()
{

}

Eye::~Eye(void)
{
	delete fAmbientLighting;
	delete fScreenCenter;
	delete fUpVector;
}
