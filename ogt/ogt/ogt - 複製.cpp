#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <GL\GLAUX.H>
#include <GL\glut.h>
#include <stdio.h>
#include <atlstr.h>

#pragma comment(lib, "glaux.lib")

GLdouble di,tw, el, az;
GLdouble px, py, pz, ro, pi, he;
GLdouble xp, yp, zp, xa, ya;

double kk=2;
double dd=10;

GLuint charlists;

int lbstate;
int mx, my;


GLdouble m_texWrap=GL_CLAMP;
GLdouble m_texFilter=GL_DECAL;
GLdouble m_texMode=GL_NEAREST;  
//All Texture Names
GLuint m_Texture[3]; //保存纹理对象的名称




void genCharList(void)
{
	// 为每一个ASCII字符产生一个显示列表
	//isFirstCall = 0;
	const int MAX_CHAR=128;
	// 申请MAX_CHAR个连续的显示列表编号
	charlists = glGenLists(MAX_CHAR);
	// 把每个字符的绘制命令都装到对应的显示列表中
	wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, charlists);
}

void drawString(const char * str)
{
	for(; *str!='\0'; ++str){
		glCallList(charlists + *str);
	}
}

void lighting1(){
	GLfloat light1_ambient[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { -0.0, -5.0, 0.0, 0.0 };
	GLfloat spot_direction[] = { 0.0, 1.0, -0.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
	glEnable(GL_LIGHT1);
}

void lighting0(){
	GLfloat light_diffuse[] = { 1.0, 1.0, 01.0, 1.0 };
	GLfloat light_position[] = { -1.0, 0.0, 01.0, 0.0 };
	//GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 5.0);
	glEnable(GL_LIGHT0);
}

void lighting2(){
	GLfloat light_diffuse[] = { 01.0, 01.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 1.0 };
	//GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT2, GL_POSITION, light_position);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 5.0);
	glEnable(GL_LIGHT2);
}

void polarView(GLdouble distance, GLdouble twist, GLdouble elevation, GLdouble azimuth){
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



void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	GLfloat mat_specular[] = { 10.0, 10.0, 10.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//glEnable(GL_LIGHTING);

	//lighting0();
	//lighting1();
	//lighting2();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	//glClearDepth(1.0f);							// 设置深度缓存
	//glEnable(GL_DEPTH_TEST);						// 启用深度测试
	//glDepthFunc(GL_LEQUAL);							// 所作深度测试的类型
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	genCharList();

	di=3;tw=el=az=0;

	px=py=pz=ro=pi=he=0;

	xp=yp=xa=ya=0;zp=0;




	glGenTextures(3, m_Texture);
	LoadTexture("5-best-panoramic-photography.bmp",0);
	LoadTexture("6-best-panoramic-photography.bmp",1);
	LoadTexture("9-best-panoramic-photography.bmp",2);




}


void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	normalView(xp,yp,zp,xa,ya);
	//polarView(di,tw,el,az);
	//pilotView(px,py,pz,ro,pi,he);

	glRotatef(90,-1,0,0);
	//char buf[50];
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glRasterPos3d(0,0,0);
	//sprintf_s(buf,"77");
	//drawString(buf);

	//glutWireCube(1);
	//glutSolidCube(1);
	//glutSolidSphere(1,50,50);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_Texture[0]);

	// //Front Face
	//glBegin(GL_POLYGON);
	//     glTexCoord2f(0,0);
	//     glVertex3f(-1.0f,-1.0f,0.0f);
	//     glTexCoord2f(1,0);
	//     glVertex3f( 1.0f,-1.0f,0.0f);
	//     glTexCoord2f(1,1);
	//     glVertex3f( 1.0f, 1.0f,0.0f);
	//     glTexCoord2f(0,1);
	//     glVertex3f(-1.0f, 1.0f,0.0f);
	// glEnd();

	GLUquadricObj* obj;
	obj = gluNewQuadric();
	gluQuadricNormals(obj, GLU_SMOOTH);
	gluQuadricTexture(obj, GL_TRUE);
	gluSphere(obj, 1, 200, 200);



	glDisable(GL_TEXTURE_2D);

	//glFlush ();
	glFinish();

	GLenum errCode;
	const GLubyte *errString;
	while((errCode=glGetError())!=GL_NO_ERROR){
		errString=gluErrorString(errCode);
		fprintf(stderr,"OpenGL Error:%s\n",errString);
		printf("OpenGL Error:%s\n",errString);
	}
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();



	//if (w <= h)
	//glOrtho (-kk, kk, -kk*(GLfloat) h/(GLfloat) w, kk*(GLfloat) h/(GLfloat) w,-dd,dd);
	//else
	//glOrtho (-kk*(GLfloat)w/(GLfloat) h, kk*(GLfloat) w/(GLfloat) h, -kk, kk,-dd,dd);

	//glOrtho (-2, 2, -2, 2,-20,20);
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, .01, dd); 

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 'a': /* s key rotates at shoulder */
		di -= 0.1;
		glutPostRedisplay();
		break;
	case 'q': /* s key rotates at shoulder */
		di += 0.1;
		glutPostRedisplay();
		break;

	case 's':
		tw -= 5;
		glutPostRedisplay();
		break;
	case 'd': /* e key rotates at elbow */
		el -= 5;
		glutPostRedisplay();
		break;
	case 'f':
		az -= 5;
		glutPostRedisplay();
		break;

	case 'w':
		tw += 5;
		glutPostRedisplay();
		break;
	case 'e': /* e key rotates at elbow */
		el += 5;
		glutPostRedisplay();
		break;
	case 'r':
		az += 5;
		glutPostRedisplay();
		break;

	case 'z':
		di=2.3;tw=el=az=0;
		glutPostRedisplay();
		break;

		//case '1':
		//	showmesh=!showmesh;
		//	glutPostRedisplay();
		//	break;
		//case '2':
		//	showcont=!showcont;
		//	glutPostRedisplay();
		//	break;
		//case '3':
		//	showface=!showface;
		//	glutPostRedisplay();
		//	break;

		//case '4':
		//	showlabl=!showlabl;
		//	glutPostRedisplay();
		//	break;

	default:
		break;
	}
}

