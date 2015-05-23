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
void Data::paint()
{
	

	
	for (std::vector<Face*>::iterator it = fFaces.begin(); it != fFaces.end(); ++it){
		
		glMaterialfv(GL_FRONT, GL_EMISSION, Vector3f(0.3, 0.4, 0.5));
		glMaterialfv(GL_FRONT, GL_AMBIENT, Vector3f(0.3, 0.4, 0.5));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector3f(0.0, 0.6, 0.7));
		glMaterialfv(GL_FRONT, GL_SPECULAR, Vector3f(0.0, 0.0, 0.8));


		glBegin(GL_TRIANGLES);
		ReturnedFace faceInfo = getInfoFromFace(*it);
		
		glNormal3f(faceInfo.normal1->x, faceInfo.normal1->y, faceInfo.normal1->z);
		glVertex3f(faceInfo.vertex1->x, faceInfo.vertex1->y, faceInfo.vertex1->z); 

		glNormal3f(faceInfo.normal2->x, faceInfo.normal2->y, faceInfo.normal2->z);
		glVertex3f(faceInfo.vertex2->x, faceInfo.vertex2->y, faceInfo.vertex2->z);

		glNormal3f(faceInfo.normal3->x, faceInfo.normal3->y, faceInfo.normal3->z);
		glVertex3f(faceInfo.vertex3->x, faceInfo.vertex3->y, faceInfo.vertex3->z);
		glEnd();
	}
	
}

ReturnedFace Data::getInfoFromFace(Face* face)
{
	ReturnedFace faceInfo;
	faceInfo.vertex1 = fVertex[face->first.first - 1];
	faceInfo.vertex2 = fVertex[face->second.first - 1];
	faceInfo.vertex3 = fVertex[face->third.first - 1];

	faceInfo.normal1 = fNormals[face->first.second - 1];
	faceInfo.normal2 = fNormals[face->second.second - 1];
	faceInfo.normal3 = fNormals[face->third.second - 1];

	return faceInfo;
}



std::vector<Face*> Data::getFaces(){
	return fFaces;
}