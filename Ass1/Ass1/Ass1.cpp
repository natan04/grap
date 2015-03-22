#include <stdlib.h>
#include "glut.h"
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>

GLint sobelWidth = 3;
GLint sobelHeight = 3;
GLubyte* image[4];
GLuint texture[4];

GLdouble sobel[3][3] = { {-0.125, 0.0 , 0.125}
						,{-0.25, 0.0 ,0.25} 
						,{-0.125 ,0.0, 0.125}  } ;


GLubyte test[3][3] = {{1, 2, 3},
					  {4, 5, 6} ,
					  {7, 8, 9}};
using namespace std;
int getImage(char* fileName);





GLubyte* convolution(GLubyte* image, GLint  width,GLint height)
{

	GLubyte* newImage =(GLubyte* ) malloc(width*height);
	GLint widthRunner, heightRunner, kernelColRunner, kernelRowRunner, tempCol, tempRow;
	GLdouble acc ;
	for (heightRunner = 0; heightRunner < height; heightRunner++)
		{
		
			for (widthRunner = 0; widthRunner < width; widthRunner++)
			{
				
				
				/*** kernel **/ 
				tempRow = heightRunner - sobelHeight/2;
				acc = 0;
				for (kernelRowRunner = 0; kernelRowRunner < sobelHeight; kernelRowRunner++,tempRow ++)
				{
					tempCol = widthRunner - sobelWidth/2;
					for (kernelColRunner = 0; kernelColRunner < sobelWidth; kernelColRunner++, tempCol++ )
					{
						
						if (tempCol >= width || tempCol < 0  || tempRow < 0 || tempRow >= height)
						{
						 
							continue;
						} 
							acc +=  (image[tempRow*width + tempCol])	*		(sobel[kernelRowRunner][kernelColRunner]);
						//	printf("row:%d   col: %d     %f  %d \n",tempRow,tempCol, sobel[kernelRowRunner][kernelColRunner], image[tempRow*width + tempCol]);	 
					}		
					
				
				}

				newImage[heightRunner*width + widthRunner] = acc; 
		}

		

	}

	delete(image);
	
	return newImage;
	

}
	

	


void display()
{

    
	glClear(GL_COLOR_BUFFER_BIT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

/*first image*/
	glBindTexture(GL_TEXTURE_2D, texture[0]); //using first texture
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); //adapt texture to shape
		glVertex3f(-1.0 ,0.0 ,1.0);
		
		glTexCoord2f(1, 0);  //adapt texture to shape
		glVertex3f(0.0f ,0.0f, 1.0);
		
		glTexCoord2f(1, 1);//adapt texture to shape
		glVertex3f(0.0 , 1.0f,1.0);
		
		glTexCoord2f(0, 1);//adapt texture to shape
		glVertex3f(-1 , 1.0f,1.0);
	glEnd();
	

/* second image */
	glBindTexture(GL_TEXTURE_2D, texture[1]); //using first texture
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); //adapt texture to shape
		glVertex3f(0 ,0.0 ,1.0);
		
		glTexCoord2f(1, 0);  //adapt texture to shape
		glVertex3f(1.0f ,0.0f, 1.0);
		
		glTexCoord2f(1, 1);//adapt texture to shape
		glVertex3f(1.0 , 1.0f,1.0);
		
		glTexCoord2f(0, 1);//adapt texture to shape
		glVertex3f(0 , 1.0f,1.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	
//	glViewport(0,0,256,256); 
    // flush the drawing to screen . 
    glFlush(); 
} 


void bla()
{
	  glutDisplayFunc(display); 
}


int main(int  argc,  char** argv) 
{

		int row, col;


	 glutInit (& argc, argv) ;
	  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB) ;

   glutInitWindowSize (512,512	) ;
     glutCreateWindow("First example") ;

	 getImage(argv[1]);
	 
	  glutIdleFunc(bla);
	  glutDisplayFunc(display); 
	    glutMainLoop();

   
	return 0;
		
	


}     



int getImage(char* fileName)
{

	FILE *file;
	int picSize;
	
	GLubyte header[54];

	//GLubyte *newpic;
	glEnable(GL_TEXTURE_2D);
	glOrtho(-1.0, 1.0, -1.0 ,1.0,2.0,-2.0);

	glClearColor(0,0,0,0);

	file	=	fopen(fileName,"rb");

	//read the header file.
	fread(header,54,1,file);
	if(header[0]!='B' || header[1]!='M')
		exit(1);  //not a BMP file
	
	picSize=(*(int*)(header+2)-54);
	GLint width=*(int*)(header+18);
	GLint height=*(int*)(header+22);

	void* memory = malloc(width*height);
	
	image[0]= (GLubyte*) memory;
	
	fread(image[0],1,width*height,file); //read the first image

	int index ;
	for (index = 1; index < 4; index++)
	{
			
		image[index] = (GLubyte*) malloc(width*height);
		memcpy((void*)image[index], (void*) image[0], width*height);
	}
	/************* finish read all images ******************/



/********* first image ********************/
/* first image handler */
	glGenTextures(1, &texture[0]);					 //generate place for new texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);			// initialize first texure 
	

	//texture properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//build texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, image[0]);



/************ second image ********************/
	glGenTextures(1, &texture[1]);  //generate place for new texture
	glBindTexture(GL_TEXTURE_2D, texture[1]); // initialize first texure 
	
	//texture properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLubyte* tempImage = convolution(image[1], width,height);
	image[1] = tempImage;


	//build texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, image[1]);
}

	