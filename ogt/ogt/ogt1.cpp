#include "cmn.h"
//#include "colormapT.h"
#include "ColorMap.hpp"
#include "Terrain.hpp"

GLdouble di,tw, el, az;
GLdouble px, py, pz, ro, pi, he;
GLdouble xp, yp, zp, xa, ya;

double kk=2;
double dd=10000;

GLuint charlists;

MousePara mp;

ColorMap am(RGB(255,0,0),RGB(0,0,255));

//All Texture Names
GLuint m_Texture[3]; //保存纹理对象的名称

const int maxh=50;

Terrain tn;

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

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);

	glClearDepth(1.0f);							// 设置深度缓存
	glEnable(GL_DEPTH_TEST);						// 启用深度测试
	glDepthFunc(GL_LEQUAL);							// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	genCharList(charlists);

	di=3;tw=el=az=0;

	px=py=pz=ro=pi=he=0;

	xp=yp=0;xa=ya=0;zp=-190;

	glGenTextures(3, m_Texture);
	LoadTexture("5-best-panoramic-photography.bmp",m_Texture[0]);
	LoadTexture("6-best-panoramic-photography.bmp",m_Texture[1]);
	LoadTexture("9-best-panoramic-photography.bmp",m_Texture[2]);

	//InitTerrain();

	//tn.InitTerrain(maxh);
	//tn.Init("12.bmp");
	tn.Init("C:\\Users\\r8anw2x\\Documents\\MATLAB\\New Folder (5)\\celP5O9eddQF6.bmp");
	am.AddNode(RGB(0,255,0),0.5);
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



void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	normalView(xp,yp,zp,xa,ya);
	//polarView(di,tw,el,az);
	//pilotView(px,py,pz,ro,pi,he);

	glRotatef(90, 1.0f,0.0f,0.0f);
	//DrawTerrain();

	//tn.DrawTerrain(am,maxh);
	tn.Draw();

	//pano(m_Texture[0]);

	//glFlush ();
	//glFinish();

	glutSwapBuffers();
	
	ErrorMsg();
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


///////////////////////////////////keybroad&mouse response////////////////////////////////////////////////////////
void asciikey (unsigned char key, int x, int y)
{
	switch (key) {

	case 127:
		//delete
		break;
	case 27:
		//esc
		break;
	case 8:
		//backspace
		break;

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

	default:
		break;
	}

	mp.x=x;
	mp.y=y;


}

void spkey (int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1 :
		//F1 function key. 
		break;
	case GLUT_KEY_F2 :
		//F2 function key. 
		break;
	case GLUT_KEY_F3 :
		//F3 function key. 
		break;
	case GLUT_KEY_F4 :
		//F4 function key. 
		break;
	case GLUT_KEY_F5 :
		//F5 function key. 
		break;
	case GLUT_KEY_F6 :
		//F6 function key. 
		break;
	case GLUT_KEY_F7 :
		//F7 function key. 
		break;
	case GLUT_KEY_F8 :
		//F8 function key. 
		break;
	case GLUT_KEY_F9 :
		//F9 function key. 
		break;
	case GLUT_KEY_F10 :
		//F10 function key. 
		break;
	case GLUT_KEY_F11 :
		//F11 function key. 
		break;
	case GLUT_KEY_F12 :
		//F12 function key. 
		break;
	case GLUT_KEY_LEFT :
		//Left directional key. 
		break;
	case GLUT_KEY_UP :
		//Up directional key. 
		break;
	case GLUT_KEY_RIGHT :
		//Right directional key. 
		break;
	case GLUT_KEY_DOWN :
		//Down directional key. 
		break;
	case GLUT_KEY_PAGE_UP :
		//Page up directional key. 
		break;
	case GLUT_KEY_PAGE_DOWN :
		//Page down directional key. 
		break;
	case GLUT_KEY_HOME :
		//Home directional key. 
		break;
	case GLUT_KEY_END :
		//End directional key. 
		break;
	case GLUT_KEY_INSERT :
		//Inset directional key. 
		break;
	default:
		break;
	}		
	mp.x=x;
	mp.y=y;
}

void mouseclick(int button, int state, int x, int y)
{
	switch(button){
	case GLUT_LEFT_BUTTON:
		switch(state){
		case GLUT_UP:
			//lbstate=0;
			printf("up");
			break;
		case GLUT_DOWN:
			//lbstate=1;
			printf("down");
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

	mp.button=button;
	mp.state=state;
	mp.x=x;
	mp.y=y;
}

void mousemove(int x, int y)
{
	switch(mp.button){
	case GLUT_LEFT_BUTTON:
		ya+=(x-mp.x)/3.0;
		xa+=(y-mp.y)/3.0;
		glutPostRedisplay();
		break;
	case GLUT_MIDDLE_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	default:
		break;
	}
	mp.x=x;
	mp.y=y;

}

void passivemousemove(int x, int y)
{
	switch(mp.button){
	case GLUT_LEFT_BUTTON:
		break;
	case GLUT_MIDDLE_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	default:
		break;
	}
	mp.x=x;
	mp.y=y;

}
///////////////////////////////////keybroad&mouse response////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	unsigned int dpmode;
	//dpmode=GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH;
	dpmode=GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH;

	glutInit(&argc, argv);
	glutInitDisplayMode ( dpmode );
	glutInitWindowSize (400, 400);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(asciikey);
	glutSpecialFunc(spkey);
	glutMouseFunc(mouseclick);
	glutMotionFunc(mousemove);
	glutPassiveMotionFunc(passivemousemove);

	glutMainLoop();
	return 0;
}