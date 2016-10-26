#pragma once

#include "SurfaceMesh.hpp"

class SurfaceMeshA : public SurfaceMesh
{
public:
	std::vector<double> pointValue;

	std::vector< std::vector<Point2D> > contourp;
	std::vector<double> contourValue;

public:
	SurfaceMeshA(void){};

	~SurfaceMeshA(void){};

	void Clear()
	{
		SurfaceMesh::Clear();
		pointValue.clear();
	};

	void ClearContour()
	{
		contourp.clear();
		contourValue.clear();
	};

	bool CheckContourInOneTriangle(size_t triangleIndex, double v, size_t &pinLocate0, size_t &pinLocate1, size_t &poutLocate0, size_t &poutLocate1)
	{
		size_t p0=triangle[triangleIndex][0];
		size_t p1=triangle[triangleIndex][1];
		size_t p2=triangle[triangleIndex][2];

		double pv0=pointValue[p0]-v;
		double pv1=pointValue[p1]-v;
		double pv2=pointValue[p2]-v;

		if(pv0==0){
			if(pv1==0){
				if(pv2==0){
					return false;
				}

				pinLocate0=pinLocate1=p0;
				poutLocate0=poutLocate1=p1;
				return true;				
			}

			if(pv2==0){
				pinLocate0=pinLocate1=p0;
				poutLocate0=poutLocate1=p2;
				return true;				
			}

			if(pv1*pv2<0){
				pinLocate0=pinLocate1=p0;
				poutLocate0=p1;
				poutLocate1=p2;
				return true;				
			}
			return false;
		}

		if(pv1==0){
			if(pv2==0){
				pinLocate0=pinLocate1=p1;
				poutLocate0=poutLocate1=p2;
				return true;				
			}

			if(pv0*pv2<0){
				pinLocate0=pinLocate1=p1;
				poutLocate0=p0;
				poutLocate1=p2;
				return true;				
			}
			return false;
		}

		if(pv2==0){
			if(pv0*pv1<0){
				pinLocate0=pinLocate1=p2;
				poutLocate0=p0;
				poutLocate1=p1;
				return true;				
			}
			return false;
		}


		if(pv0*pv1<0){
			pinLocate0=p0;
			pinLocate1=p1;
			if(pv0*pv2<0){
				poutLocate0=p0;
				poutLocate1=p2;
			}
			else{
				poutLocate0=p1;
				poutLocate1=p2;
			}
			return true;
		}

		if(pv0*pv2<0){
			pinLocate0=p0;
			pinLocate1=p2;

			poutLocate0=p1;
			poutLocate1=p2;
			return true;
		}

		return false;
	};

	bool CheckOutPointLocateInOneTriangle(size_t triangleIndex, double v, size_t pinLocate0, size_t pinLocate1, size_t &poutLocate0, size_t &poutLocate1)
	{
		std::vector<size_t> pin;
		pin.push_back(pinLocate0);
		pin.push_back(pinLocate1);
		std::vector<size_t>::iterator it;
		it = std::unique(pin.begin(), pin.end()); 
		pin.resize( std::distance(pin.begin(),it) ); 
		std::sort(pin.begin(), pin.end()); 

		std::vector<size_t> ti(3);	
		it=std::set_difference(triangle[triangleIndex].begin(),triangle[triangleIndex].begin()+3,pin.begin(),pin.end(),ti.begin());
		ti.resize( std::distance(ti.begin(),it) );

		if(ti.size()==2){
			double pvt0=pointValue[ti[0]]-v;
			double pvt1=pointValue[ti[1]]-v;

			if(pvt0==0){
				if(pvt1==0){
					return false;
				}
				poutLocate0=poutLocate1=ti[0];
				return true;
			}
			if(pvt1==0){
				poutLocate0=poutLocate1=ti[1];
				return true;
			}

			if(pvt0*pvt1<0){
				poutLocate0=ti[0];
				poutLocate1=ti[1];
				return true;
			}

			return false;
		}

		if(ti.size()==1){
			double pvt0=pointValue[ti[0]]-v;
			poutLocate0=poutLocate1=ti[0];
			if(pvt0!=0){
				double pinv0=pointValue[pinLocate0]-v;
				if(pvt0*pinv0<0){
					if(pinLocate0>ti[0]){
						poutLocate1=pinLocate0;
					}
					else{
						poutLocate0=pinLocate0;
					}
				}
				else{
					if(pinLocate1>ti[0]){
						poutLocate1=pinLocate1;
					}
					else{
						poutLocate0=pinLocate1;
					}
				}
			}
			return true;
		}
		return false;
	};

