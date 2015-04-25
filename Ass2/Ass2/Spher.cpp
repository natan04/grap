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
	fKt = NULL;
	value = strtok(NULL, "");
	if (value)
	{
		if (strcmp(value,"M") == 0)
		{
			fMirror = 1; 
			*fKa *= 0;
			*fKd *=0;
			fKs->x = fKs->y = fKs ->z = 1;;
		}
		else
		{
			value = strtok(value, " ");
			if (strcmp(value,"T") == 0)
			{	
				t1 = atof(strtok(NULL, SEP));
				t2 = atof(strtok(NULL, SEP));
				t3 = atof(strtok(NULL, SEP));
				fKt = new Vector3f(t1,t2,t3);
				t1 = atof(strtok(NULL, SEP));
				fRefractiveIndex = t1;
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
Shape*  Spher::findIntersectionTrans(Ray ray, Vector3f& willReturn, Vector3f& normal )
{

	ray.startLocation = ray.direction*-2 + ray.startLocation;
	Vector3f startPointToCenterOfSphere = *fCenterCoordinate - ray.startLocation ;
	ray.direction.normalize();
	GLfloat v = Vector3f::dotProduct(startPointToCenterOfSphere, ray.direction);
	GLfloat lengthProjection = Vector3f::dotProduct(startPointToCenterOfSphere, ray.direction);
	GLfloat dSquare =  startPointToCenterOfSphere.getSquaredLength() - pow(lengthProjection, 2);

	
	
	if (v < 0 || dSquare > fRadiusSquare || dSquare < -0.00001  /*floating point loss of significent.*/)
		return NULL;

	GLfloat Th = sqrt(fRadiusSquare - dSquare);
	GLfloat t = lengthProjection * 2*Th;
	if (t <= 0)
		t = lengthProjection - 2*Th;

	willReturn = ray.startLocation + t*ray.direction;
	normal = (willReturn -  *fCenterCoordinate);
	normal.normalize();

	return this;
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

Ray Spher::generateTranRay(Point intersection, Vector3f direction, Vector3f normal)
{
	direction.normalize();
	normal.normalize();
	GLfloat angleIn = acos(Vector3f::dotProduct(normal, direction * -1));
	GLfloat angleOut =  asin(sin(angleIn)/fRefractiveIndex);
	Vector3f T = ((1/(fRefractiveIndex))*cos(angleIn) - cos(angleOut)) * normal;
	T = T - (1/fRefractiveIndex)*(direction*-1);
	Ray inside;
	inside.direction = T;
	inside.startLocation = intersection;


	findIntersectionTrans(inside, intersection, normal); 
		inside.direction.normalize();
	normal.normalize();
	 angleIn = acos(Vector3f::dotProduct(normal*-1, inside.direction * -1));
	 GLfloat value = fRefractiveIndex*sin(angleIn);
	 angleOut =  asin(fRefractiveIndex*sin(angleIn));
	 T = (((fRefractiveIndex))*cos(angleIn) - cos(angleOut)) * normal;
	T = T - (fRefractiveIndex)*( inside.direction *-1);
	
	inside.direction = T;
	inside.startLocation = intersection;


	return inside;
}
Color Spher::getAmbient(Point intersection) 
{
	return *fKa;
}

GLboolean Spher::lightIntersection(Ray ray, Vector3f& willReturn, Vector3f& normal,Vector3f directionOfSource  )
{
	Vector3f startPointToCenterOfSphere = *fCenterCoordinate - ray.startLocation ;
	ray.direction.normalize();
	GLfloat v = Vector3f::dotProduct(startPointToCenterOfSphere, ray.direction);
	GLfloat lengthProjection = Vector3f::dotProduct(startPointToCenterOfSphere, ray.direction);
	GLfloat dSquare =  startPointToCenterOfSphere.getSquaredLength() - pow(lengthProjection, 2);

	
	
	if (v < 0 || dSquare > fRadiusSquare || dSquare < -0.00001  /*floating point loss of significent.*/)
		return false;



	return true;
}