void keyboard2 (unsigned char key, int x, int y)
{
	double ds=0.01;
	int as=5;
	switch (key) {
	case 'j': /* s key rotates at shoulder */
		px += ds;
		glutPostRedisplay();
		break;
	case 'k': /* s key rotates at shoulder */
		py += ds;
		glutPostRedisplay();
		break;
	case 'l':
		pz += ds;
		glutPostRedisplay();
		break;


	case 'u': /* s key rotates at shoulder */
		px -= ds;
		glutPostRedisplay();
		break;
	case 'i': /* s key rotates at shoulder */
		py -= ds;
		glutPostRedisplay();
		break;
	case 'o':
		pz -= ds;
		glutPostRedisplay();
		break;


	case 's': /* e key rotates at elbow */
		ro += as;
		glutPostRedisplay();
		break;
	case 'd':
		pi += as;
		glutPostRedisplay();
		break;
	case 'f':
		he += as;
		glutPostRedisplay();
		break;

	case 'w': /* e key rotates at elbow */
		ro -= as;
		glutPostRedisplay();
		break;
	case 'e':
		pi -= as;
		glutPostRedisplay();
		break;
	case 'r':
		he -= as;
		glutPostRedisplay();
		break;

	default:
		break;
	}
}

void mouseclick(int button, int state, int x, int y)
{
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
		zp*=0.9;
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
		zp/=0.9;
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

void mousemove(int x, int y)
{
	if(lbstate==GLUT_DOWN){
		ya+=(x-mx)/3.0;
		xa+=(y-my)/3.0;
		glutPostRedisplay();
		mx=x;
		my=y;
	}
}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (400, 400);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	//glutKeyboardFunc(keyboard2);
	glutMouseFunc(mouseclick);
	glutMotionFunc(mousemove);

	glutMainLoop();
	return 0;
}