	void TraceContour(size_t tiStart, double v, std::vector<size_t> &triangleUncheck, std::vector< std::vector<size_t> > &contourpl)
	{
		bool bLost=true;

		if(contourpl.empty()
			||triangleUncheck.empty())
			return;

		while(bLost){

			size_t pin0=contourpl.back()[0];
			size_t pin1=contourpl.back()[1];
			std::vector<size_t> nextti0;
			nextti0.assign(point2triangle[pin0].begin(),point2triangle[pin0].end());
			std::vector<size_t>::iterator it;

			if(pin0!=pin1){
				it=std::set_intersection(point2triangle[pin0].begin(),point2triangle[pin0].end(),point2triangle[pin1].begin(),point2triangle[pin1].end(),nextti0.begin());
				nextti0.resize( std::distance(nextti0.begin(),it) );
			}

			std::vector<size_t> nextti(nextti0.size());
			it=std::set_intersection(nextti0.begin(),nextti0.end(),triangleUncheck.begin(),triangleUncheck.end(),nextti.begin());
			nextti.resize( std::distance(nextti.begin(),it) );


			std::vector<size_t> pout(2);
			bLost=false;
			for(size_t i=0;i<nextti.size();i++){
				it=std::remove(triangleUncheck.begin(),triangleUncheck.end(),nextti[i]);
				triangleUncheck.resize( std::distance(triangleUncheck.begin(),it) );

				if(CheckOutPointLocateInOneTriangle(nextti[i],v,pin0,pin1,pout[0],pout[1])){
					bLost=true;
					contourpl.push_back(pout);
					tiStart=nextti[i];
					break;
				}

			}
		}

	};

	void FindContour(double v)
	{
		std::vector<size_t> triangleUnchecked;
		for(size_t i=0;i<triangle.size();i++){
			triangleUnchecked.push_back(i);
		}


		while(!triangleUnchecked.empty()){
			std::vector<size_t> pinLocate(2);
			std::vector<size_t> poutLocate(2);
			size_t tinow=triangleUnchecked.back();
			triangleUnchecked.pop_back();
			bool flg=CheckContourInOneTriangle(tinow,v,pinLocate[0],pinLocate[1],poutLocate[0],poutLocate[1]);
			if(flg){
				std::vector< std::vector<size_t> > contourpl;
				contourpl.push_back(pinLocate);
				TraceContour(tinow,v,triangleUnchecked,contourpl);
				if(contourpl.back()[0]!=poutLocate[0] || contourpl.back()[1]!=poutLocate[1]){
					std::reverse(contourpl.begin(),contourpl.end());
					contourpl.push_back(poutLocate);
					TraceContour(tinow,v,triangleUnchecked,contourpl);
				}
				else{
					contourpl.push_back(pinLocate);
				}

				std::vector<Point2D> contourp0(contourpl.size(),Point2D());

				//clock_t t = clock();

				//convert location to coordinate
				for(size_t i=0;i<contourpl.size();i++){
					if(contourpl[i][0]==contourpl[i][1]){
						contourp0[i]=point[contourpl[i][0]];
					}
					else{
						contourp0[i]=Point2D::Interp(point[contourpl[i][0]],point[contourpl[i][1]],pointValue[contourpl[i][0]],pointValue[contourpl[i][1]],v);
					}

				}
				contourp.push_back(contourp0);	
				contourValue.push_back(v);
				//printf( "Elapsed time %d ms.\n", clock() - t );


			}
		}
	};

	void GenContourMap(const std::vector<double> &vl)
	{
		for(size_t i=0;i<vl.size();i++){
			FindContour(vl[i]);
		}
	};

	void GenContourMap(size_t Ncontour, double vmin, double vmax)
	{
		if(vmin==vmax){
			return FindContour(vmin);
		}

		if(Ncontour<=2){
			FindContour(vmin);
			FindContour(vmax);
			return;
		}

		std::vector<double> vl(Ncontour,(vmax-vmin)/(Ncontour-1));
		vl.front()=vmin;
		std::vector<double> revl(Ncontour);
		std::partial_sum(vl.begin(),vl.end(),revl.begin());
		GenContourMap(revl);

	};

	void GenContourMap(size_t Ncontour=10)
	{
		auto re=std::minmax_element(pointValue.begin(),pointValue.end());
		GenContourMap(Ncontour,(*re.first),(*re.second));
	};

};
