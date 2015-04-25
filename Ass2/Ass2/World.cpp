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

	WIDTH = fEye->fRx;
	HEIGHT = fEye->fRy;
	Color* screen = (Color*) malloc(sizeof(Color) * (HEIGHT * WIDTH)); 
	

	for (GLuint xAxis =0; xAxis < WIDTH; xAxis ++)
	{
		std::cout << 100*xAxis/WIDTH << "%" << std::endl;
		for (GLuint yAxis = 000; yAxis < HEIGHT; yAxis++)
		{
			
			Ray ray = fEye->generateRay(xAxis, yAxis);
			Shape* shape;
			Vector3f intersection, normal;
			if (shape = FindIntersection(ray, intersection, normal))
			{
				screen[(xAxis + yAxis*WIDTH)] = sanity(getColor(ray, intersection, normal, *shape, 0));
			}
			else
			{
				ray = ray;
			}
		}
		
	}
	return (GLubyte*) screen;
	
}

/*
GLubyte* World::paintFish()
{

	WIDTH = fEye->fRx;
	HEIGHT = fEye->fRy;
	Color* screen = (Color*) malloc(sizeof(Color) * (HEIGHT * WIDTH)); 
	

	for (GLuint xAxis =0; xAxis < WIDTH; xAxis ++)
	{
		for (GLuint yAxis = 0; yAxis < HEIGHT; yAxis++)
		{
			
			Ray ray = fEye->generateFishRay(xAxis, yAxis);
			Shape* shape;
			Vector3f intersection, normal;
			if (shape = FindIntersection(ray, intersection, normal))
			{
				screen[(xAxis + yAxis*WIDTH)] = sanity(getColor(ray, intersection, normal, *shape, 0));
			}
			else
			{
				ray = ray;
			}
		}
		
	}
	return (GLubyte*) screen;
	
}
*/
Color World::sanity(Color& color)
{
	if (color.x > 1) 
		color.x = 1;
	if (color.y > 1)
		color.y = 1;
	if (color.z > 1)
		color.z = 1;

	return color;
}


Color World::getColor( Ray ray,  Vector3f &intersection, const Vector3f &normal,  Shape &shape, GLuint level)
{
	
	ray.direction.normalize();
	Color color;
	color.x = color.y = color.z = 0;
	
	color+= (shape.getAmbient(intersection)) * (*fEye->fAmbientLighting) ;
	
	for(std::vector<Light*>::iterator it = fLights.begin(); it != fLights.end(); ++it)
	{
		if (Light* light =  (*it)->findIntersection(intersection, fShapes,ray.direction))
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
	Color reflective, trans;
	Shape *shapeRef, *shapeTran;
	Vector3f normalTrans, normalRef, newIntersectionRef, newIntersectionTran;
	Ray outRef = generateRayReflecttion(normal, ray, intersection);
	shapeRef = FindIntersection(outRef, newIntersectionRef, normalRef);
	trans.x = trans.y = trans.z = 0;
	if (!(shapeRef) ) 
		reflective.x = reflective.y = reflective.z = 0;
	else
		reflective = *shape.fKs * getColor(outRef, newIntersectionRef, normalRef, *shapeRef, level + 1);

	if (shape.fKt)
	{
		Ray outTran = shape.generateTranRay(intersection, ray.direction, normal); 
		shapeTran = FindIntersection(outTran, newIntersectionTran, normalTrans);
		if ((shapeTran) ) 
			trans = *shape.fKt * getColor(outTran, newIntersectionTran, newIntersectionTran, *shapeTran, level + 1);
	}	
	color +=reflective + trans;
	return color;
}

//ray is coming inside
Ray World::generateRayReflecttion(Vector3f normal, Ray ray, Vector3f &intersection)
{
	Ray rayOut;
	Vector3f reflective =  (ray.direction - 2*normal*(Vector3f::dotProduct(normal, ray.direction)));
	rayOut.direction = reflective;
	rayOut.startLocation = intersection;

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
		fShapes.push_back(shape);
	}
}

