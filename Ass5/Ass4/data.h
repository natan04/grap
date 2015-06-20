#pragma once
#include "Vector3f.h"
#include <stdlib.h>
#include <vector>
#include "glut.h"

#define SEP " "
typedef Vector3f Vertex;
typedef Vector3f Texture;
typedef Vector3f Normal;
using namespace std;

typedef struct returned_face
{

	vector<Normal*> normals;
	vector<Vertex*> vetexs;
	GLuint count;
	GLfloat alpha;
} ReturnedFace;

typedef struct struct_faces
{
	int poli;
	std::vector<std::pair<GLfloat, GLfloat>*> pairs;
	GLuint count;
} Face;

class Data
{
	
public:
	GLuint name;
	int add(char* name, char* rest);
	std::vector<Face*> getFaces(void);
std::vector<ReturnedFace*>* Data::paint(std::vector<Face*>* fFaces);

private:
	Vertex*		parseVertex(char* buf);
	Face*		parseFaces(char* buf);
	Texture*	parseTexture(char* buf);
	Normal*		parseNormal(char* buf);

ReturnedFace* Data::getInfoFromFace(Face* face);

	
	std::vector<Vertex*> fVertex;
	std::vector<Texture*> fTextures;
	std::vector<Normal*> fNormals;

};

