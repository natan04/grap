#include "World.h"
#include "Light.h"


World::World(void)
{
}


World::~World(void)
{
}

void World::addToWorld(char* lineArg)
{

	char* name =  strtok(lineArg, SPACE); // first token	
	char* rest = strtok(NULL, "");
	if (strcmp(name, "light") == 0)
	{
		Light* l = new Light(rest);
	}
	else if (strcmp(name, "scene") == 0)
		std::cout << "scene" << std::endl;
	else if (strcmp(name, "spher") == 0)
		std::cout << "spher" << std::endl;
	else if (strcmp(name, "plane") == 0)
		std::cout << "plane" << std::endl;

}