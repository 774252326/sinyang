#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <GL\GLAUX.H>
#include <GL\glut.h>
#include <stdio.h>
#include "pcloud.h"
#include <time.h> 
#include <fstream>

GLdouble di,tw, el, az;
GLdouble px, py, pz, ro, pi, he;
GLdouble xp, yp, zp, xa, ya;

double kk=2;
double dd=10;

GLuint charlists;

int lbstate;
int mx, my;

pcloud pc;

double qr;

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



///////////////////////////////////////////////////////////////
void startup()
{
	//char fp[]="C:\\Users\\r8anw2x\\Dropbox\\xyz.wrl";
	//char fp[]="C:\\Users\\r8anw2x\\Dropbox\\ball[1][40][40].wrl.result.txt";
	char fp[]="C:\\Users\\r8anw2x\\Documents\\MATLAB\\byo3d\\mlStructuredLight\\ball[1][100][100].wrl";

	pc.load(fp);
	pc.sortlist();
	pc.uniquelist();

	for(size_t i=0;i<pc.clist.size();i++){
		pc.clist[i].pt[0]=1;
	}

	qr=10;
	clock_t t=clock();
	pc.initialBall(qr);

	pc.gunqiu(qr);

	printf("\n%d ms\n",clock()-t);

	char buf[500];
	int nn;
	nn=sprintf_s(buf,"%s.result.txt",fp);
	std::ofstream outfile (buf,std::ofstream::binary);
	outfile<<"point list:\n";
	for(size_t i=0;i<pc.plist.size();i++){
		outfile<<pc.plist[i].pt[0]<<','<<pc.plist[i].pt[1]<<','<<pc.plist[i].pt[2]<<','<<'\n';
	}
	outfile<<"triangle list:\n";
	for(size_t i=0;i<pc.triangleList.size();i++){
		for(size_t j=0;j<pc.triangleList[i].size();j++){
			outfile<<pc.triangleList[i][j]<<',';
		}
		outfile<<'\n';
	}
	outfile<<"triangle normal list:\n";
	for(size_t i=0;i<pc.normalList.size();i++){
		outfile<<pc.normalList[i].pt[0]<<','<<pc.normalList[i].pt[1]<<','<<pc.normalList[i].pt[2]<<','<<'\n';
	}

	outfile.close();


	//pc.load2(fp);
}


void drawpoint(int ci)
{
	if(ci==0){
		////////point index////////////
		char buf[50];
		int nn;
		glDisable(GL_LIGHTING);
		glColor3d(1,1,1);
		for(size_t i=0;i<pc.plist.size();i++){	
			//glColor3dv(pc.clist[i].pt);	
			glRasterPos3dv(pc.plist[i].pt);
			//nn=sprintf(buf,".");
			nn=sprintf_s(buf,"%d",i);
			glListBase(charlists);
			glCallLists(nn, GL_UNSIGNED_BYTE, buf);
		}

	}
	else{
		////////////////point//////////////////////////
		char dot[1]={'.'};
		glDisable(GL_LIGHTING);
		glColor3d(1,1,1);
		for(size_t i=0;i<pc.plist.size();i++){	
			//glColor3dv(pc.clist[i].pt);			
			glRasterPos3dv(pc.plist[i].pt);
			glListBase(charlists);
			glCallLists(1, GL_UNSIGNED_BYTE, dot);
		}

	}

}


void drawfront()
{
	////////////////////////////////////front//////////////////////////////////////
	glDisable(GL_LIGHTING);
	if(!pc.F.empty()){
		glColor3d(1,1,0);
		glBegin(GL_LINES);
		glVertex3dv(pc.plist[pc.F.front()[0]].pt);
		glVertex3dv(pc.plist[pc.F.front()[1]].pt);
		glEnd();

		glColor3d(1,0,0);
		for(size_t i=1;i<pc.F.size();i++){
			glBegin(GL_LINES);
			glVertex3dv(pc.plist[pc.F[i][0]].pt);
			glVertex3dv(pc.plist[pc.F[i][1]].pt);
			glEnd();
		}
	}

}


