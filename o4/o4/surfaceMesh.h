#pragma once

//#include <boost/numeric/ublas/vector.hpp>
//#include <boost/numeric/ublas/matrix.hpp>
//#include <boost/numeric/ublas/io.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

//#include "KDbHeader.h"

class surfaceMesh
{
public:
	surfaceMesh(void);
	~surfaceMesh(void);

	//boost::numeric::ublas::matrix<double> point;
	//boost::numeric::ublas::matrix<long> triangle;
	std::vector< std::vector<double> > point;
	std::vector< std::vector<long> > triangle;

	//boost::numeric::ublas::vector<double> pointValue;
	//boost::numeric::ublas::vector<double> triangleValue;
	std::vector<double> pointValue;
	std::vector<double> triangleValue;


	//boost::numeric::ublas::matrix<long> edge;
	//boost::numeric::ublas::matrix<long> edgeTriangle;

	//boost::numeric::ublas::matrix<double> triangleCentroid;
	//boost::numeric::ublas::matrix<long> triangleEdge;	
	//boost::numeric::ublas::matrix<long> triangleNext;

	std::vector< std::vector<double> > triangleCentroid;
	std::vector< std::vector<long> > triangleEdge;	
	std::vector< std::vector<long> > triangleNext;



	std::vector< std::vector<long> > point2triangle;
	std::vector< std::vector<long> > edge;
	std::vector< std::vector<long> > edgeTriangle;



	std::vector< std::vector< std::vector<double> > > contourv;
	std::vector<double> contourValue;

	double maxPointValue;
	double minPointValue;
	double maxTriangleValue;
	double minTriangleValue;

	std::vector< std::vector<long> > border;
	std::vector< std::vector<long> > borderEdge;


	bool loadPoint(double ** pt, long np);
	bool loadFace(long ** fc, long nf);

	void genEdge();
	void genPointToFaceMap(void);
	void genFaceCentroid(void);

	const size_t tri;
public:
	long findEdge(std::vector<long> cedge);
	// indicate whether surface is close or open
	bool isClose;

	//std::vector< std::vector<double> > findContourInOneTriangle(long triangleIndex, double v, long * pinLocate, long * poutLocate);
	void findContourInOneTriangle(long triangleIndex, double v, long * pinLocate, long * poutLocate);
	void findContour(double v);

	double rsl(double x1, double x2, double y1, double y2, double x);
	long findOutPointLocateInOneTriangle(long triangleIndex, double v, long pinLocate);
	std::vector<double> calPointAtLocate(long pLocate, double v);
	void calPointAtLocate(long pLocate, double v, double *pp);
	void genContourMap(const long contourNumber, double cvmin, double cvmax);
	void genContourMap(const long contourNumber);
	bool loadPointAndFace(double ** pt, long np, long ** fc, long nf);
	bool loadPointV(double * ptv, long np);
	bool loadFaceV(double * fcv, long nf);


	long findEdgeFromList(std::vector<long> cedge, std::vector< std::vector<long> > elist);
	//double *pv;
	void interpPointV(int choice=0);
	void clear(void);


	// generate contour map from vector x[0..nd-1]
	void genContourMapA(double * x, long nd);
	void clearContour(void);

	//void loadKWaferLevelContourNodeVector( std::vector<KWaferLevelContourNode> m_dataAry );
	void loadKWaferLevelContourNodeVector( double** m_dataAry, long nf, int flag=0 );

	void calPointValueRange();
	void calTriangleValueRange();

	void genBorder(std::vector< std::vector<long> > remainEdge);

};

