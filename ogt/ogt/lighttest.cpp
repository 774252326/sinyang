#include <GL/glut.h>

#include <stdlib.h>

#include<math.h>

#include<stdio.h>

#include <atlstr.h>
#include <GL\GLAUX.H>

#pragma comment(lib, "glaux.lib")



#define PI 3.14159

#define D 1

int lbstate;
int mx, my;

float alpha = 0;
float spin = 0;

GLdouble ya=0,xa=180;
float mox=0,moy=0,moz=10;

GLdouble m_texWrap=GL_CLAMP;
GLdouble m_texFilter=GL_DECAL;
GLdouble m_texMode=GL_NEAREST;  
//All Texture Names
GLuint m_Texture[3]; //保存纹理对象的名称


void LoadTexture(CString fileName, int texName)
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
	glBindTexture(GL_TEXTURE_2D, m_Texture[texName]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_texWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_texWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_texFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_texFilter);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_texMode);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_texture->sizeX,m_texture->sizeY, GL_RGB, GL_UNSIGNED_BYTE, m_texture->data);
}


void pano(GLuint Texture)
{
	//glRotatef(90,-1,0,0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,Texture);

	GLUquadricObj* obj;
	obj = gluNewQuadric();
	gluQuadricNormals(obj, GLU_SMOOTH);
	gluQuadricTexture(obj, GL_TRUE);
	gluSphere(obj, 1, 200, 200);

	glDisable(GL_TEXTURE_2D);
}

void init()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat lightambient[] = {1.0,1.0,1.0,1.0};//环境光
	GLfloat lightdiffuse[] = {1.0,1.0,1.0,1.0};//漫反射
	GLfloat lightspecular[] = {1.0,1.0,1.0,1.0 };//镜面反射光
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightdiffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,lightspecular);
	glLightModelf(GL_LIGHT_MODEL_AMBIENT, (0.0, 0.0, 0.0));
	GLfloat emission[] = {0.0, 0.0, 0.0, 1.0};//发射光
	GLfloat ambient[] ={0.1,0.1,0.1,0.0};//环境光
	GLfloat diffuse[] ={0.50,0.5,0.5,0.5};//漫反射特性
	GLfloat specular[] ={0.5,0.5,0.5,0.0 };//镜面反射光色
	GLfloat shininess[] ={100.0}; //镜面反射的光亮度

	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glGenTextures(3, m_Texture);
	//LoadTexture("5-best-panoramic-photography.bmp",0);
	//LoadTexture("6-best-panoramic-photography.bmp",1);
	//LoadTexture("9-best-panoramic-photography.bmp",2);

	LoadTexture("14596142-start-track-line-on-a-red-running-track.bmp",0);

}



void display()
{

	GLfloat lightposition[] = {0, 0, 1, 1.0 };//光源位置
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	//glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	glPushMatrix();
	gluLookAt(
		//mox, moy, moz,
		D*cos(ya*PI/180)*sin(xa*PI/180), D*sin(ya*PI/180), D*cos(ya*PI/180)*cos(xa*PI/180),
		0, 0, 0, 
		0.0, 1.0, 0.0);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
	glPushMatrix();
	
glTranslatef(mox+D*cos(ya*PI/180)*sin(xa*PI/180), moy+D*sin(ya*PI/180), moz+D*cos(ya*PI/180)*cos(xa*PI/180));
	printf("%g,%g,%g\n",xa,ya,moz);
	//glutSolidTeapot(0.5);



	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_Texture[0]);

	for(int i=-5;i<=5;i++){
	// //Front Face
	glBegin(GL_POLYGON);
	     glTexCoord2f(0,0);
	     glVertex3f(-1.0f,-1.0f,-10.0f+i*20);
	     glTexCoord2f(1,0);
	     glVertex3f( 1.0f,-1.0f,-10.0f+i*20);
	     glTexCoord2f(1,1);
	     glVertex3f( 1.0f,-1.0f, 10.0f+i*20);
	     glTexCoord2f(0,1);
	     glVertex3f(-1.0f,-1.0f, 10.0f+i*20);
	 glEnd();
	}

	//glDisable(GL_TEXTURE_2D);




	glPopMatrix();

	glutSwapBuffers();
	//依据提交光源位置前、后的不同，有四种情况
	//
	//1 前、后都被注释掉。这时启用默认位置（0.0, 0.0, 1.0, 0.0），在摄像机坐标系中度量的，我们将看到Teapot永远是亮的。
	//
	//2 启用前，效果同上，光源和视点一起运动，可以理解为光源位置在摄像机坐标系中度量的。
	//
	//3 启用后，我们会看到Teapot的明暗变化，视点运动，光源固定在世界坐标系中，光源等同于普通几何对象。
	//
	//4 前、后都启用， 效果同3，这时前被后屏蔽，后起作用。



}



void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, .01, 20.0);
	glMatrixMode(GL_MODELVIEW);
}



void mouse(int button, int state, int x, int y)
{

	//if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	//{

	//	alpha = alpha + 45;

	//	if(alpha >= 360) alpha -= 360;

	//	spin = spin + 45;

	//	if(spin >= 360) spin -= 360;

	//	glutPostRedisplay();

	//}


	switch(button){
	case GLUT_LEFT_BUTTON:

		mx=x;
		my=y;
		lbstate=state;
		switch(state){
		case GLUT_UP:
			//lbstate=0;
			break;
		case GLUT_DOWN:
			//lbstate=1;
			break;
		default:
			break;
		}
		break;

	case GLUT_MIDDLE_BUTTON:
		//zp*=0.9;
		mox-=0.1;
		glutPostRedisplay();

		switch(state){
		case GLUT_UP:
			break;
		case GLUT_DOWN:
			break;
		default:
			break;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		//zp/=0.9;
		mox+=.1;
		glutPostRedisplay();
		switch(state){
		case GLUT_UP:
			break;
		case GLUT_DOWN:
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}


}



void myidle()
{

	alpha = alpha + 0.1;
	if(alpha >= 360) alpha -= 360;

	moz+=0.001;
	if(moz>10)
		moz=-10;

	glutPostRedisplay();

}



void mousemove(int x, int y)
{
	if(lbstate==GLUT_DOWN){
		xa+=(x-mx)/3.0;
		ya+=(y-my)/3.0;
		glutPostRedisplay();
		mx=x;
		my=y;
	}
}



int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RED | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("move light");

	glutIdleFunc(myidle);

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glutMotionFunc(mousemove);

	glutMainLoop();
	return 0;

}
