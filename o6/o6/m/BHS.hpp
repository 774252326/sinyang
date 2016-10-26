#pragma once

#include <vector>
#include <numeric>
#include <functional>
#include "../funT1/slvmatT.h"

//matlab griddata v4 interpolation
class BHS
{
public:
	std::vector< std::vector<double> > point;
	std::vector<double> pointValue;

	std::vector<double> coef;

public:
	BHS(){};
	~BHS(){};

protected:
	static double Distance2(const std::vector<double> &p1, const std::vector<double> &p2)
	{
		std::vector<double> d(p1.begin(),p1.end());
		for(size_t i=0;i<d.size();i++){
			d[i]-=p2[i];
		}
		double x=0;
		x=std::inner_product(d.begin(),d.end(),d.begin(),x,std::plus<double>(),std::multiplies<double>());
		return x;
	};

	static void Distance2Matrix(const std::vector< std::vector<double> > &pt, std::vector< std::vector<double> > &dm)
	{
		dm.clear();
		for(size_t i=0;i<pt.size();i++){
			std::vector<double> r1;
			for(size_t j=0;j<pt.size();j++){
				double tmp=Distance2(pt[i],pt[j]);
				r1.push_back(tmp);
			}
			dm.push_back(r1);
		}
	};


	static void GreenVector(const std::vector< std::vector<double> > &pt, const std::vector<double> &ptx, std::vector<double> &greenVT)
	{
		size_t p=ptx.size();
		greenVT.clear();
		switch(p){
		case 2:	
			for(size_t i=0;i<pt.size();i++){
				double tmp=Distance2(pt[i],ptx);
				if(tmp!=0)
					tmp=tmp*(log(tmp)/2-1);
				greenVT.push_back(tmp);
			}
			break;
		case 4:
			for(size_t i=0;i<pt.size();i++){
				double tmp=Distance2(pt[i],ptx);
				tmp=log(tmp)/2;
				greenVT.push_back(tmp);
			}
			break;
		default:
			{
				double ind=2-((double)p)/2;
				for(size_t i=0;i<pt.size();i++){
					double tmp=Distance2(pt[i],ptx);
					tmp=pow(tmp,ind);
					greenVT.push_back(tmp);
				}
			}
			break;
		}
	};

	static void GreenMatrix(const std::vector< std::vector<double> > &pt, std::vector< std::vector<double> > &dm)
	{
		dm.clear();
		for(size_t i=0;i<pt.size();i++){
			std::vector<double> gv;
			GreenVector(pt,pt[i],gv);
			dm.push_back(gv);
			//TRACE("\n%d",i);
		}

	};
public:
	void CalCoef()
	{
		std::vector< std::vector<double> > dm;
		GreenMatrix(point,dm);

		coef.assign(pointValue.begin(),pointValue.end());

		size_t r=dm.size();
		double **phi=matrix<double>(1,r,1,r);

		for(size_t i=0;i<dm.size();i++){
			for(size_t j=0;j<dm[i].size();j++){
				phi[i+1][j+1]=dm[i][j];
			}
		}

		slvmat(phi,r,coef.data()-1);

		free_matrix(phi,1,r,1,r);
	};


	double BHSInt(const std::vector<double> &ptx)
	{
		std::vector<double> gvx;
		GreenVector(point,ptx,gvx);
		double x=0;
		return std::inner_product(coef.begin(),coef.end(),gvx.begin(),x);
	};	
};