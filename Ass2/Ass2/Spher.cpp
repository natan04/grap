#include "Spher.h"


Spher::Spher(char* arg)
{
	GLfloat t1, t2, t3;
	char* value;
 
	//Center location
	t1 = atof(strtok(arg, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fCenterCoordinate = new Vector3f(t1,t2,t3);

	//radius
	fRadius = atof(strtok(NULL, SEP));

	//matirials parameter
	t1 = atof(strtok(NULL, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fKa  = new Vector3f(t1,t2,t3);

	t1 = atof(strtok(NULL, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fKd = new Vector3f(t1,t2,t3);

	t1 = atof(strtok(NULL, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fKs = new Vector3f(t1,t2,t3);

	fShininess =  atof(strtok(NULL, " "));
	fMirror = 0;
	value = strtok(NULL, "");
	if (value)
	{
		if (strcmp(value,"M") == 0)
			fMirror = 1; 
	}
}


Spher::~Spher(void)
{
	delete fCenterCoordinate;
	delete fKa;
	delete fKd;
	delete fKs;
}
