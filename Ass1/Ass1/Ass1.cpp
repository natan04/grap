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

int kernel_x [3][3] =
{   {-1 , 0 , 1},
    {-2 , 0 , 2},
    {-1 , 0 , 1}
};

int kernel_y [3][3] =
{   {-1 , -2 , -1},
    {0 , 0 , 0},
    {1 , 2 , 1}
};

using namespace std;
int getImage(char* fileName);

void treshHold(GLubyte *given,  GLint size, GLubyte tresh )
{
	for (int i = 0; i < size; i ++ )
	{
			given[i]  = (GLubyte)255 * (tresh <= given[i] );
		
	}

}

GLubyte* dithering(GLubyte *given,  GLint height, GLint width)
{
	GLubyte *returned = (GLubyte *) malloc(width*height); 
	float *tmp1;
	int colorNum=16;
	int sum;
	tmp1	=	new float[height * width];
	
	memset(tmp1,0,sizeof(float)* height * width);
	for(int cols = 0;cols < width;cols++)
	{
		for(int rows = 0; rows<height; rows++)
		{
			sum=0;
			tmp1[cols + rows * width] += given[cols + rows*width];

			while(tmp1[cols + rows * width]>=256/colorNum && sum<255)
			{
				sum+=256/colorNum;
				tmp1[cols + rows*width] -= 256/colorNum;
			}

			if(sum<255)
				returned[cols + rows*width]=sum;
			else returned[cols + rows*width]=240;
		
			if(cols + rows*width+1	<	width * height)
				tmp1[cols + rows*width + 1]+=tmp1[cols + rows * width]*7.0/16.0;
			if(rows < height-1)
			{
				if(cols < width-1)
					tmp1[cols + (1 + rows) * width+1]+=tmp1[cols + rows * width]*1/16.0;
	
				tmp1[cols + (1 + rows)*width]+=tmp1[cols + rows * width]*5.0/16.0;
				tmp1[cols + (1 + rows) * width-1] += tmp1[cols + rows * width]*3.0/16.0;
			}
		}
	}

	
	delete tmp1;
	delete given;
	return returned;
}

GLubyte * halftone(GLubyte *given,  GLint height, GLint width)
{
	

	GLubyte *returned = (GLubyte *) malloc(4*width*height); 
	memset(returned,255,height*width*4*sizeof(GLubyte));


	for(int cols=0;cols<width;cols++)
	{
		
		for(int rows=0;rows<height;rows++)
		{
			int newRows=rows*2;
			int newCols=cols*2;
			
			
			if(given[cols + rows*width]<=255*8/10.0)
				returned[newCols + newRows*width]=0;
						
			if(given[cols + rows*width]<=255*6/10.0)
				returned[(newCols+1) + (newRows + 1)*2*width]=0;
			
			if(given[cols + rows*width]<=255*4/10.0)
				returned[(newCols + 1) + (newRows)*2*width]=0;

			if(given[cols + rows*width]<=255*2/10.0)
				returned[(newCols) + (newRows + 1)*2*width]=0;
	}
	}


	delete(given);
	return returned;

}

GLubyte* sobel(GLubyte* image, GLint  width,GLint height)
{

	GLubyte* newImage =(GLubyte* ) malloc(width*height);
	GLint widthRunner, heightRunner, kernelColRunner, kernelRowRunner, tempCol, tempRow;
	GLdouble accX, accY; ;
	for (heightRunner = 0; heightRunner < height; heightRunner++)
		{
		
			for (widthRunner = 0; widthRunner < width; widthRunner++)
			{
				
				
				/*** kernel **/ 
				tempRow = heightRunner - sobelHeight/2;
				accX = 0, accY = 0;
				for (kernelRowRunner = 0; kernelRowRunner < sobelHeight; kernelRowRunner++,tempRow ++)
				{
					tempCol = widthRunner - sobelWidth/2;
					for (kernelColRunner = 0; kernelColRunner < sobelWidth; kernelColRunner++, tempCol++ )
					{
						
						if (tempCol >= width || tempCol < 0  || tempRow < 0 || tempRow >= height)
						{
						 
							continue;
						} 
							accX +=  (image[tempRow*width + tempCol])	*		(kernel_x[kernelRowRunner][kernelColRunner]);
						accX +=  (image[tempRow*width + tempCol])	*		(kernel_y[kernelRowRunner][kernelColRunner]);
					
					}		
					
				
				}

				newImage[heightRunner*width + widthRunner] = sqrt(accX*accX + accY*accY); 
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


	/* third image */
	glBindTexture(GL_TEXTURE_2D, texture[2]); //using first texture
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); //adapt texture to shape
		glVertex3f(-1 ,-1.0 ,1.0);
		
		glTexCoord2f(1, 0);  //adapt texture to shape
		glVertex3f(0.0f ,-1.0f, 1.0);
		
		glTexCoord2f(1, 1);//adapt texture to shape
		glVertex3f(0.0 , 0.0f,1.0);
		
		glTexCoord2f(0, 1);//adapt texture to shape
		glVertex3f(-1 , 0.0f,1.0);
	glEnd();
	
	
	/* third image */
	glBindTexture(GL_TEXTURE_2D, texture[3]); //using first texture
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); //adapt texture to shape
		glVertex3f(0 ,-1.0 ,1.0);
		
		glTexCoord2f(1, 0);  //adapt texture to shape
		glVertex3f(1.0f ,-1.0f, 1.0);
		
		glTexCoord2f(1, 1);//adapt texture to shape
		glVertex3f(1.0 , 0.0f,1.0);
		
		glTexCoord2f(0, 1);//adapt texture to shape
		glVertex3f(0 , 0.0f,1.0);
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
	GLuint offsetToPixels = *(uintptr_t*) (header + 10);
	void* memory = malloc(width*height);
	
	image[0]= (GLubyte*) memory;
	fseek(file, offsetToPixels, SEEK_SET);
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

	
	GLubyte* tempImage = sobel(image[1], width,height);
	image[1] = tempImage;

	treshHold(image[1], width * height , 150);
	//build texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, image[1]);

/************ third image ********************/
	glGenTextures(1, &texture[2]);  //generate place for new texture
	glBindTexture(GL_TEXTURE_2D, texture[2]); // initialize first texure 
	
	//texture properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	tempImage = halftone(image[2], width,height);
	image[2] = tempImage;


	//build texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width*2, height*2, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, image[2]);


	/************ furth image ********************/
	glGenTextures(1, &texture[3]);  //generate place for new texture
	glBindTexture(GL_TEXTURE_2D, texture[3]); // initialize first texure 
	
	//texture properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	tempImage = dithering(image[3], width,height);
	image[3] = tempImage;


	//build texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, tempImage);



}

	