// SARCalibCurve.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "SARCalibCurve.h"
#include "funT\\smsp.h"
#include "funT\\lspfitT.h"
//#include <algorithm>
#include "func.h"

// SARCalibCurve

SARCalibCurve::SARCalibCurve()
{
}

SARCalibCurve::~SARCalibCurve()
{
}


// SARCalibCurve member functions


void SARCalibCurve::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code


		size_t si=0;

		ar<<ll.size();

		for(size_t i=0;i<ll.size();i++){
			ar<<ll[i]
			<<scl[i].AC
				<<scl[i].LC
				<<scl[i].SC;
			for(size_t j=0;j<ll[i];j++){
				ar<<sconc[j+si]
				<<normq[j+si];
			}
			si+=ll[i];
		}

	}
	else
	{	// loading code

		size_t nll;
		double tmp;
		ar>>nll;

		ll.assign(nll,0);
		scl.assign(nll,SCP());

		for(size_t i=0;i<ll.size();i++){
			ar>>ll[i]
			>>scl[i].AC
				>>scl[i].LC
				>>scl[i].SC;
			for(size_t j=0;j<ll[i];j++){
				ar>>tmp;
				sconc.push_back(tmp);
				ar>>tmp;
				normq.push_back(tmp);
			}
		}

	}
}

//bool compless (double c1, double c2) {
//	return (c1>c2);
//}

//calculate coeffiecient of S=c1*A+c0
bool SARCalibCurve::CalSARLine(double evoR, double & c1, double & c0)
{
	size_t si=0;

	std::vector<double> xs;
	std::vector<double> ys;
	
	for(size_t i=0;i<ll.size();i++){
		std::vector<double> x(sconc.begin()+si,sconc.begin()+si+ll[i]);
		std::vector<double> y(normq.begin()+si,normq.begin()+si+ll[i]);

		std::vector<double> c1(4,0);
		std::vector< std::vector<double> > c(x.size()-1,c1);
		smspl(x,y,1.0,c);
		std::vector<double> r;
		int ni=SolveCubicPP(x,c,evoR,r);

		if(ni>0){
			SortInsert(xs,ys,r.back()*scl[i].AC/scl[i].SC,r.back());

			//double x1=r.back()*scl[i].AC/scl[i].SC;
			//

			//	std::vector<double>::iterator it;
			//	it = std::find_first_of (xs.begin(), xs.end(),&x1, (&x1)+1, compless);
			//	size_t idx=it-xs.begin();
			//	xs.insert(it,x1);
			//	ys.insert(ys.begin()+idx,r.back());

		}

		si+=ll[i];
	}


	if(xs.size()>2){
		std::vector<double> c;
		lspfit(xs,ys,2,c);

		c1=c[1];
		c0=c[0];

		return true;

	}
	
	return false;
}


bool SARCalibCurve::GetStandradCalibCurve(std::vector<double> & x, std::vector<double> & y)
{
	size_t si=0;
	
	for(size_t i=0;i<ll.size();i++){
		if( scl[i].AC==0 && scl[i].SC!=0 ){
			x.assign(sconc.begin()+si,sconc.begin()+si+ll[i]);
			y.assign(normq.begin()+si,normq.begin()+si+ll[i]);
			return true;
		}
		si+=ll[i];
	}

	return false;
}
