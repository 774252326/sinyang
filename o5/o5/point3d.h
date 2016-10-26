//#ifndef POINT3D_H
//#define POINT3D_H
#pragma once

typedef struct POINT3D {
	//double x;
	//double y;
	//double z;
	double pt[3];
} point3d;

#define vector3d poiint3d

bool leq0(double x);

bool meq0(double x);

bool eq0(double x);

point3d zeropoint3d();

//a vector from p2 to p1
point3d vt21(point3d p1, point3d p2);

double dotpro(point3d v1, point3d v2);

void crosspro(point3d v1, point3d v2, point3d &vp);

double abs3d(point3d v);

double dist(point3d p1, point3d p2);

void triangleNormal(point3d p1, point3d p2, point3d p3, point3d c, point3d &nv);

double triangleRadius(point3d p1, point3d p2, point3d p3);

double rotateAngle(point3d o, point3d no, point3d oc, point3d nc);

bool p3deq(point3d p1, point3d p2);

bool p3dcp(point3d p1, point3d p2);

//#endif
