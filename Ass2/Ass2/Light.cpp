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
		fCutoff = DEG2RAD(atof(strtok(NULL, SEP)));

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

Light* Light::findIntersection(Vector3f& intersectionPoint,std::vector<Shape*> & shapes ,Vector3f directionOfSource)
{
			Vector3f intersection;

	if (fSpotlight)
	{
		Ray ray;
		ray.startLocation = intersectionPoint;
		ray.direction = *fSpotlight - intersectionPoint;
		GLboolean intersactPoint = intersact(ray, shapes, directionOfSource, intersection);
	
		if  (intersactPoint)
		{
			GLdouble lengthToSpotLight = Vector3f::squaredDistance(ray.startLocation, *fSpotlight);
			GLdouble lengthToIntersection = Vector3f::squaredDistance(ray.startLocation, intersection);
			if (lengthToSpotLight > lengthToIntersection)
				return NULL;
			GLfloat angle = acos(Vector3f::dotProduct(ray.direction, *fDirection*-1)/(ray.direction.getLength()*fDirection->getLength()));
			if (angle > fCutoff)
				return NULL;
			else
				return this;
		}
		else
		{

			GLfloat angle = acos(Vector3f::dotProduct(ray.direction, *fDirection*-1)/(ray.direction.getLength()*fDirection->getLength()));
			if (angle > fCutoff)
				return NULL;
			else
				return this;
		}
	}
	else
	{
		Ray ray;
		//reversing the ray from lighter. and sending it from the intersection point
		ray.startLocation = intersectionPoint;		
		ray.direction = *fDirection * -1;
		if ( intersact(ray, shapes, directionOfSource, intersection))
			return NULL;
		else
		{
			return this;
		}

	}
}

GLboolean Light::intersact(Ray& ray, std::vector<Shape*> & shapes, Vector3f directionOfSource, Vector3f& intersection)
{
	Vector3f  normal;
	for(std::vector<Shape*>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
		if (  ((*it)->findIntersectionPoint(ray, intersection, normal)))
			return true;
	}
	return false;
}


		
