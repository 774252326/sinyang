#pragma once

#include <vector>

class triangleMesh
{
public:
	triangleMesh(void);
	triangleMesh(long,long);
	~triangleMesh(void);

	long nface;
	long npoint;
	float **point;
	long **face;
	float *pointValue;
	float *faceValue;

	float maxPointValue;
	float minPointValue;
	float maxFaceValue;
	float minFaceValue;
	bool loadPoint(float ** pt, long np);
	bool loadFace(long ** fc, long nf);
	bool loadPointV(float * ptv, long np);
	bool loadFaceV(float * fcv, long nf);
//	bool findLine(long * triangle, float v, float * p1, float * p2);
	// sort triangle vertex in face matrix according to point value
	void sortFaceVertex(void);
	long **faceNext;
	void getFaceNext(void);
	long isBelongToFace(long * edge, long * face, long nv);
	double rsl(double x1, double x2, double y1, double y2, double x);
//	float ** findOneContour(float v, bool *faceOfInterest, long * np);
//	long * faceAtVertex(long pt, long *nfav);
	//edge[][1..2] is two vertex
	long **edge;
	// generate edge list from faceNext matrix
	void genEdge(void);
	long nedge;
	// nface*3 matrix from face to edge
	long **faceEdge;
 //edge[][1..2] is two triangle
	long **edgeFace;
	// next edge list from edgei
//	long * nextEdgeFromEdge(long edgei, long *nne);

//	long * nextEdgeFromPoint(long, long pointi, long *nne);

	std::vector<float> contourpx;
	std::vector<float> contourpy;
	std::vector<float> contourpz;
//	float * getNextPoint(long * edgelist, long nel, float v, long * edgeBelong);
//	void getPointFromOneEdge(long edgei, float v, float * p1, long *p1idx, float * p2, long *p2idx);
//	long * edgeLoopToPointLoop(long * edgelist, long nel);

	std::vector< std::vector<long> > point2face;
	void genPointToFaceMap(void);
	void findContourInOneTriangle(long triangleIndex, float v, float * pin, long * pinLocate, float * pout, long * poutLocate);
	void findContour(float v);

	std::vector< std::vector< std::vector<float> > > contourv;
	std::vector< std::vector< std::vector<long> > > contourv1;
};

