#pragma once

#include <vector>

typedef struct POINT3D {
	//double x;
	//double y;
	//double z;
	double pt3[3];
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
	void load(wchar_t * fp);
	void unity(double a);
	double range;
	void warp(double x, double y, double z);
};

