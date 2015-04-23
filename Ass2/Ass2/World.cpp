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
				screen[(xAxis + yAxis*fEye->fRy)] = getColor(ray, intersection, normal, *shape, 0);
			}
			else
			{
				ray = ray;
			}
		}
		
	}
	return (GLubyte*) screen;
	
}

Color World::getColor( Ray ray,  Vector3f &intersection, const Vector3f &normal,  Shape &shape, GLuint level)
{
	
	ray.direction.normalize();
	Color color;
	color.x = color.y = color.z = 0;
	
	color+= (*shape.fKa) * (*fEye->fAmbientLighting) + (*fEye->fAmbientLighting);
	
	for(std::vector<Light*>::iterator it = fLights.begin(); it != fLights.end(); ++it)
	{
		if (Light* light =  (*it)->findIntersection(intersection, fShapes))
		{
			Color diffuse, specular;
			diffuse *=0; specular*=0;
			Vector3f NormalToLight = (*light->fDirection)* -1; //we need to oposite direction
			NormalToLight.normalize();
				
			diffuse  = (*shape.fKd) * (Vector3f::dotProduct(normal, NormalToLight));
			specular = handleSpecular(normal, *light->fDirection, ray.direction, shape.fShininess)*(*shape.fKs);
			color += (diffuse + specular) * *light->fIntensity;
		}
	}
	if (MAX_LEVEL == level)
		return color;
	Color reflective;
	Shape *newShape;
	Vector3f normalNew, newIntersection;
	Ray out = generateRayReflecttion(normal, ray);
	newShape = FindIntersection(out, newIntersection, normalNew);
	if (!(newShape)) 
		return color;
	reflective = *shape.fKs * getColor(out, newIntersection, normalNew, *newShape, level + 1);
	color +=reflective;
	return color;
}

//ray is coming inside
Ray World::generateRayReflecttion(Vector3f normal, Ray ray)
{
	Ray rayOut;
	Vector3f reflective =  (ray.direction - 2*normal*(Vector3f::dotProduct(normal, ray.direction)));
	rayOut.direction = reflective;
	rayOut.startLocation = ray.startLocation;

	return rayOut;
}
//the direction of l is to us.
GLfloat World::handleSpecular(Vector3f normal, Vector3f l, Vector3f v, GLfloat shininess)
{
		Vector3f reflective =  (l - 2*normal*(Vector3f::dotProduct(normal, l)));
		reflective.normalize(); 
		GLfloat vDotR  = Vector3f::dotProduct(reflective, v);
		return pow(vDotR, shininess);
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

