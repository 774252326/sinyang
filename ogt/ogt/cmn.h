#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <GL\GLAUX.H>
#include <GL\glut.h>
#pragma comment( lib, "glut32.lib")
#pragma comment( lib, "glaux.lib")

#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <atlstr.h>

#include <opencv2\opencv.hpp>

//class Model;
void genCharList(GLuint &charlists);
void drawString(const char * str, GLuint charlists);
void drawCoordinate(float x,float y, float z, GLuint charlists);
void polarView(GLdouble distance, GLdouble twist, GLdouble elevation, GLdouble azimuth);
void pilotView(GLdouble planex, GLdouble planey,GLdouble planez, GLdouble roll,GLdouble pitch, GLdouble heading);
void normalView(GLdouble xpos, GLdouble ypos, GLdouble zpos, GLdouble xangle, GLdouble yangle);
void LoadTexture(CString fileName, GLuint m_Texture, GLint m_texWrap=GL_CLAMP, GLint m_texFilter=GL_DECAL, GLfloat m_texMode=GL_NEAREST);

void ErrorMsg(void);

typedef struct
{
	int x;
	int y;
	int button;
	int state;
} MousePara;