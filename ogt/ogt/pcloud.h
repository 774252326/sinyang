#pragma once

#include <vector>
#include <algorithm>

#include "point3d.h"



typedef struct ROTATEP {
	long pi;
	point3d cp;
	point3d nn;
	double angle;
} rotatep;


bool cmprotp(rotatep p1, rotatep p2);


class pcloud
{
public:
	pcloud(void);
	~pcloud(void);
	//point list
	std::vector<point3d> plist;
	// color list
	std::vector<point3d> clist;

	point3d maxp;
	point3d minp;
	void load(wchar_t * fp);
	void load(char * fp);
	void load2(char * fp);

	void unity(double a);
	void getminmax(void);
	void align(void);


	double range;
	void warp(double x, double y, double z);
	void warp1();


	std::vector< std::vector< std::vector< std::vector<long> > > > voxel;
	void genvoxel(double d);


	//not used=0;on front=1;used=2;
	std::vector<int> pstatelist;

	bool seedtrg(double r, long &pi1, long &pi2, long &pi3, point3d &c);

	std::vector< std::vector<long> > triangle;

	void nbvidx(long x, long y, long z, std::vector<long> &nb);

	//F[i][0],F[i][1]:edge point index
		//F[i][2]:another vertex in triangle
		//F[i][3]:normal index
		//F[i][4]:center index

	std::vector< std::vector<long> > F;

	std::vector< std::vector<long> > FBoudary;

	//bool cmprotp(rotatep p1, rotatep p2);

	void ballPivot(long fi, long &pi3, point3d &nn, double r, point3d &newc);
	void ballPivot2(long fi, long &pi3, point3d &nn, double r, point3d &newc);
	std::vector< std::vector<long> > triangleList;
	std::vector<point3d> normalList;
	std::vector<point3d> centerList;



	double pi;

	void gunqiu(double r);

	void initialBall(double r);

	void sortlist();
	void uniquelist();

};

