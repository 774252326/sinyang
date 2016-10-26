#pragma once

#include <numeric>
#include "Mesh2D.hpp"


class SurfaceMesh : public Mesh2D
{
public:
	//std::vector<Point2D> point;
	//std::vector< std::vector<size_t> > triangle;
	std::vector< std::vector<size_t> > point2triangle;
	
public:
	SurfaceMesh(void){};

	~SurfaceMesh(void){};

	void Clear()
	{
		point.clear();
		triangle.clear();
		point2triangle.clear();
	};

	void SortPointInTriangle()
	{
		for(size_t i=0;i<point2triangle.size();i++){
			std::sort(point2triangle[i].begin(),point2triangle[i].end());
		}
	};

	void GenPointToFaceMap()
	{
		std::vector<size_t> tline;
		point2triangle.assign(point.size(),tline);
		for(size_t i=0;i<triangle.size();i++){
			std::sort(triangle[i].begin(),triangle[i].end());
			for(size_t j=0;j<triangle[i].size();j++){
				point2triangle[triangle[i][j]].push_back(i);
			}
		}

		SortPointInTriangle();
	};

	void FindEdge(std::vector<size_t> &edgeP0, std::vector<size_t> &edgeP1, std::vector<size_t> &ti)
	{
		for(size_t i=0;i<triangle.size();i++){

			std::vector<size_t> adjectTriangle(2);
			std::vector<size_t>::iterator it;
			it=std::set_intersection(
				point2triangle[triangle[i][0]].begin(),
				point2triangle[triangle[i][0]].end(),
				point2triangle[triangle[i][2]].begin(),
				point2triangle[triangle[i][2]].end(),
				adjectTriangle.begin());
			if(it!=adjectTriangle.end()){
				edgeP0.push_back(triangle[i][0]);
				edgeP1.push_back(triangle[i][2]);
				ti.push_back(i);
			}

			it=std::set_intersection(
				point2triangle[triangle[i][0]].begin(),
				point2triangle[triangle[i][0]].end(),
				point2triangle[triangle[i][1]].begin(),
				point2triangle[triangle[i][1]].end(),
				adjectTriangle.begin());
			if(it!=adjectTriangle.end()){
				edgeP0.push_back(triangle[i][0]);
				edgeP1.push_back(triangle[i][1]);
				ti.push_back(i);
			}

			it=std::set_intersection(
				point2triangle[triangle[i][1]].begin(),
				point2triangle[triangle[i][1]].end(),
				point2triangle[triangle[i][2]].begin(),
				point2triangle[triangle[i][2]].end(),
				adjectTriangle.begin());
			if(it!=adjectTriangle.end()){
				edgeP0.push_back(triangle[i][1]);
				edgeP1.push_back(triangle[i][2]);
				ti.push_back(i);
			}
		}
	};



};
