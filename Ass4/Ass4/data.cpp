#include "data.h"

void Data::add(char* buf)
{


	if (buf[0] == '#' || std::strlen(buf) == 0)
			return;

	char* name =  strtok(buf, SEP); // first token	
	char* rest = strtok(NULL, "");

	if (strcmp(name, "f") == 0)
		fFaces.push_back(parseFaces(rest));

	if (strcmp(name, "v") == 0)
		fVertex.push_back(parseVertex(rest));

	if (strcmp(name, "vt") == 0)
		fTextures.push_back(parseTexture(rest));

	if (strcmp(name, "vn") == 0)
		fNormals.push_back(parseNormal(rest));


}

Vertex* Data::parseVertex(char* buf)
{
	GLfloat t1, t2, t3;
 
	//direction
	t1 = atof(strtok(buf, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	Vertex * v = new Vertex(t1, t2, t3);

	return v;

}

Face* Data::parseFaces(char* buf)
{
	Face* face = new Face();

	char* group =  strtok(buf, SEP); // first token	
	char* rest = strtok(NULL, "");
	char* first = strtok(group,"//");
	char* second = strtok(NULL,"");
	
	face->first.first = atof(first);
	face->first.second = atof(++second);

	 group =  strtok(rest, SEP); // first token	
	 rest = strtok(NULL, "");
	 first = strtok(group,"//");
	 second = strtok(NULL,"");

	 face->second.first = atof(first);
	 face->second.second = atof(++second);

	 group =  strtok(rest, SEP); // first token	
	 rest = strtok(NULL, "");
	 first = strtok(group,"//");
	 second = strtok(NULL,"");

	 face->third.first = atof(first);
	 face->third.second = atof(++second);


	return face;


}

Texture* Data::parseTexture(char* buf)
{
	GLfloat t1, t2, t3; 
	//direction
	t1 = atof(strtok(buf, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	Texture * v = new Texture(t1, t2, t3);

	return v;
}

Normal* Data::parseNormal(char* buf)
{
	GLfloat t1, t2, t3;
 
	//direction
	t1 = atof(strtok(buf, SEP));
	t2 = atof(strtok(NULL, SEP));
	t3 = atof(strtok(NULL, SEP));
	Normal * v = new Normal(t1, t2, t3);

	return v;
}