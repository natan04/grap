#include "Eye.h"


Eye::Eye(char* args)
{
	GLfloat t1, t2, t3;

	fCameraLocation = new Vector3f(0, 0, 0);
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
	fRxD2 = fRx/2;
	fRyD2 = fRy/2;

	//ambient vector
	t1 = atof(strtok(NULL, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	fAmbientLighting = new Vector3f(t1, t2, t3);


	fPixelToWorld  = fWidth/fRx;
	
	fTowardVector = new Vector3f();
	fRightVector = new Vector3f();
	*fTowardVector = *fScreenCenter - *fCameraLocation;
	*fRightVector = (Vector3f::crossProduct(*fTowardVector, *fUpVector));

	fTowardVector->normalize();
	fRightVector->normalize();

}

Ray Eye::generateRay(GLuint aX, GLuint aY)
{
	Ray ray;
	Vector3f right = (*fRightVector);
	GLfloat valueX =  ((GLfloat)aX - fRxD2)*fPixelToWorld;
	GLfloat valueY =  ((GLfloat)aY - fRyD2)*fPixelToWorld;
	
	Vector3f point= (*fScreenCenter) +  valueX *  (*fRightVector)  + valueY * (*fUpVector) ;
	ray.direction = point - (*fCameraLocation);
	ray.startLocation = *fCameraLocation;
	ray.direction.normalize();
	return ray;
}


void Eye::init()
{

}

Eye::~Eye(void)
{
	delete fTowardVector;
	delete fRightVector;
	delete fAmbientLighting;
	delete fScreenCenter;
	delete fUpVector;
}
