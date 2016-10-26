//#pragma comment( lib, "opengl32.lib" )
//#pragma comment( lib, "glu32.lib" )
//#pragma comment( lib, "glut32.lib")
//#pragma comment(lib, "glaux.lib")
#include "StdAfx.h"
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "ro.h"
#include "triangleMesh.h"
#include "surfaceMesh.h"
#include <vector>
#include <string>

//#include "../../interp/shepard_interp_2d.h"

#include "..\..\funT\paddingT.h"
#include "../../funT/matmulT.h"

//long r=1256,c=3,h=3;
//long r=1604,c=3,h=4;
long r=4696,c=3,h=4;

float ***dt=f3tensor<float>(1,r,1,c,1,h);

static GLdouble di,tw, el, az;

static GLdouble px, py, pz, ro, pi, he;

double kk=2;
double dd=10;

float innerp[3]={0,0,0};

long **face;
double **point;
double *facecolor;
long np;
long nf;

bool showmesh=false;
bool showface=false;
bool showcont=true;

std::vector< std::vector< std::vector<float> > > contourv1;

triangleMesh trm;
surfaceMesh sfm;

void selectNormal(float normal[3], float point[3], float inner[3], bool outflag){
	float s,abs;
	s=normal[0]*(point[0]-inner[0])+normal[1]*(point[1]-inner[1])+normal[2]*(point[2]-inner[2]);
	abs=sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);

	if( (s>0 && !outflag)||(s<0 && outflag) ){
		normal[0]=-normal[0];
		normal[1]=-normal[1];
		normal[2]=-normal[2];
	}
	normal[0]/=abs;
	normal[1]/=abs;
	normal[2]/=abs;
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
	GLfloat light_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat light_position[] = { -1.0, 0.0, 01.0, 0.0 };
	//GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 5.0);
	glEnable(GL_LIGHT0);
}

void lighting2(){
	GLfloat light_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
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

	di=3;tw=el=az=0;

	px=py=pz=ro=pi=he=0;

}

void triangle(void)
{
	long i,j,k;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	//glLineWidth(1);

	float v1[3];
	float v2[3];
	float nor[3];
	float rgba[4];

	float mncv=0.2;
	float mxcv=0.5;

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	for(i=1;i<=r;i++){

		//glColor3f(0.0f, 0.0f, 1.0f); 
		glBegin (GL_TRIANGLES);


		for(k=1;k<=3;k++){
			v1[k-1]=dt[i][1][k]-dt[i][3][k];
			v2[k-1]=dt[i][2][k]-dt[i][3][k];
		}
		nor[0]=v1[1]*v2[2]-v1[2]*v2[1];
		nor[1]=v1[2]*v2[0]-v1[0]*v2[2];
		nor[2]=v1[0]*v2[1]-v1[1]*v2[0];


		//glNormal3f(v1[1]*v2[2]-v1[2]*v2[1],v1[2]*v2[0]-v1[0]*v2[2],v1[0]*v2[1]-v1[1]*v2[0]);
		for(j=1;j<=c;j++){
			//glColor3f(dt[i][j][3]+0.5,0.5-dt[i][j][3],0.5+dt[i][j][1]);
			//glColor3f(1,0,0.50);
			//glNormal3f(v1[1]*v2[2]-v1[2]*v2[1],v1[2]*v2[0]-v1[0]*v2[2],v1[0]*v2[1]-v1[1]*v2[0]);
			//glNormal3f(v1[2]*v2[1]-v1[1]*v2[2],v1[0]*v2[2]-v1[2]*v2[0],v1[1]*v2[0]-v1[0]*v2[1]);


			genColor(rgba,(dt[i][j][4]-mncv)/(mxcv-mncv)*0.9);
			glColor4fv(rgba);
			selectNormal(nor,&dt[i][j][1],innerp,true);

			glNormal3fv(nor);


			glVertex3f(dt[i][j][1],dt[i][j][2],dt[i][j][3]);
		}
		glEnd();

		//glLineWidth(2);
		//glColor3f(1.0f, 0.0f, 0.0f); 
		//glBegin (GL_LINE_LOOP);
		//for(j=1;j<=c;j++){
		//	glVertex3f(dt[i][j][1],dt[i][j][2],dt[i][j][3]);
		//}
		//glEnd();
		////glLineWidth(1);
	}
}


