#include "World.h"
World::World(void){
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