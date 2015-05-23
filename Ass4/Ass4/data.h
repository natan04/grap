#pragma once
#include "Vector3f.h"
#include <stdlib.h>
#include <vector>
#include "glut.h"

#define SEP " "
typedef Vector3f Vertex;
typedef Vector3f Texture;
typedef Vector3f Normal;

typedef struct struct_faces
{
	std::pair<GLfloat, GLfloat> first;
	std::pair<GLfloat, GLfloat> second;
	std::pair<GLfloat, GLfloat> third;

} Face;

class Data
{
	

public:
	void add(char* line);


private:
	Vertex*		parseVertex(char* buf);
	Face*		parseFaces(char* buf);
	Texture*	parseTexture(char* buf);
	Normal*		parseNormal(char* buf);

	std::vector<Vertex*> fVertex;
	std::vector<Texture*> fTextures;
	std::vector<Normal*> fNormals;
	std::vector<Face*> fFaces;

};