void drawtri(int ci)
{
	////////////////////triangle mesh////////////////////////
	if(ci==0){
		glEnable(GL_LIGHTING);
		glColor3d(0,1,1);
		for(size_t i=0;i<pc.triangleList.size()-1;i++){
			//glBegin (GL_LINE_LOOP);
			glBegin (GL_TRIANGLES);
			glNormal3dv(pc.normalList[i].pt);
			for(size_t j=0;j<pc.triangleList[i].size();j++){
				glVertex3dv(pc.plist[pc.triangleList[i][j]].pt);
			}
			glEnd();
		}

		glColor3d(0,0,1);
		//glBegin (GL_LINE_LOOP);
		glBegin (GL_TRIANGLES);
		glNormal3dv(pc.normalList.back().pt);
		for(size_t j=0;j<pc.triangleList.back().size();j++){
			glVertex3dv(pc.plist[pc.triangleList.back()[j]].pt);
		}
		glEnd();
	}
	else{
		glDisable(GL_LIGHTING);
		glColor3d(0,1,1);
		for(size_t i=0;i<pc.triangleList.size()-1;i++){
			glBegin (GL_LINE_LOOP);
			//glBegin (GL_TRIANGLES);
			glNormal3dv(pc.normalList[i].pt);
			for(size_t j=0;j<pc.triangleList[i].size();j++){
				glVertex3dv(pc.plist[pc.triangleList[i][j]].pt);
			}
			glEnd();
		}

		glColor3d(0,0,1);
		//glBegin (GL_LINE_LOOP);
		glBegin (GL_TRIANGLES);
		glNormal3dv(pc.normalList.back().pt);
		for(size_t j=0;j<pc.triangleList.back().size();j++){
			glVertex3dv(pc.plist[pc.triangleList.back()[j]].pt);
		}
		glEnd();
	}
}



void draws()
{
	glTranslated(pc.maxp.pt[0]/-2, pc.maxp.pt[1]/-2, pc.maxp.pt[2]/-2);
	drawfront();
	drawtri(0);
	//drawpoint(1);
}


//////////////////////////////////////////////////////////////////////



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
	glTranslated(xpos, ypos, zpos);
	glRotated(xangle, 1.0f,0.0f,0.0f);
	glRotated(yangle, 0.0f,1.0f,0.0f);
	printf("[%f,%f,%f,%f,%f]\n", xpos, ypos, zpos, xangle, yangle);
}


void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	GLfloat mat_specular[] = { 10.0, 10.0, 10.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);

	//lighting0();
	//lighting1();
	lighting2();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	//glClearDepth(1.0f);							// 设置深度缓存
	//glEnable(GL_DEPTH_TEST);						// 启用深度测试
	//glDepthFunc(GL_LEQUAL);							// 所作深度测试的类型
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	genCharList();

	di=3;tw=el=az=0;

	px=py=pz=ro=pi=he=0;

	xp=yp=xa=ya=0;zp=-3;




}


void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	normalView(xp,yp,zp,xa,ya);
	//polarView(di,tw,el,az);
	//pilotView(px,py,pz,ro,pi,he);


	//char buf[50];
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glRasterPos3d(0,0,0);
	//sprintf_s(buf,"77");
	//drawString(buf);

	//glutWireCube(1);

	draws();

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

void keyboard3 (unsigned char key, int x, int y)
{
	switch (key) {
	case 'g': /* s key rotates at shoulder */
		pc.gunqiu(qr);
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

	startup();

	glutInit(&argc, argv);
	glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (400, 400);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard3);
	glutMouseFunc(mouseclick);
	glutMotionFunc(mousemove);

	glutMainLoop();
	return 0;
}