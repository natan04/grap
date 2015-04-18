#include "Light.h"




Light::Light(char* args)
{
	GLfloat t1, t2, t3;
	char* value;

	//direction
	t1 = atof(strtok(args, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fDirection = new Vector3f(t1, t2, t3);
	
	//intensity
	t1 = atof(strtok(NULL, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fIntensity = new Vector3f(t1, t2, t3);

	value = strtok(NULL, SEP);

	if (value)
	{
		t1 = atof(value);
		t2 = atof(strtok(NULL, SEP));
		t3 = atof(strtok(NULL, SEP));
		fSpotlight = new Vector3f(t1, t2, t3);
		fCutoff = atof(strtok(NULL, SEP));

	}
	else 
	{
		fSpotlight = NULL;
		fCutoff = -1;
	}


}


Light::~Light(void)
{
	delete fSpotlight;
	delete fDirection;
	delete fIntensity;

}
