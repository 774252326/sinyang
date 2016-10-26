#pragma once
#include <vector>
#include <algorithm>
#include "Point2D.hpp"


class Mesh2D
{
public:	
	std::vector<Point2D> point;
	std::vector< std::vector<size_t> > triangle;
	Mesh2D(void){};
	~Mesh2D(void){};

protected:
	void Init()
	{	
		std::vector<Point2D> pinit;
		pinit.assign(4,point.front());

		for(size_t i=0;i<point.size();i++){
			if(point[i].x[0]<pinit[0].x[0]){
				pinit[0].x[0]=point[i].x[0];
				pinit[1].x[0]=point[i].x[0];
			}
			if(point[i].x[0]>pinit[2].x[0]){
				pinit[2].x[0]=point[i].x[0];
				pinit[3].x[0]=point[i].x[0];
			}

			if(point[i].x[1]<pinit[0].x[1]){
				pinit[0].x[1]=point[i].x[1];
				pinit[2].x[1]=point[i].x[1];
			}

			if(point[i].x[1]>pinit[1].x[1]){
				pinit[1].x[1]=point[i].x[1];
				pinit[3].x[1]=point[i].x[1];
			}
		}

		Point2D dd=pinit[3]-pinit[0];
		pinit[0]=pinit[0]-dd;
		pinit[3]=pinit[3]+dd;

		dd=pinit[2]-pinit[1];
		pinit[1]=pinit[1]-dd;
		pinit[2]=pinit[2]+dd;

		point.resize(point.size()+pinit.size());
		std::copy_backward(pinit.begin(),pinit.end(),point.end());

		triangle.clear();
		std::vector<size_t> t0(3,point.size()-1);
		t0[0]-=2;
		t0[1]-=1;
		triangle.push_back(t0);

		t0[2]-=3;
		triangle.push_back(t0);

	};

	bool OutCenter(size_t indexT, Point2D &pc, double &r2)
	{
		return Point2D::OutCenter(point[triangle[indexT][0]],point[triangle[indexT][1]],point[triangle[indexT][2]],pc,r2);
	};

	bool FindTriangle(size_t indexP, std::vector<size_t> &indexTIn)
	{
		size_t sz=indexTIn.size();
		for(size_t i=0;i<triangle.size();i++){
			Point2D pc;
			double r2;
			bool flg=OutCenter(i,pc,r2);
			if(flg){
				double d2=Point2D::Dist2(pc,point[indexP]);
				if(d2<r2){
					indexTIn.push_back(i);
				}
			}
		}
		return (indexTIn.size()>sz);

	};


	bool Merge(const std::vector<size_t> &borderNew, std::vector<size_t> &border)
	{
		std::vector<size_t> bn(borderNew.begin(),borderNew.end());
		size_t nsz=borderNew.size();
		bn.resize(2*nsz);
		std::copy_backward(borderNew.begin(),borderNew.end(),bn.end());

		border.push_back(border.front());
		while(bn.size()>nsz){
			std::vector<size_t> edge(bn.begin(),bn.begin()+2);
			std::vector<size_t>::iterator it;
			it=std::search(border.begin(),border.end(),edge.begin(),edge.end());
			if(it!=border.end()){
				border.pop_back();
				it++;
				std::rotate(border.begin(),it,border.end());
				border.resize(border.size()+nsz-2);
				std::copy_backward(bn.begin()+2,bn.begin()+nsz,border.end());
				std::reverse(border.end()-nsz+2,border.end());
				return true;
			}
			bn.erase(bn.begin());
		}
		border.pop_back();
		return false;
	};


	bool Merge2(const std::vector<size_t> &borderNew, std::vector<size_t> &border)
	{
		bool flg=Merge(borderNew,border);
		if(flg)
			return true;
		std::reverse(border.begin(),border.end());
		return Merge(borderNew,border);
	};

	void Merge3(std::vector< std::vector<size_t> > &borderlist, std::vector<size_t> &borderadd)
	{	
		bool flg=false;
		for(size_t i=0;i<borderlist.size();i++){
			flg=Merge2(borderadd,borderlist[i]);
			if(flg){
				borderadd.assign(borderlist[i].begin(),borderlist[i].end());
				borderlist.erase(borderlist.begin()+i);
				return Merge3(borderlist,borderadd);
			}
		}
		if(!flg){
			borderlist.push_back(borderadd);
		}

	};

	bool MergePolygon(const std::vector<size_t> &indexTIn, std::vector<size_t> &indexPBorder)
	{

		std::vector< std::vector<size_t> > border;

		for(size_t i=0;i<indexTIn.size();i++){
			std::vector<size_t> t0(triangle[indexTIn[i]].begin(),triangle[indexTIn[i]].end());

			Merge3(border,t0);
		}

		if(border.size()!=1){
			return false;
		}
		indexPBorder.assign(border.front().begin(),border.front().end());
		return true;
	};


	bool AddPoint(size_t indexP)
	{
		std::vector<size_t> tin;
		if(FindTriangle(indexP,tin)){
			std::vector<size_t> bdin;
			if(MergePolygon(tin,bdin)){

				std::sort(tin.begin(),tin.end());

				while(!tin.empty()){
					triangle.erase(triangle.begin()+tin.back());
					tin.pop_back();
				}

				std::vector<size_t> t0(3,indexP);
				bdin.push_back(bdin.front());
				for(size_t i=0;i<bdin.size()-1;i++){
					t0[1]=bdin[i];
					t0[2]=bdin[i+1];
					triangle.push_back(t0);
				}
				return true;
			}
		}
		return false;

	};
public:
	bool Delaunay2DMesh()
	{
		if(point.size()<3)
			return false;
		Init();
		for(size_t i=0;i<point.size()-4;i++){
			TRACE("\n%d",i);
			bool flg=AddPoint(i);
			if(!flg){
				return false;
			}
		}
		size_t j=0;
		while(j<triangle.size()){
			if(	triangle[j][0]>=point.size()-4 || 
				triangle[j][1]>=point.size()-4 ||
				triangle[j][2]>=point.size()-4 ){
					triangle.erase(triangle.begin()+j);
			}
			else{
				j++;
			}
		}
		point.erase(point.end()-4,point.end());
		return true;
	};


};

