

#include <GL/glut.h>

#include <stdlib.h>

#include<math.h>

#include<stdio.h>

#define PI 3.14159

#define D 4

 

float alpha = 0;

float spin = 0;

 

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

GLfloat ambient[] ={0.2,0.2,0.2,0.0};//环境光

GLfloat diffuse[] ={1.0,0.5,0.5,0.5};//漫反射特性

GLfloat specular[] ={0.5,0.5,0.5,0.0 };//镜面反射光色

GLfloat shininess[] ={100.0}; //镜面反射的光亮度

 

 

glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);

glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);

glMaterialfv(GL_FRONT,GL_SPECULAR,specular);

glMaterialfv(GL_FRONT,GL_SHININESS,shininess);

glMaterialfv(GL_FRONT, GL_EMISSION, emission);

 

glMatrixMode(GL_MODELVIEW);

glLoadIdentity();

 

}

 

void display()

{

GLfloat lightposition[] = {0.0, 0.0, 0.0, 1.0 };//光源位置

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  

glMatrixMode(GL_MODELVIEW);

glPushMatrix();

//glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

glLoadIdentity();



gluLookAt( D*sin(alpha*PI/180),180-alpha, D*cos(alpha*PI/180), 0.0, 0.0, 0.0, 0.0, 1.0, 1.0);

 

//glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

 

glutSolidTeapot(0.5);

glPopMatrix();

glutSwapBuffers();

 

}

 

void reshape(int w, int h)

{

glViewport(0, 0, (GLsizei)w, (GLsizei)h);

glMatrixMode(GL_PROJECTION);

glLoadIdentity();

gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);

 

glMatrixMode(GL_MODELVIEW);

 

}

 

void mouse(int button, int state, int x, int y)

{

if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)

{

alpha = alpha + 45;

if(alpha >= 360) alpha -= 360;

spin = spin + 45;

if(spin >= 360) spin -= 360;

glutPostRedisplay();

}

}

 

void myidle()

{

alpha = alpha + 0.1;

if(alpha >= 360) alpha -= 360;

 

glutPostRedisplay();

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

 

glutMainLoop();

 

return 0;

}