void triangle1(void)
{
	long i,j,k;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);



	float v1[3];
	float v2[3];
	float nor[3];
	float rgba[4];

	float mncv=findmin(facecolor,nf);
	float mxcv=findmax(facecolor,nf);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	for(i=1;i<=nf;i++){


		glBegin (GL_TRIANGLES);


		//for(k=1;k<=3;k++){
		//	v1[k-1]=dt[i][1][k]-dt[i][3][k];
		//	v2[k-1]=dt[i][2][k]-dt[i][3][k];
		//}
		//nor[0]=v1[1]*v2[2]-v1[2]*v2[1];
		//nor[1]=v1[2]*v2[0]-v1[0]*v2[2];
		//nor[2]=v1[0]*v2[1]-v1[1]*v2[0];



		for(j=1;j<=c;j++){



			genColor(rgba,(point[face[i][j]][4]-mncv)/(mxcv-mncv)*0.9);
			glColor4fv(rgba);

			//selectNormal(nor,&dt[i][j][1],innerp,true);

			//glNormal3fv(nor);


			glVertex3f(point[face[i][j]][1],point[face[i][j]][2],point[face[i][j]][3]);
		}
		glEnd();

		glLineWidth(2);
		glColor3f(1.0f, 1.0f, 1.0f); 
		glBegin (GL_LINE_LOOP);
		for(j=1;j<=c;j++){
			glVertex3f(point[face[i][j]][1],point[face[i][j]][2],point[face[i][j]][3]);
		}
		glEnd();
		glLineWidth(1);
	}
}


void triangle2(void)
{
	long i,j,k;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	float mncv=trm.minPointValue;
	float mxcv=trm.maxPointValue;

	float v1[3];
	float v2[3];
	float nor[3];
	float rgba[4];

	if(showcont){
		glLineWidth(1);
		for(i=0;i<trm.contourv.size();i++){
			genColor(rgba,(trm.contourValue[i]-mncv)/(mxcv-mncv)*0.9);
			glColor4fv(rgba);
			glBegin (GL_LINE_STRIP);
			for(j=0;j<trm.contourv[i][0].size();j++){
				glVertex3f(trm.contourv[i][0][j],trm.contourv[i][1][j],trm.contourv[i][2][j]);
			}
			glEnd();
		}
	}

	if(showmesh){
		glLineWidth(1);
		glColor3f(1.0f, 1.0f, 1.0f); 
		for(i=1;i<=trm.nface;i++){
			glBegin (GL_LINE_LOOP);
			for(j=1;j<=3;j++){
				glVertex3f(trm.point[trm.face[i][j]][1],trm.point[trm.face[i][j]][2],trm.point[trm.face[i][j]][3]);
			}
			glEnd();
		}
		//glLineWidth(1);
	}

	if(showface){
		for(i=1;i<=trm.nface;i++){
			glBegin (GL_TRIANGLES);
			for(j=1;j<=3;j++){		
				genColor(rgba,(trm.pointValue[trm.face[i][j]]-mncv)/(mxcv-mncv)*0.9);
				glColor4fv(rgba);
				glVertex3f(trm.point[trm.face[i][j]][1],trm.point[trm.face[i][j]][2],trm.point[trm.face[i][j]][3]);
			}
			glEnd();
		}
	}



}

