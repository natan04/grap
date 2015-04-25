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
			}
		}
	}

if (fNormalToPlane->x == 0 )
{
	//fRight = new Vector3f(fWidth/2 , 0,0);
	fRight = new Vector3f(1 , 0,0);
	fUp =new Vector3f( Vector3f::crossProduct(*fRight, *fNormalToPlane));
}
else
	if (fNormalToPlane->y == 0)
	{
		//fUp = new Vector3f(0, fLength/2 , 0);
		fUp = new Vector3f(0, 1 , 0);
		fRight = new Vector3f( Vector3f::crossProduct(*fNormalToPlane, *fUp));
	}
	else
		if (fNormalToPlane->z == 0 )
		{
			//fRight = new Vector3f(0, 0 ,fWidth/2 );
			fRight = new Vector3f(0, 0 ,1 );
			fUp = new Vector3f(Vector3f::crossProduct(*fRight, *fNormalToPlane));
		}
		fUp->normalize();
		fRight->normalize();
}


Plane::~Plane(void)
{
	delete fCenterToPoint ;
	delete fNormalToPlane;
	delete fKa;
	delete fKd;
	delete fKs;
}


GLboolean  Plane::lightIntersection(Ray ray, Vector3f& willReturn, Vector3f& normal, Vector3f directionOfSource  )
{
	ray.startLocation = -0.001*directionOfSource + ray.direction; //the problem with thin plane. we needed to take back the intersection point.
	GLfloat NdotV = Vector3f::dotProduct(*fNormalToPlane,  ray.direction);
		if (NdotV > 0.0001f)
		normal = *fNormalToPlane * -1;
	else
		normal = *fNormalToPlane;
	//if (NdotV < -0.00001f)
	//	return NULL; 
	NdotV = Vector3f::dotProduct(normal,  ray.direction);

	GLfloat t = Vector3f::dotProduct(normal, (*fCenterToPoint - ray.startLocation));
	t /= NdotV;

	if (t <= 0)
		return false;

return true;
}

Shape*  Plane::findIntersectionPoint(Ray ray, Vector3f& willReturn, Vector3f& normal )
{

		ray.startLocation = -0.001*ray.direction + ray.direction; //the problem with thin plane. we needed to take back the intersection point.

	GLfloat NdotV = Vector3f::dotProduct(*fNormalToPlane,  ray.direction);
	if (NdotV > 0.0001f)
		normal = *fNormalToPlane * -1;
	else
		normal = *fNormalToPlane;
	//if (NdotV < -0.00001f)
	//	return NULL; 
	NdotV = Vector3f::dotProduct(normal,  ray.direction);

	GLfloat t = Vector3f::dotProduct(normal, (*fCenterToPoint - ray.startLocation));
	t /= NdotV;

	if (t < 0)
		return NULL;

	willReturn = ray.startLocation + t*ray.direction;
	
	Vector3f centerToIntersection = willReturn - *fCenterToPoint;

	GLfloat width = Vector3f::dotProduct(centerToIntersection, *fRight);
	GLfloat height = Vector3f::dotProduct(centerToIntersection, *fUp);

	if (abs(width) > fWidth/2 || abs(height) > fLength/2)
		return NULL;


	return this;	
}
Color Plane::getAmbient(Point intersection)
{
	if (fMirror)
		return Color(0,0,0);
	Vector3f centerToIntersection = intersection - *fCenterToPoint;
	  
	GLfloat width = Vector3f::dotProduct(centerToIntersection, *fRight);
	GLfloat height = Vector3f::dotProduct(centerToIntersection, *fUp);
	width +=fWidth/2;
	height +=fLength/2;
	GLfloat mid1 = width/fWidth;
	GLfloat mid2 = height/fLength;
	GLint index1 =  32*mid1;
	GLint index2 =  32*mid2;


	if ((index1 + index2) % 2)
		return Color(0,0,0);
	else 
		return Color(1,1,1);

}

Ray Plane::generateTranRay(Point intersection, Vector3f direction, Vector3f normal)
{
	Ray ray;
	ray.direction = direction;
	ray.startLocation = intersection;
	return ray;
}