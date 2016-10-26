#include "cmn.h"

void genCharList(GLuint &charlists)
{
	// 为每一个ASCII字符产生一个显示列表
	//isFirstCall = 0;
	const int MAX_CHAR=128;
	// 申请MAX_CHAR个连续的显示列表编号
	charlists = glGenLists(MAX_CHAR);
	// 把每个字符的绘制命令都装到对应的显示列表中
	wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, charlists);
}

void drawString(const char * str, GLuint charlists)
{
	for(; *str!='\0'; ++str){
		glCallList(charlists + *str);
	}
}

void drawCoordinate(float x,float y, float z, GLuint charlists)
{
	char buf[50];
	//glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos3d(x,y,z);
	sprintf_s(buf,"%g,%g,%g",x,y,z);
	drawString(buf, charlists);
}


void polarView(GLdouble distance, GLdouble twist, GLdouble elevation, GLdouble azimuth)
{
	glTranslated(0,0,-distance);
	glRotated(-twist,0,0,1);
	glRotated(-elevation,1,0,0);
	glRotated(azimuth,0,0,1);
	printf("[%f,%f,%f,%f]\n",distance, twist, elevation, azimuth);
}

void pilotView(GLdouble planex, GLdouble planey,GLdouble planez, GLdouble roll,GLdouble pitch, GLdouble heading)
{
	glRotated(roll, 0.0, 0.0, 1.0);
	glRotated(pitch, 0.0, 1.0, 0.0);
	glRotated(heading, 1.0, 0.0, 0.0);
	glTranslated(-planex, -planey, -planez);

	printf("[%f,%f,%f,%f,%f,%f]\n",planex, planey, planez, roll, pitch, heading);

}

void normalView(GLdouble xpos, GLdouble ypos, GLdouble zpos, GLdouble xangle, GLdouble yangle)
{
	glTranslatef(xpos, ypos, zpos);
	glRotatef(xangle, 1.0f,0.0f,0.0f);
	glRotatef(yangle, 0.0f,1.0f,0.0f);
	printf("[%f,%f,%f,%f,%f]\n", xpos, ypos, zpos, xangle, yangle);
}

void LoadTexture(CString fileName, GLuint m_Texture, GLint m_texWrap, GLint m_texFilter, GLfloat m_texMode)
{
	//Load Texture
	AUX_RGBImageRec* m_texture;
	m_texture = auxDIBImageLoad(fileName);
	if(!m_texture)
	{
		//std::cout<<"Picture could not be loaded";
		//exit(1);
		return;
	}
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_texWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_texWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_texFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_texFilter);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_texMode);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_texture->sizeX,m_texture->sizeY, GL_RGB, GL_UNSIGNED_BYTE, m_texture->data);
}

void ErrorMsg(void)
{
	GLenum errCode;
	const GLubyte *errString;
	while((errCode=glGetError())!=GL_NO_ERROR){
		errString=gluErrorString(errCode);
		fprintf(stderr,"OpenGL Error:%s\n",errString);
		printf("OpenGL Error:%s\n",errString);
	}
}