void triangle3(void)
{
	long i,j,k;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	float mncv=sfm.minPointValue;
	float mxcv=sfm.maxPointValue;

	float v1[3];
	float v2[3];
	float nor[3];
	float rgba[4];

	if(showcont){
		glLineWidth(1);
		for(i=0;i<sfm.contourv.size();i++){
			genColor(rgba,(sfm.contourValue[i]-mncv)/(mxcv-mncv)*0.9);
			glColor4fv(rgba);
			glBegin (GL_LINE_STRIP);
			for(j=0;j<sfm.contourv[i].size();j++){
				glVertex3dv(sfm.contourv[i][j].data());
			}
			glEnd();
		}
	}

	if(showmesh){
		glLineWidth(1);
		glColor3f(1.0f, 1.0f, 1.0f); 
		for(i=1;i<sfm.triangle.size();i++){
			glBegin (GL_LINE_LOOP);
			for(j=0;j<sfm.triangle[i].size();j++){
				glVertex3dv(sfm.point[sfm.triangle[i][j]].data());
			}
			glEnd();
		}
		//glLineWidth(1);
	}

	if(showface){
		for(i=1;i<sfm.triangle.size();i++){
			glBegin (GL_TRIANGLES);
			for(j=0;j<sfm.triangle[i].size();j++){		
				genColor(rgba,(sfm.pointValue[sfm.triangle[i][j]]-mncv)/(mxcv-mncv)*0.9);
				glColor4fv(rgba);
				glVertex3dv(sfm.point[sfm.triangle[i][j]].data());
			}
			glEnd();
		}
	}



}

void p4(void){
	//glBegin (GL_LINE_LOOP);

	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	float sqrt3=0.57735026918962576450914878050196;

	glBegin (GL_TRIANGLES);

	glNormal3f(0,0,-1);
	glVertex3f (0, 0, 0);
	//glNormal3f(0,0,-1);
	glVertex3f (1, 0, 0);
	//glNormal3f(0,0,-1);
	glVertex3f (0, 1, 0);


	glNormal3f(-1,0,0);
	glVertex3f (0, 0, 0);
	//glNormal3f(-1,0,0);
	glVertex3f (0, 0, 1);
	//glNormal3f(-1,0,0);
	glVertex3f (0, 1, 0);



	glNormal3f(0,-1,0);
	glVertex3f (0, 0, 0);
	//glNormal3f(0,-1,0);
	glVertex3f (1, 0, 0);
	//glNormal3f(0,-1,0);
	glVertex3f (0, 0, 1);



	glNormal3f(1*sqrt3,1*sqrt3,1*sqrt3);
	glVertex3f (0, 0, 1);
	//glNormal3f(1*sqrt3,1*sqrt3,1*sqrt3);
	glVertex3f (1, 0, 0);
	//glNormal3f(1*sqrt3,1*sqrt3,1*sqrt3);
	glVertex3f (0, 1, 0);


	glEnd();


}


void p44(void){
	//glBegin (GL_LINE_LOOP);

	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	float sqrt3=0.57735026918962576450914878050196;

	glBegin (GL_TRIANGLE_FAN);


	glColor3f(1,0,0);
	glVertex3f (0, 0, 0);
	glColor3f(0,1,0);
	glVertex3f (1, 0, 0);
	glColor3f(0,1,1);
	glVertex3f (1, 1, 0);
	glColor3f(0,0,1);
	glVertex3f (0, 1, 0);

	glEnd();


	glBegin (GL_POLYGON);


	glColor3f(1,0,0);
	glVertex3f (0, 0, 0);
	glColor3f(0,1,0);
	glVertex3f (-1, 0, 0);
	glColor3f(0,1,1);
	glVertex3f (-1, -1, 0);
	glColor3f(0,0,1);
	glVertex3f (0, -1, 0);




	glEnd();


	glBegin (GL_POLYGON);


	glColor3f(1,0,0);
	glVertex3f (0, 0, 0);
	glColor3f(0,1,0);
	glVertex3f (-1, 0, 0);
	glColor3f(0,1,1);
	glVertex3f (-1, 1, 0);
	glColor3f(0,0,1);
	glVertex3f (0, 1, 0);


	glEnd();

	glBegin (GL_TRIANGLE_STRIP);


	glColor3f(1,0,0);
	glVertex3f (0, 0, 0);
	glColor3f(0,0,1);
	glVertex3f (0, -1, 0);
	glColor3f(0,1,0);
	glVertex3f (1, 0, 0);
	glColor3f(0,1,1);
	glVertex3f (1, -1, 0);


	glEnd();
}


