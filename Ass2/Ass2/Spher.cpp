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

//assuming getting allocated vector inside will return param.
GLboolean  Spher::findIntersectionPoint(Vector3f& startPoint, Vector3f& direction, Vector3f& willReturn, Vector3f& normal )
{
	Vector3f startPointToCenterOfSphere = *fCenterCoordinate - startPoint;
	GLfloat lengthProjection = Vector3f::dotProduct(startPointToCenterOfSphere, direction);
	GLfloat dSquare =  pow(lengthProjection, 2) - pow(lengthProjection, 2);

	if (dSquare > fRadiusSquare)
		return 0;

	GLfloat Th = sqrt(fRadiusSquare - dSquare);
	GLfloat t = lengthProjection -Th;
	if (t <= 0)
		t = lengthProjection + Th;

	willReturn = startPoint + t*direction;
	normal = (willReturn -  *fCenterCoordinate);
	normal.normalize();

	return true;
}