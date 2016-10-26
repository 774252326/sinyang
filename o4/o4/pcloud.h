#pragma once

#include <vector>
#include <algorithm>

typedef struct POINT3D {
	//double x;
	//double y;
	//double z;
	double pt[3];
} point3d;

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
	void unity(double a);
	void getminmax(void);
	void align(void);


	double range;
	void warp(double x, double y, double z);
	void warp1();


	std::vector< std::vector< std::vector< std::vector<long> > > > voxel;
	void genvoxel(double d);


	int slvrt2(double a, double b, double c, double &r1, double &r2);

	int findBallCenter(point3d p1, point3d p2, point3d p3, double r, point3d &c1, point3d &c2);

	double dist(point3d p1, point3d p2);
	bool eq0(double x);
	//not used=0;on front=1;used=2;
	std::vector<int> pstatelist;

	bool seedtrg(double r, long &pi1, long &pi2, long &pi3);

	std::vector< std::vector<long> > triangle;

	void nbvidx(long x, long y, long z, std::vector<long> &nb);

	std::vector< std::vector<long> > F;

	long ballPivot(long pi1, long pi2, point3d oldc, double r, point3d &newc);
};

