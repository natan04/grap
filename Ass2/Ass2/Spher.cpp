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
	fRadiusSquare = pow(fRadius, 2);

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
	fKr = NULL;
	value = strtok(NULL, "");
	if (value)
	{
		if (strcmp(value,"M") == 0)
		{
			fMirror = 1; 
			*fKa *= 0;
			*fKd *=0;
			*fKs *=0;
			fKr = new Vector3f(1,1,1);
		}
		else
		{
			value = strtok(value, " ");
			if (strcmp(value,"R") == 0)
			{	
				t1 = atof(strtok(NULL, SEP));
				t2 = atof(strtok(NULL, SEP));
				t3 = atof(strtok(NULL, SEP));
				fKr = new Vector3f(t1,t2,t3);
			}
		}

	}
}

Spher::~Spher(void)
{
	delete fCenterCoordinate;
	delete fKa;
	delete fKd;
	delete fKs;
}

//assuming getting allocated vector inside will return param.
Shape*  Spher::findIntersectionPoint(Ray ray, Vector3f& willReturn, Vector3f& normal )
{
	Vector3f startPointToCenterOfSphere = *fCenterCoordinate - ray.startLocation ;
	ray.direction.normalize();
	GLfloat v = Vector3f::dotProduct(startPointToCenterOfSphere, ray.direction);
	GLfloat lengthProjection = Vector3f::dotProduct(startPointToCenterOfSphere, ray.direction);
	GLfloat dSquare =  startPointToCenterOfSphere.getSquaredLength() - pow(lengthProjection, 2);

	
	
	if (v < 0 || dSquare > fRadiusSquare || dSquare < -0.00001  /*floating point loss of significent.*/)
		return NULL;

	GLfloat Th = sqrt(fRadiusSquare - dSquare);
	GLfloat t = lengthProjection -Th;
	if (t <= 0)
		t = lengthProjection + Th;

	willReturn = ray.startLocation + t*ray.direction;
	normal = (willReturn -  *fCenterCoordinate);
	normal.normalize();

	return this;
}

Color Spher::getAmbient(Point intersection) 
{
	return *fKa;
}