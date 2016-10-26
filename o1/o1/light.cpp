
#include "StdAfx.h"

#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

static float theta=0;
float pi=3.1415926535897932384626433832795;


void lighting(){
		GLfloat light1_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { -1.0, 0.0, 1.0, 0.0 };
	GLfloat spot_direction[] = { 1.0, -0.0, -1.0 };
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

void init(void)
{

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);


glEnable(GL_LIGHTING);


	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


	GLfloat light_position[] = { 100.0, 100.0, 100.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		//glEnable(GL_LIGHT0);

	//GLfloat light1_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
	//GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light1_position[] = { -1.0, 0.0, 1.0, 0.0 };
	//GLfloat spot_direction[] = { 1.0, -0.0, -1.0 };
	//glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	//glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	//glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	//glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

	//glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

	//glEnable(GL_LIGHT1);


		lighting();


	glEnable(GL_DEPTH_TEST);






}
void display(void)
{
	//glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////glutSolidSphere (1.0, 200, 16);
	////glutSolidTeapot(1);
	////glutSolidOctahedron();
	//glMatrixMode(GL_MODELVIEW);
	//

	//////glMatrixMode (GL_PROJECTION);
	//glPushMatrix();
	////gluLookAt(2*cos(theta),0,2*sin(theta),0,0,0,0,1,0);
	//
	////glRotatef((GLfloat)theta,0,1,0);
	//glutSolidTorus(.5,1,200,200);

	//glPopMatrix();

	//glFlush ();
	////glutSwapBuffers();




	GLfloat light_position[] = {1.5, 0.0, 0.0,  1.0 };
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glPushMatrix();
gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
glPushMatrix();
glRotated(theta, 0.0, 1.0, 0.0);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glPopMatrix();
glTranslatef(1.5,0,0);
glutSolidTorus (0.275, 0.85, 28, 150);
glPopMatrix();
glFlush();



}
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
	else
		glOrtho (-1.5*(GLfloat)w/(GLfloat)h,1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void go(void){
	theta+=.1;
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutIdleFunc(go);

	glutMainLoop();
	return 0;
}