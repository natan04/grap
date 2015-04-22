#include "World.h"
#include "Light.h"


World::World(void)
{
}


World::~World(void)
{
	for(std::vector<Shape*>::iterator it = fShapes.begin(); it != fShapes.end(); ++it) {
		delete *it;
	}
	for(std::vector<Light*>::iterator it = fLights.begin(); it != fLights.end(); ++it) {
		delete *it;
	}
	delete fEye;

}

GLubyte* World::paint()
{
	Color* screen = (Color*) malloc(sizeof(Vector3f) * (fEye->fRx) * (fEye->fRy)); 
	
	
	for (GLuint xAxis = 0; xAxis < fEye->fRx; xAxis ++)
	{
		for (GLuint yAxis = 0; yAxis < fEye->fRy; yAxis++)
		{
			Ray ray = fEye->generateRay(xAxis, yAxis);
			Shape* shape;
			Vector3f intersection, normal;
			if (shape = FindIntersection(ray, intersection, normal))
			{
				screen[(xAxis + yAxis*fEye->fRy)] = getColor(ray, intersection, normal, *shape);
			}
			else
			{
				ray = ray;
			}
		}
		
	}
	return (GLubyte*) screen;
	
}

Color World::getColor( Ray ray,  Vector3f &intersection, const Vector3f &normal,  Shape &shape)
{
	ray.direction.normalize();
	Color color;
	color.x = color.y = color.z = 0;
	
	color+= (*shape.fKa) * (*fEye->fAmbientLighting) + (*fEye->fAmbientLighting);
	
	for(std::vector<Light*>::iterator it = fLights.begin(); it != fLights.end(); ++it)
	{
		if (Light* light =  (*it)->findIntersection(intersection, fShapes))
			color += ( 
						(*shape.fKd) * (Vector3f::dotProduct(normal, (*light->fDirection)* -1)) 
						+		
						/*Ks */					(*shape.fKs) *  
						/* R vector:: */		pow((Vector3f::dotProduct
													((*light->fDirection) -2*(normal *  (Vector3f::dotProduct(*light->fDirection, normal))),
													/* normlized vector */  ray.direction)),shape.fShininess)
						)*
						*light->fIntensity;

	}

	return color;
}

Vector3f	powerVector(Vector3f& vector, unsigned int power)
{
	Vector3f vec;
	vec.x = pow(vector.x, power);
	vec.y = pow(vector.x, power);
	vec.z = pow(vector.x, power);
	return vec;
}

Shape* World::FindIntersection(Ray ray,Vector3f& aIntersectionPoint, Vector3f &aNormal)
{
	Shape * found = NULL, *value;
	GLfloat distance = M_INFINITY;
	Vector3f intersection, normal;
	for(std::vector<Shape*>::iterator it = fShapes.begin(); it != fShapes.end(); ++it) {
		
		if ( value = ((*it)->findIntersectionPoint(ray, intersection, normal)))
		{
			GLfloat newDistance =	pow(intersection[0] - ray.startLocation[0],2) + 
									pow(intersection[1] - ray.startLocation[1],2) +
									pow(intersection[2] - ray.startLocation[2],2);

			if (newDistance  < distance)
			{
				found = value;
				distance = newDistance;
				aNormal = normal;
				aIntersectionPoint = intersection;
			}

		}

	
	}

	aNormal.normalize();
	return found;
}




void World::addToWorld(char* lineArg)
{

	char* name =  strtok(lineArg, SPACE); // first token	
	char* rest = strtok(NULL, "");
	if (strcmp(name, "light") == 0)
	{
		Light* light = new Light(rest);
		fLights.push_back(light);
	}
	else if (strcmp(name, "scene") == 0)
	{
		fEye = new Eye(rest);
	}
	else if (strcmp(name, "spher") == 0)
	{
		Shape* shape = new Spher(rest);
		fShapes.push_back(shape);
	}
	else if (strcmp(name, "plane") == 0)
	{
		Shape* shape = new Plane(rest);
		//fShapes.push_back(shape);
	}
}

