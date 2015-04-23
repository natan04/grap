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

	//spotlight handle;
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

Light* Light::findIntersection(Vector3f& intersectionPoint,std::vector<Shape*> & shapes)
{
	Vector3f normal;
	Ray ray;
	ray.startLocation = intersectionPoint;
	ray.direction = *fDirection * -1;
	if ( intersact(ray, shapes))
		return NULL;
	else
	{
		if (fSpotlight)
		{
			GLfloat angle = Vector3f::dotProduct(ray.direction, normal)/(ray.direction.getLength()*normal.getLength());
			if (angle > fCutoff)
				return NULL;
			else return this;
		}
		else return this;
	}
}

GLboolean Light::intersact(Ray& ray, std::vector<Shape*> & shapes)
{
	Vector3f intersection, normal;
	for(std::vector<Shape*>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
		if (  ((*it)->findIntersectionPoint(ray, intersection, normal)))
			return true;
	}
	return false;
}


		