void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	polarView(di,tw,el,az);

	//pilotView(px,py,pz,ro,pi,he);

	//triangle();

	//triangle1();
	triangle2();
	//triangle3();
	//p44();

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
	//lighting2();
	//gluLookAt(0,2,0,0,0,0,0,0,1);
	//pilotView(px,py,pz,ro,pi,he);

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
		tw = (tw + 5);
		glutPostRedisplay();
		break;
	case 'd': /* e key rotates at elbow */
		el = (el + 5);
		glutPostRedisplay();
		break;
	case 'f':
		az += 5;
		glutPostRedisplay();
		break;
	case 'z':
		di=2.3;tw=el=az=0;
		glutPostRedisplay();
		break;

	case '1':
		showmesh=!showmesh;
		glutPostRedisplay();
		break;
	case '2':
		showcont=!showcont;
		glutPostRedisplay();
		break;
	case '3':
		showface=!showface;
		glutPostRedisplay();
		break;

	default:
		break;
	}
}

void keyboard2 (unsigned char key, int x, int y)
{
	switch (key) {
	case 'q': /* s key rotates at shoulder */
		px += 0.1;
		glutPostRedisplay();
		break;
	case 'w': /* s key rotates at shoulder */
		py += 0.1;
		glutPostRedisplay();
		break;
	case 'e':
		pz += 0.1;
		glutPostRedisplay();
		break;

	case 'a': /* e key rotates at elbow */
		ro += 5;
		glutPostRedisplay();
		break;
	case 's':
		pi += 5;
		glutPostRedisplay();
		break;
	case 'd':
		he+=5;
		glutPostRedisplay();
		break;

	case 'z': /* s key rotates at shoulder */
		px -= 0.1;
		glutPostRedisplay();
		break;
	case 'x': /* s key rotates at shoulder */
		py -= 0.1;
		glutPostRedisplay();
		break;
	case 'c':
		pz -= 0.1;
		glutPostRedisplay();
		break;

	default:
		break;
	}
}


//#include <string>
//#include <vector>
//#include <fstream>
////using namespace std;
//using std::string;


//const std::vector<char> FileData(
//	(
//	std::istreambuf_iterator<char>(
//	std::ifstream(
//	FileName, std::ios::ios_base::binary
//	)
//	)
//	), std::istreambuf_iterator<char>()
//	);  
//
//std::vector< std::vector<float> > read2(char *fn){
//	std::ifstream ifs(fn);
//		float temp;
//		std::vector<float> line;
//		std::vector< std::vector<float> > mx;
//
//	if(ifs.bad()){
//		return mx;
//	}
//	else{
//
//		mx.push_back(line);
//		int i;
//		while(!ifs.eof()){
//			ifs>>temp;
//			i=mx.size();
//			mx[i-1].push_back(temp);
//			if(mx[i-1].size()>=10){
//				mx.push_back(line);
//			}
//		}
//
//		ifs.close();
//		return mx;
//	}
//
//	if( ifs.is_open() ){
//		while( ifs.good() && !ifs.eof() ){
//			if( mx.empty() ){
//				mx.push_back(line);
//			}
//			else{
//				if(
//
//}



