#include "data.h"



int Data::add(char* name, char* rest)
{




	


	if (strcmp(name, "v") == 0)
		fVertex.push_back(parseVertex(rest));

	if (strcmp(name, "vt") == 0)
		fTextures.push_back(parseTexture(rest));

	if (strcmp(name, "vn") == 0)
		fNormals.push_back(parseNormal(rest));

	if (strcmp(name, "o") == 0)
		return atoi(rest);

	return -1;
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
std::vector<ReturnedFace*>* Data::paint(std::vector<Face*>* fFaces)
{
	
	std::vector<ReturnedFace*>* values = new std::vector<ReturnedFace*>();
	
	for (std::vector<Face*>::iterator it = fFaces->begin(); it != fFaces->end(); ++it){
		
		values->push_back(getInfoFromFace(*it));

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
	faceInfo->alpha  = 1;
	return faceInfo;
}


