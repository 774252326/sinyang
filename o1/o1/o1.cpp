// o1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "glut32.lib")



#include<windows.h>    
#include <GL/glut.h>   //glut.h�к���gl.h��glu.h�ļ�    

void myDisplay(void)    
{    
	glClear(GL_COLOR_BUFFER_BIT);    
	glPointSize(5.0f);    
	glBegin(GL_POINTS);    
	glVertex2f(0.0f, 0.0f);    
	glVertex2f(0.5f, 0.5f);    
	glEnd();    
	glFlush();    
}    





//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char *argv[])  
{

	glutInit(&argc, argv);    
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);    
	glutInitWindowPosition(100, 100);    
	glutInitWindowSize(400, 400);    
	glutCreateWindow("���㣨���أ�");    
	glutDisplayFunc(&myDisplay);    
	glutMainLoop();  
	return 0;    


}

