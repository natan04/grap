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
	GLfloat t1, t2;
	Face* face = new Face();
	char* group = NULL;
	char* rest = NULL;
	int count  = 0;
	do {
		 if (count == 0)
			group =  strtok(buf, SEP); // first token	
		 else
			group =  strtok(rest, SEP); // first token	
	 
			count++;
			
			rest = strtok(NULL, "");
		
			char* first = strtok(group,"//");
			char* second = strtok(NULL,"");
			
			t1 = atof(first);
			t2 = atof(++second);
			face->pairs.push_back(new std::pair<GLfloat, GLfloat> (t1, t2));
	}
	while (rest);

	face->count = count;

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
std::vector<ReturnedFace*> Data::paint()
{
	
	
	std::vector<ReturnedFace*> values;
	for (std::vector<Face*>::iterator it = fFaces.begin(); it != fFaces.end(); ++it){
		
		values.push_back(getInfoFromFace(*it));

	}
	
	return values;
}

ReturnedFace* Data::getInfoFromFace(Face* face)
{
	ReturnedFace* faceInfo = new ReturnedFace();

	for (std::vector<pair<GLfloat, GLfloat>*>::iterator it = face->pairs.begin(); it != face->pairs.end(); ++it)
	{
		faceInfo->vetexs.push_back(fVertex[(*it)->first -1]);
		faceInfo->normals.push_back(fNormals[(*it)->second -1]);
	
	}
	faceInfo->count = face->count;

	return faceInfo;
}



std::vector<Face*> Data::getFaces(){
	return fFaces;
}