int main(int argc, char** argv)
{
	time_t start,end;

	//wchar_t fp[]=L"C:\\Users\\r8anw2x\\Documents\\Visual Studio 2010\\Projects\\testgrid2.txt";
	//char fp[]="C:\\Users\\r8anw2x\\Documents\\Visual Studio 2010\\Projects\\grid1.txt";
	//char fp[]="C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\octahedron.txt";
	//char fp[]="C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\gridTopSurface04022013_2.txt";
	char fp[]="C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\05021013\\gridSurface05022013_2ring.txt";
	//char fp[]="C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\05021013\\gridSurface05022013_2.txt";


	char pfp[]="C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\gridTopSurface04022013_2.point.txt";
	char tfp[]="C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\gridTopSurface04022013_2.triangle.txt";
	char pvfp[]="C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\gridTopSurface04022013_2.pointvalue.txt";
	char pvifp[]="C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\gridTopSurface04022013_2.pointvalueinterpolate.txt";
	char tvfp[]="C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\gridTopSurface04022013_2.trianglevalue.txt";



	//readf2(fp,dt[1],r,c*h);
	long i,j,k;
	double **dtt;
	dtt=readf2<double>(fp,&nf,10);

	//std::vector< std::vector<float> > mx;
	//mx=read2(fp);

	//for(i=0;i<mx.size();i++){
	//	for(j=0;j<10;j++){
	//		std::cout<<mx[i][j]<<' ';
	//	}
	//}


	facecolor=vector<double>(1,nf);

	//for(i=1;i<=nf;i++){
	//	facecolor[i]=dtt[i][10];
	//}


	//std::string prefix = "grid04022013.txt";
	//std::string str = "C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\" + prefix;	

	//std::ifstream myfile;
	//myfile.open(str,std::ifstream::in);

	//double tmp;
	//std::vector<double> data;
	//while( myfile >> tmp >> tmp2 >> temp3 ) {
	//	data.push_back(tmp);
	//}
	//myfile.close();

	//for(i = 0; i < data.size(); i++) 
	//	std::cout << data[i] << std::endl;

	face=matrix<long>(1,nf,1,3);


	point=genPFlist(dtt,nf,face,facecolor,&np);

	//pdt(point,np,4);
	//pdt(face,nf,3);

	double *pv=vector<double>(1,np);
	for(i=1;i<=np;i++){
		pv[i]=point[i][4];
	}




	//writef2(pfp,trm.point,trm.npoint,3);
	//writef2(tfp,trm.face,trm.nface,3);
	//writef1(pvfp,trm.pointValue,trm.npoint);
	//writef1(tvfp,trm.faceValue,trm.nface);





	//double **fxyz=matrix<double>(1,3,1,nf);
	//mattp(trm.faceCentroid,1,trm.nface,1,3,fxyz);

	//double **pxyz=matrix<double>(1,3,1,np);
	//mattp(trm.point,1,trm.npoint,1,3,pxyz);

	//double *fv=vector<double>(1,nf);
	//copyvt(trm.faceValue,trm.nface,fv);

	//double *npv=shepard_interp_2d(nf,&fxyz[1][1],&fxyz[2][1],&fv[1],2,np,&pxyz[1][1],&pxyz[2][1]);

	//copyvt(&npv[-1],np,pv);

	






	time (&start);
	//trm.interpPointV();

	//std::cout<<'['<<trm.minFaceValue<<','<<trm.maxFaceValue<<']'<<'\n';
	//std::cout<<'['<<trm.minPointValue<<','<<trm.maxPointValue<<']'<<'\n';


	//sfm.loadPointAndFace(point,np,face,nf);
	////sfm.loadPoint(point,np);
	////sfm.loadFace(face,nf);
	////sfm.genEdge();
	////sfm.genPointToFaceMap();
	////sfm.genFaceCentroid();
	//sfm.loadFaceV(facecolor,nf);
	//sfm.loadPointV(pv,np);
	//sfm.genContourMap(50);

	
	trm.loadPointAndFace(point,np,face,nf,0);
	trm.loadFaceV(facecolor,nf);
	trm.loadPointV(pv,np);
	trm.genContourMap(50);

	time (&end);



	printf ("Elapsed time %.2lf seconds.\n", difftime (end,start) );
	//writef1(pvifp,trm.pointValueInterp,trm.npoint);


	//trm.pointValue[3]=1;
	//trm.pointValue[6]=1;
	//trm.pointValue[1]=0.5;
	//trm.pointValue[2]=0.5;
	//trm.pointValue[4]=0.5;
	//trm.pointValue[5]=1;


	//float pv[7]={0,0.5,0.5,1,0.5,1,1};


	


	//el=trm.nextEdgeFromEdge(3,&nel);
	//el=trm.nextEdgeFromPoint(3,1,&nel);
	//std::cout<<std::endl;
	//pdt(el,nel);

	//readf3(fp,dt,r,c,h);
	//pdt(dt,r,c,h);

	//const char *v;int vv;
	//v=(char*)glGetString(GL_VERSION);

	//glGetIntegerv(GL_MAJOR_VERSION,&vv);
	//printf("opengl version=%c\n",v[0]);


	//glutInit(&argc, argv);
	glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
	//glutInitWindowSize (400, 400);
	//glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}