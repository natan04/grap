#include "Plane.h"

Plane::Plane(char* arg)
{
	GLfloat t1, t2, t3;
	char* value;
 
	//normal location
	t1 = atof(strtok(arg, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fNormalToPlane = new Vector3f(t1,t2,t3);

	//center parameter
	t1 = atof(strtok(NULL, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fCenterToPoint  = new Vector3f(t1,t2,t3);

	fWidth = atof(strtok(NULL, SEP));
	fLength = atof(strtok(NULL, SEP));
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


Plane::~Plane(void)
{
	delete fCenterToPoint ;
	delete fNormalToPlane;
	delete fKa;
	delete fKd;
	delete fKs;
}

Shape*  Plane::findIntersectionPoint(Ray ray, Vector3f& willReturn, Vector3f& normal )
{
	GLfloat t = Vector3f::dotProduct(*fNormalToPlane, 
					/*(Q0 - P0)/N dot V */	(*fCenterToPoint - ray.startLocation)/(Vector3f::dotProduct(*fNormalToPlane, ray.direction))); 

	if (t < 0)
		return NULL;

	willReturn = ray.startLocation + t*ray.startLocation;
	normal = *fNormalToPlane;

	return this;	
}
