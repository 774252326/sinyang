//#include "StdAfx.h"
//#include "../../TSVBase/header/db/surfaceMesh.h"
////#include "contour\funT\nrutilT.h"
////#include "contour\funT\bhsintT.h"
////#include "../../funT/paddingT.h"
//
//#include "contour\Eigen\Dense"


#include "StdAfx.h"
#include "surfaceMesh.h"
#include "../../funT/nrutilT.h"
#include "../../funT/bhsintT.h"
//#include "../../funT/paddingT.h"

#include "../../Eigen/Dense"


surfaceMesh::surfaceMesh(void)
	: tri(3)
	, isClose(false)
	//, pv(NULL)
	, maxPointValue(0)
	, minPointValue(0)
	, maxTriangleValue(0)
	, minTriangleValue(0)
{
}


surfaceMesh::~surfaceMesh(void)
{
	clear();
}


bool surfaceMesh::loadPoint(double ** pt, long np)
{
	long i;
	if(np>0){

		point.clear();
		std::vector<double> onepoint;
		point.push_back(onepoint);

		for(i=1;i<=np;i++){
			onepoint.assign(&pt[i][1],&pt[i][1+tri]);
			point.push_back(onepoint);			
		}

		//point.resize(np+1,tri);
		pointValue.resize(np+1);	

		//std::vector<long> line;

		//for(i=0;i<=np;i++){
			//point2triangle.push_back(line);
			
		//}



		return true;
	}
	else{
		return false;
	}
}

bool surfaceMesh::loadPointV(double * ptv, long np)
{
	if(np==(pointValue.size()-1)){

		pointValue.assign(&ptv[0],&ptv[pointValue.size()]);

		calPointValueRange();
		return true;
	}
	else{
		return false;
	}
}


bool surfaceMesh::loadFace(long ** fc, long nf)
{
	long i;
	if(nf>0){
		triangle.clear();
		std::vector<long> onetriangle;
		triangle.push_back(onetriangle);
		//triangleEdge.push_back(onetriangle);

		for(i=1;i<=nf;i++){
			onetriangle.assign(&fc[i][1],&fc[i][1+tri]);
			triangle.push_back(onetriangle);
			//onetriangle.clear();
			//triangleEdge.push_back(onetriangle);
		}

		triangleValue.resize(nf+1);
		//triangleNext.resize(nf+1);

		return true;
	}
	else{
		return false;
	}

}


bool surfaceMesh::loadFaceV(double * fcv, long nf)
{
	if (nf==(triangleValue.size()-1) ){

		triangleValue.assign(&fcv[0],&fcv[triangleValue.size()]);
		calTriangleValueRange();
		return true;
	}
	else{
		return false;
	}
}

// generate edge list and face to edge list from faceNext matrix
void surfaceMesh::genEdge()
{
	//isvolume=1, if input a volumn grid;
	//isvolumn=0, if input a planar grid;

	edge.clear();
	std::vector<long> currentedge(3,0);
	edge.push_back(currentedge);

	std::vector< std::vector<long> > remaine;
	//std::vector< std::vector<long> >::iterator rit;
	long nedge=0;

	std::vector<long> newedge;
	edgeTriangle.push_back(newedge);

	std::vector<long> onetriangle;

	long i,j,jn,k;
	long edgei;
	
	
	edgei=0;
	triangleEdge.push_back(onetriangle);

	for(i=1;i<triangle.size();i++){
		triangleEdge.push_back(onetriangle);

		for(j=0;j<triangle[i].size();j++){

			currentedge.clear();
			jn=( (j==triangle[i].size()-1)? 0:j+1 );
			currentedge.push_back(triangle[i][j]);
			currentedge.push_back(triangle[i][jn]);

			//edgei=findEdge(currentedge);

			edgei=0;


			//for(rit=remaine.begin();rit!=remaine.end();++rit){
			//	if( ( ((*rit)[0]==currentedge[0]) && ((*rit)[1]==currentedge[1]) )||( ((*rit)[0]==currentedge[1]) && ((*rit)[1]==currentedge[0]) ) ){
			//		edgei=(*rit)[2];
			//		remaine.erase(rit);
			//		break;
			//	}
			//}


			for(k=remaine.size()-1;k>=0;k--){
				if(	(remaine[k][0]==currentedge[0]) && (remaine[k][1]==currentedge[1]) ){
					edgei=remaine[k][2];
					remaine.erase(remaine.begin()+k);
					break;
				}
				if(	(remaine[k][0]==currentedge[1]) && (remaine[k][1]==currentedge[0]) ){
					edgei=remaine[k][2];
					remaine.erase(remaine.begin()+k);
					break;
				}
			}


			if(edgei==0){
				edgei=edge.size();
				edge.push_back(currentedge);
				currentedge.push_back(edgei);
				remaine.push_back(currentedge);

				//triangleEdge[i][j]=edgei;
				triangleEdge[i].push_back(edgei);
				edgeTriangle.push_back(newedge);
				edgeTriangle[edgei].push_back(i);
			}
			else{
				//triangleEdge[i][j]=edgei;
				triangleEdge[i].push_back(edgei);
				edgeTriangle[edgei].push_back(i);
			}





		}
	}


	isClose=true;
	for(i=1;i<edgeTriangle.size();i++){
		if(edgeTriangle[i].size()<2){
			isClose=false;
			break;
		}
	}


}


long surfaceMesh::findEdge(std::vector<long> cedge)
{
	long edgei;
	for(edgei=edge.size()-1;edgei>0;edgei--){
		if(	(edge[edgei][0]==cedge[0]) && (edge[edgei][1]==cedge[1]) )
			return edgei;
		if(	(edge[edgei][0]==cedge[1]) && (edge[edgei][1]==cedge[0]) )
			return edgei;
	}
	return 0;
}

void surfaceMesh::genPointToFaceMap(void)
{
	long i,j;

			std::vector<long> line;

		for(i=0;i<point.size();i++){
			point2triangle.push_back(line);			
		}

	for(i=1;i<triangle.size();i++){
		for(j=0;j<triangle[i].size();j++){
			point2triangle[triangle[i][j]].push_back(i);
		}
	}

}


//std::vector< std::vector<double> > surfaceMesh::findContourInOneTriangle(long triangleIndex, double v, long * pinLocate, long * poutLocate)
//{
//
//	//std::vector<long> ptriangle;
//	//ptriangle=triangle[triangleIndex];
//	//std::vector<long> etriangle;
//	//etriangle=triangleEdge[triangleIndex];
//
//	long *ptriangle;
//	ptriangle=triangle[triangleIndex].data();
//	long *etriangle;
//	etriangle=triangleEdge[triangleIndex].data();
//	//double *pv;
//	//pv=pointValue.data();
//
//
//
//
//	double e1,e2,e3,e12,e23,e31;
//	long i;
//
//	std::vector<double> pin;
//	std::vector<double> pout;
//
//	std::vector< std::vector<double> > pinout;
//
//	e1=pv[ptriangle[0]]-v;
//	e2=pv[ptriangle[1]]-v;
//	e3=pv[ptriangle[2]]-v;
//
//	e12=e1*e2;
//	e23=e2*e3;
//	e31=e3*e1;
//
//	pinLocate[0]=0;
//	poutLocate[0]=0;
//
//	if(e1==0){
//		if(e2==0){
//			if(e3==0){
//			}
//			else{
//				//copyvt(point[p[1]],dim,pin);				
//				pinout.push_back(point[ptriangle[0]]);
//				pinLocate[0]=-ptriangle[0];
//
//				//copyvt(point[p[2]],dim,pout);
//				pinout.push_back(point[ptriangle[1]]);
//				poutLocate[0]=-ptriangle[1];
//			}
//		}
//		else{
//			if(e3==0){
//				//copyvt(point[p[1]],dim,pin);
//				//pinLocate[0]=-p[1];
//				//copyvt(point[p[dim]],dim,pout);
//				//poutLocate[0]=-p[dim];
//
//				pinout.push_back(point[ptriangle[0]]);
//				pinLocate[0]=-ptriangle[0];
//				pinout.push_back(point[ptriangle[2]]);
//				poutLocate[0]=-ptriangle[2];
//
//
//			}
//			else{
//				if(e23<0){
//					//copyvt(point[p[1]],dim,pin);
//					//pinLocate[0]=-p[1];
//					pinout.push_back(point[ptriangle[0]]);
//					pinLocate[0]=-ptriangle[0];
//
//					for(i=0;i<tri;i++){
//						pout.push_back(rsl(pv[ptriangle[1]],pv[ptriangle[2]],point[ptriangle[1]][i],point[ptriangle[2]][i],v));
//					}
//					pinout.push_back(pout);
//					pout.clear();
//					poutLocate[0]=etriangle[1];
//				}
//			}
//		}
//
//	}
//	else{
//		if(e2==0){
//			if(e3==0){
//				//copyvt(point[p[2]],dim,pin);
//				//pinLocate[0]=-p[2];
//				//copyvt(point[p[dim]],dim,pout);
//				//poutLocate[0]=-p[dim];
//
//				pinout.push_back(point[ptriangle[1]]);
//				pinLocate[0]=-ptriangle[1];
//				pinout.push_back(point[ptriangle[2]]);
//				poutLocate[0]=-ptriangle[2];
//
//			}
//			else{
//				if(e31<0){
//					//copyvt(point[p[2]],dim,pin);
//					//pinLocate[0]=-p[2];
//
//					pinout.push_back(point[ptriangle[1]]);
//					pinLocate[0]=-ptriangle[1];
//
//					//for(i=1;i<=dim;i++){
//					//pout[i]=rsl(pointV[p[1]],pointV[p[dim]],point[p[1]][i],point[p[dim]][i],v);
//					//}
//					//poutLocate[0]=e[dim];
//
//					for(i=0;i<tri;i++){
//						pout.push_back(rsl(pv[ptriangle[0]],pv[ptriangle[2]],point[ptriangle[0]][i],point[ptriangle[2]][i],v));
//					}
//					pinout.push_back(pout);
//					pout.clear();
//					poutLocate[0]=etriangle[2];
//
//
//				}
//			}
//		}
//		else{
//			if(e3==0){
//				if(e12<0){
//					//copyvt(point[p[dim]],dim,pin);
//					//pinLocate[0]=-p[dim];
//
//					pinout.push_back(point[ptriangle[2]]);
//					pinLocate[0]=-ptriangle[2];
//
//
//					//for(i=1;i<=dim;i++){
//					//	pout[i]=rsl(pointV[p[1]],pointV[p[2]],point[p[1]][i],point[p[2]][i],v);
//					//}
//					//poutLocate[0]=e[1];
//
//					for(i=0;i<tri;i++){
//						pout.push_back(rsl(pv[ptriangle[0]],pv[ptriangle[1]],point[ptriangle[0]][i],point[ptriangle[1]][i],v));
//					}
//					pinout.push_back(pout);
//					pout.clear();
//					poutLocate[0]=etriangle[0];
//
//				}
//			}
//			else{
//				if(e12<0){
//					if(pinLocate[0]==0){
//						//for(i=1;i<=dim;i++){
//						//pin[i]=rsl(pointV[p[1]],pointV[p[2]],point[p[1]][i],point[p[2]][i],v);
//						//}
//						//pinLocate[0]=e[1];
//
//						for(i=0;i<tri;i++){
//							pin.push_back(rsl(pv[ptriangle[0]],pv[ptriangle[1]],point[ptriangle[0]][i],point[ptriangle[1]][i],v));
//						}
//						pinLocate[0]=etriangle[0];
//
//
//					}
//					else{
//						//for(i=1;i<=dim;i++){
//						//	pout[i]=rsl(pointV[p[1]],pointV[p[2]],point[p[1]][i],point[p[2]][i],v);
//						//}
//						//poutLocate[0]=e[1];
//
//
//						for(i=0;i<tri;i++){
//							pout.push_back(rsl(pv[ptriangle[0]],pv[ptriangle[1]],point[ptriangle[0]][i],point[ptriangle[1]][i],v));
//						}
//						poutLocate[0]=etriangle[0];
//
//					}					
//				}
//
//				if(e23<0){
//					if(pinLocate[0]==0){
//						//for(i=1;i<=3;i++){
//						//	pin[i]=rsl(pointV[p[3]],pointV[p[2]],point[p[3]][i],point[p[2]][i],v);
//						//}
//						//pinLocate[0]=e[2];
//
//						for(i=0;i<tri;i++){
//							pin.push_back(rsl(pv[ptriangle[2]],pv[ptriangle[1]],point[ptriangle[2]][i],point[ptriangle[1]][i],v));
//						}
//						pinLocate[0]=etriangle[1];
//
//					}
//					else{
//						//for(i=1;i<=3;i++){
//						//	pout[i]=rsl(pointV[p[3]],pointV[p[2]],point[p[3]][i],point[p[2]][i],v);
//						//}
//						//poutLocate[0]=e[2];
//
//						for(i=0;i<tri;i++){
//							pout.push_back(rsl(pv[ptriangle[2]],pv[ptriangle[1]],point[ptriangle[2]][i],point[ptriangle[1]][i],v));
//						}
//						poutLocate[0]=etriangle[2];
//					}					
//				}
//
//				if(e31<0){
//					if(pinLocate[0]==0){
//						//for(i=1;i<=3;i++){
//						//	pin[i]=rsl(pointV[p[1]],pointV[p[3]],point[p[1]][i],point[p[3]][i],v);
//						//}
//						//pinLocate[0]=e[3];
//
//						for(i=0;i<tri;i++){
//							pin.push_back(rsl(pv[ptriangle[0]],pv[ptriangle[2]],point[ptriangle[0]][i],point[ptriangle[2]][i],v));
//						}
//						pinLocate[0]=etriangle[2];
//
//					}
//					else{
//						//for(i=1;i<=3;i++){
//						//	pout[i]=rsl(pointV[p[1]],pointV[p[3]],point[p[1]][i],point[p[3]][i],v);
//						//}
//						//poutLocate[0]=e[3];
//
//
//						for(i=0;i<tri;i++){
//							pout.push_back(rsl(pv[ptriangle[0]],pv[ptriangle[2]],point[ptriangle[0]][i],point[ptriangle[2]][i],v));
//						}
//						poutLocate[0]=etriangle[2];
//
//					}					
//				}
//
//				pinout.push_back(pin);
//				pinout.push_back(pout);
//
//			}
//		}
//	}
//
//	return pinout;
//}


void surfaceMesh::findContourInOneTriangle(long triangleIndex, double v, long * pinLocate, long * poutLocate)
{

	//std::vector<long> ptriangle;
	//ptriangle=triangle[triangleIndex];
	//std::vector<long> etriangle;
	//etriangle=triangleEdge[triangleIndex];

	long *ptriangle;
	ptriangle=triangle[triangleIndex].data();
	long *etriangle;
	etriangle=triangleEdge[triangleIndex].data();
	double *pv;
	pv=pointValue.data();




	double e1,e2,e3,e12,e23,e31;


	e1=pv[ptriangle[0]]-v;
	e2=pv[ptriangle[1]]-v;
	e3=pv[ptriangle[2]]-v;

	e12=e1*e2;
	e23=e2*e3;
	e31=e3*e1;

	pinLocate[0]=0;
	poutLocate[0]=0;

	if(e1==0){
		if(e2==0){
			if(e3==0){
			}
			else{
				pinLocate[0]=-ptriangle[0];
				poutLocate[0]=-ptriangle[1];
			}
		}
		else{
			if(e3==0){
				pinLocate[0]=-ptriangle[0];
				poutLocate[0]=-ptriangle[2];
			}
			else{
				if(e23<0){
					pinLocate[0]=-ptriangle[0];
					poutLocate[0]=etriangle[1];
				}
			}
		}

	}
	else{
		if(e2==0){
			if(e3==0){
				pinLocate[0]=-ptriangle[1];
				poutLocate[0]=-ptriangle[2];
			}
			else{
				if(e31<0){
					pinLocate[0]=-ptriangle[1];
					poutLocate[0]=etriangle[2];
				}
			}
		}
		else{
			if(e3==0){
				if(e12<0){
					pinLocate[0]=-ptriangle[2];
					poutLocate[0]=etriangle[0];
				}
			}
			else{
				if(e12<0){
					//if(pinLocate[0]==0){
					pinLocate[0]=etriangle[0];
					//}
					//else{
					//poutLocate[0]=etriangle[0];
					//}					
				}

				if(e23<0){
					if(pinLocate[0]==0){
						pinLocate[0]=etriangle[1];
					}
					else{
						poutLocate[0]=etriangle[1];
						return;
					}
				}

				if(e31<0){
					//if(pinLocate[0]==0){
					//pinLocate[0]=etriangle[2];
					//}
					//else{
					poutLocate[0]=etriangle[2];
					//}					
				}

			}
		}
	}


}



double surfaceMesh::rsl(double x1, double x2, double y1, double y2, double x)
{
	double dx=x2-x1;

	if(dx==0)
		return 0;
	else
		return (x*(y2-y1)+x2*y1-x1*y2)/dx;
}


void surfaceMesh::findContour(double v)
{
	long i,j;

	long pinLocate,poutLocate;

	long currentPlocate=0,startLocate;

	std::vector< std::vector<double> > contourp;

	//contour point location
	std::vector<long> contourpl;
	//vector of contour point location
	std::vector< std::vector<long> > contourplv;

	std::vector<long> nextfacelist;

	std::vector<bool> triangleUnchecked;
	triangleUnchecked.assign(triangle.size(),1);


	bool lost;

	for(i=1;i<triangle.size();i++){
		if(triangleUnchecked[i]){
			findContourInOneTriangle(i,v,&pinLocate,&poutLocate);
			triangleUnchecked[i]=0;
			if( (pinLocate!=0) && (poutLocate!=0) ){
				contourpl.clear();
				contourpl.push_back(pinLocate);
				contourpl.push_back(poutLocate);
				startLocate=pinLocate;
				lost=false;
				while( (poutLocate!=0) && (poutLocate!=startLocate) && !lost ){				
					currentPlocate=poutLocate;
					nextfacelist.clear();
					if(poutLocate>0){
						nextfacelist=edgeTriangle[poutLocate];
					}
					else{
						nextfacelist=point2triangle[-poutLocate];
					}
					lost=true;//if all next face are checked, poutlocate will not update.
					for(j=0;j<nextfacelist.size();j++){
						if(triangleUnchecked[nextfacelist[j]]){
							poutLocate=findOutPointLocateInOneTriangle(nextfacelist[j],v,currentPlocate);
							triangleUnchecked[nextfacelist[j]]=0;
							if(poutLocate!=0){
								contourpl.push_back(poutLocate);
								lost=false;
								break;
							}
						}
					}



				}

				//here we find a new section of contour stored in contourpl

				if( contourpl.front()!=contourpl.back() ){// new section is not a close loop
					std::reverse(contourpl.begin(),contourpl.end());//reverse direction and search again

					startLocate=contourpl.front();
					poutLocate=contourpl.back();
					lost=false;
					while( (poutLocate!=0) && (poutLocate!=startLocate) && !lost ){				
						currentPlocate=poutLocate;
						nextfacelist.clear();
						if(poutLocate>0){
							nextfacelist=edgeTriangle[poutLocate];
						}
						else{
							nextfacelist=point2triangle[-poutLocate];
						}
						lost=true;//if all next face are checked, poutlocate will not update.
						for(j=0;j<nextfacelist.size();j++){
							if(triangleUnchecked[nextfacelist[j]]){
								poutLocate=findOutPointLocateInOneTriangle(nextfacelist[j],v,currentPlocate);
								triangleUnchecked[nextfacelist[j]]=0;
								if(poutLocate!=0){
									contourpl.push_back(poutLocate);
									lost=false;
									break;
								}
							}
						}
					}
				}

				//the whole new section come out
				contourplv.push_back(contourpl);
				contourValue.push_back(v);				

			}


		}
	}


	//clock_t t = clock();

	//convert location to coordinate
	for(i=0;i<contourplv.size();i++){
		contourp.clear();
		for(j=0;j<contourplv[i].size();j++){
			contourp.push_back(calPointAtLocate(contourplv[i][j],v));
		}
		contourv.push_back(contourp);

		//std::cout<<v<<'\n';
	}
	//printf( "Elapsed time %d ms.\n", clock() - t );


}

long surfaceMesh::findOutPointLocateInOneTriangle(long triangleIndex, double v, long pinLocate)
{


	//std::vector<long> tp;
	//tp=triangle[triangleIndex];

	//std::vector<long> te;
	//te=triangleEdge[triangleIndex];
	double e0,e1,e2;
	//poutLocate[0]=0;

	//std::vector<double> pout;

	long *tp;
	tp=triangle[triangleIndex].data();

	long *te;
	te=triangleEdge[triangleIndex].data();
	double *pv;
	pv=pointValue.data();

	e0=pv[tp[0]]-v;
	e1=pv[tp[1]]-v;
	e2=pv[tp[2]]-v;

	if(pinLocate==te[0]){
		if(e2==0){
			return -tp[2];
		}
		else{
			if( e2*e1<0 ){
				return te[1];
			}
			else{
				if( e2*e0<0 ){
					return te[2];
				}
				else{
					return 0;
				}
			}

		}						
	}

	if(pinLocate==te[1]){
		if(e0==0){
			return -tp[0];
		}
		else{
			if( e0*e1<0 ){
				return te[0];
			}
			else{
				if( e0*e2<0 ){
					return te[2];
				}
				else{
					return 0;
				}
			}

		}						
	}


	if(pinLocate==te[2]){
		if(e1==0){
			return -tp[1];
		}
		else{
			if( e2*e1<0 ){
				return te[1];
			}
			else{
				if( e1*e0<0 ){
					return te[0];
				}
				else{
					return 0;
				}
			}

		}						
	}


	if(pinLocate==-tp[0]){
		if(e2*e1<0){
			return te[1];
		}
		else{
			if( e2==0 && e1!=0 ){
				return -tp[2];
			}
			else{
				if( e2!=0 && e1==0 ){
					return -tp[1];
				}
				else{
					return 0;
				}
			}

		}						
	}

	if(pinLocate==-tp[1]){
		if(e0*e2<0){
			return te[2];
		}
		else{
			if( e0==0 && e2!=0 ){
				return -tp[0];
			}
			else{
				if( e0!=0 && e2==0 ){
					return -tp[2];
				}
				else{
					return 0;
				}
			}

		}						
	}

	if(pinLocate==-tp[2]){
		if(e0*e1<0){
			return te[0];
		}
		else{
			if( e0==0 && e1!=0 ){
				return -tp[0];
			}
			else{
				if( e0!=0 && e1==0 ){
					return -tp[1];
				}
				else{
					return 0;
				}
			}

		}						
	}






}


std::vector<double> surfaceMesh::calPointAtLocate(long pLocate, double v)
{
	std::vector<double> pout;

	if(pLocate>0){
		long *e;
		e=edge[pLocate].data();
		double *p0;
		p0=point[e[0]].data();
		double *p1;
		p1=point[e[1]].data();
		double *pv;
		pv=pointValue.data();

		for(long i=0;i<tri;i++){
			pout.push_back(rsl(pv[e[0]],pv[e[1]],p0[i],p1[i],v));
		}			
	}
	else{
		if(pLocate<0){
			pout=point[-pLocate];
		}
	}
	return pout;

}

void surfaceMesh::calPointAtLocate(long pLocate, double v, double *pp)
{
	//std::vector<double> pout;

	if(pLocate>0){
		long *e;
		e=edge[pLocate].data();
		double *p0;
		p0=point[e[0]].data();
		double *p1;
		p1=point[e[1]].data();
		double *pv;
		pv=pointValue.data();

		for(long i=0;i<tri;i++){
			pp[i]=(rsl(pv[e[0]],pv[e[1]],p0[i],p1[i],v));
		}			
	}
	else{
		if(pLocate<0){
			pp=point[-pLocate].data();
			//pout=point[-pLocate];
		}
	}
	//return pout;

}



void surfaceMesh::genContourMap(const long contourNumber, double cvmin, double cvmax)
{
	//clearContour();

	if(cvmin==cvmax){
		findContour(cvmin);
	}
	else{
		double cvalue;
		double vstep=(cvmax-cvmin)/(double)(contourNumber+1);
		for(cvalue=cvmin;cvalue<=cvmax;cvalue+=vstep){
			findContour(cvalue);
		}
	}

}


void surfaceMesh::genContourMap(const long contourNumber)
{
	//clearContour();

	genContourMap(contourNumber,minPointValue,maxPointValue);

}

void surfaceMesh::genFaceCentroid(void)
{
	double temp;

	long i,j,k;
	std::vector<double> onet;
	triangleCentroid.push_back(onet);

	for(i=1;i<triangle.size();i++){

		onet.clear();
		for(k=0;k<tri;k++){
			temp=0;
			for(j=0;j<triangle[i].size();j++){
				temp+=point[triangle[i][j]][k];
			}
			//triangleCentroid[i][k]=temp/tri;
			onet.push_back(temp/tri);
		}
		triangleCentroid.push_back(onet);

	}

}

bool surfaceMesh::loadPointAndFace(double ** pt, long np, long ** fc, long nf)
{
	if( !loadPoint(pt,np) ){
		return false;
	}
	if( !loadFace(fc,nf) ){
		return false;
	}
	genPointToFaceMap();
	genFaceCentroid();
	//getFaceNext();
	genEdge();
	//std::cout<<edge.size()<<'\n';

	return true;
}

long surfaceMesh::findEdgeFromList(std::vector<long> cedge, std::vector< std::vector<long> > elist)
{
	//std::vector<long>::reverse_iterator rit;

	//for( rit=elist.rbegin();rit!=elist.rend();rit++){
	//	if( (*rit[0]==cegde[0]) && (*rit[1]==cegde[1]) )
	//		return 

	return 0;
}


void surfaceMesh::interpPointV(int choice)
{



	if(choice==4){
		////////////////////////////////v4 interpolate/////////////////////////////
		//bhsint(faceCentroid,nface,2,faceValue,point,npoint,pointValue);

		double **tc=matrix<double>(1,triangle.size()-1,1,tri);
		long i,j;
		for(i=1;i<=triangle.size()-1;i++){
			for(j=0;j<=tri-1;j++){
				tc[i][j+1]=triangleCentroid[i][j];
			}
		}


		double **pot=matrix<double>(1,point.size()-1,1,tri);

		for(i=1;i<=point.size()-1;i++){
			for(j=0;j<=tri-1;j++){
				pot[i][j+1]=point[i][j];
			}
		}

		bhsint(tc,triangle.size()-1,2,triangleValue.data(),pot,point.size()-1,pointValue.data());

		free_matrix(tc,1,triangle.size()-1,1,tri);
		free_matrix(pot,1,point.size()-1,1,tri);


	}

	//////////////////////////////////avg/////////////////////////////////////////
	//long i,j;
	//double sum;
	//for(i=1;i<=npoint;i++){
	//	sum=0;
	//	for(j=1;j<point2face[i].size();j++){
	//		sum+=faceValue[point2face[i][j]];
	//	}
	//	pointValue[i]=sum/(j-1);
	//}
	///////////////////////////////////////weight avg////////////////////////////////////////////////
	//	long i,j,k,facei;
	//double sum;
	//double w,wsum;
	//for(i=1;i<=npoint;i++){
	//	sum=0;
	//	wsum=0;
	//	for(j=1;j<point2face[i].size();j++){
	//		facei=point2face[i][j];
	//		w=0;
	//		for(k=1;k<=3;k++){
	//		w+=pow((point[i][k]-faceCentroid[facei][k]),2);
	//		}
	//		w=pow(w,-1.5f);
	//		sum+=w*faceValue[facei];
	//		wsum+=w;
	//	}
	//	pointValue[i]=sum/wsum;
	//}






	/////////////////////////////////////////////////////////////////////////////

	if(choice==0){
		long i,j,facei,nnb;
		using namespace Eigen;


		MatrixXd X;
		for(i=1;i<point.size();i++){	
			nnb=point2triangle[i].size();
			if(nnb>=3){
				MatrixXd A(nnb,3);
				MatrixXd B(nnb,1);
				for(j=0;j<nnb;j++){
					facei=point2triangle[i][j];
					A(j,0)=triangleCentroid[facei][0];
					A(j,1)=triangleCentroid[facei][1];
					A(j,2)=1;
					B(j,0)=triangleValue[facei];
				}
				if(nnb==3){
					X=A.lu().solve(B);
				}
				else{
					X=((A.transpose())*A).lu().solve((A.transpose())*B);
				}
				pointValue[i]=point[i][0]*X(0)+point[i][1]*X(1)+X(2);
			}
			else{
				if(nnb==2){
					pointValue[i]=0.5*(triangleValue[point2triangle[i][0]]+triangleValue[point2triangle[i][1]]);
				}
				else{
					pointValue[i]=triangleValue[point2triangle[i][0]];

				}
			}

		}

		long tr0,tr1;

		for(i=1;i<point.size();i++){	
			nnb=point2triangle[i].size();
			if(nnb==2){
				tr0=point2triangle[i][0];
				tr1=point2triangle[i][1];

			}
			if(nnb==1){
				tr0=point2triangle[i][0];
				pointValue[i]=3*triangleValue[tr0]-pointValue[triangle[tr0][0]]-pointValue[triangle[tr0][1]]-pointValue[triangle[tr0][2]]+pointValue[i];
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//	long i,j,k,facei,nnb;
	//using namespace Eigen;


	//MatrixXd X;
	//for(i=1;i<point.size();i++){	
	//	nnb=point2triangle[i].size();
	//	if(nnb>=4){
	//		MatrixXd A(nnb,4);
	//		MatrixXd B(nnb,1);
	//		for(j=0;j<nnb;j++){
	//			facei=point2triangle[i][j];
	//			A(j,0)=triangleCentroid[facei][0];
	//			A(j,1)=triangleCentroid[facei][1];
	//			A(j,2)=triangleCentroid[facei][2];
	//			A(j,3)=1;
	//			B(j,0)=triangleValue[facei];
	//		}

	//		
	//		if(nnb==4){
	//			X=A.lu().solve(B);
	//		}
	//		else{
	//			X=((A.transpose())*A).lu().solve((A.transpose())*B);
	//		}
	//		pointValue[i]=point[i][0]*X(0)+point[i][1]*X(1)+point[i][2]*X(2)+X(3);
	//	}
	//	else{
	//		//if(nnb==2){
	//			pointValue[i]=(triangleValue[point2triangle[i][0]]+triangleValue[point2triangle[i][1]]+triangleValue[point2triangle[i][2]])/3.0;
	//		//}
	//		//else{
	//			//pointValue[i]=triangleValue[point2triangle[i][0]];
	//			//pointValue[i]=0;
	//		//}
	//	}

	//}



	///////////////////////////get min and max////////////////////////////////////////
	calPointValueRange();
}


void surfaceMesh::clear(void)
{
	point.clear();
	triangle.clear();


	pointValue.clear();
	triangleValue.clear();



	triangleCentroid.clear();
	triangleEdge.clear();	
	triangleNext.clear();



	point2triangle.clear();
	edge.clear();
	edgeTriangle.clear();


	contourv.clear();
	contourValue.clear();
}


// generate contour map from vector x[0..nd-1]
void surfaceMesh::genContourMapA(double * x, long nd)
{
	for(long i=0;i<nd;i++){
		findContour(x[i]);
	}
}


void surfaceMesh::clearContour(void)
{
	contourv.clear();
	contourValue.clear();
}

//new read data function
//void surfaceMesh::loadKWaferLevelContourNodeVector( std::vector<KWaferLevelContourNode> data )
//{
//
//	long pnowidx;
//	long i,j;
//
//	std::vector<double> pnow(3,0);
//	std::vector<long> tnow(3,0);
//	std::vector<long> pcount;
//
//	clear();
//	triangle.push_back(tnow);
//	triangleValue.push_back(0);
//	point.push_back(pnow);
//	pointValue.push_back(0);
//	pcount.push_back(0);
//
//	for(i=0;i<data.size();i++){
//		triangleValue.push_back(data[i].dVal);
//
//		//first point
//		pnow[0]=data[i].ptVec1.x;
//		pnow[1]=data[i].ptVec1.y;
//		pnow[2]=data[i].ptVec1.z;
//		pnowidx=0;
//		for(j=1;j<point.size();j++){
//			if(pnow[0]==point[j][0])
//				if(pnow[1]==point[j][1])
//					if(pnow[2]==point[j][2]){
//						pnowidx=j;
//						pointValue[j]+=data[i].dVal;
//						pcount[j]+=1;
//						break;
//					}
//
//		}
//		if(pnowidx==0){
//			point.push_back(pnow);
//			pointValue.push_back(data[i].dVal);
//			pcount.push_back(1);
//			pnowidx=point.size()-1;
//		}
//		tnow[0]=pnowidx;
//
//
//		//second point
//		pnow[0]=data[i].ptVec2.x;
//		pnow[1]=data[i].ptVec2.y;
//		pnow[2]=data[i].ptVec2.z;
//		pnowidx=0;
//		for(j=1;j<point.size();j++){
//			if(pnow[0]==point[j][0])
//				if(pnow[1]==point[j][1])
//					if(pnow[2]==point[j][2]){
//						pnowidx=j;
//						pointValue[j]+=data[i].dVal;
//						pcount[j]+=1;
//						break;
//					}
//
//		}
//		if(pnowidx==0){
//			point.push_back(pnow);
//			pointValue.push_back(data[i].dVal);
//			pcount.push_back(1);
//			pnowidx=point.size()-1;
//		}
//		tnow[1]=pnowidx;
//
//
//
//		//third point
//		pnow[0]=data[i].ptVec3.x;
//		pnow[1]=data[i].ptVec3.y;
//		pnow[2]=data[i].ptVec3.z;
//		pnowidx=0;
//		for(j=1;j<point.size();j++){
//			if(pnow[0]==point[j][0])
//				if(pnow[1]==point[j][1])
//					if(pnow[2]==point[j][2]){
//						pnowidx=j;
//						pointValue[j]+=data[i].dVal;
//						pcount[j]+=1;
//						break;
//					}
//
//		}
//		if(pnowidx==0){
//			point.push_back(pnow);
//			pointValue.push_back(data[i].dVal);
//			pcount.push_back(1);
//			pnowidx=point.size()-1;
//		}
//		tnow[2]=pnowidx;
//
//		//triangle
//		triangle.push_back(tnow);
//
//	}
//
//	//interpolate point value using average 
//	for(i=1;i<pointValue.size();i++){
//		pointValue[i]/=(double)pcount[i];
//	}
//
//	calPointValueRange();
//	calTriangleValueRange();
//
//	genPointToFaceMap();
//	genFaceCentroid();
//	genEdge();
//
//
//}


//new read data function
void surfaceMesh::loadKWaferLevelContourNodeVector( double **data, long nd )
{

	long rs=1;
	long cs=1;

	long pnowidx;
	long i,j;

	std::vector<double> pnow(3,0);
	std::vector<long> tnow(3,0);
	std::vector<long> pcount;

	clear();
	triangle.push_back(tnow);
	triangleValue.push_back(0);
	point.push_back(pnow);
	pointValue.push_back(0);
	pcount.push_back(0);

	for(i=rs;i<rs+nd;i++){
		triangleValue.push_back(data[i][cs+9]);

		//first point
		pnow[0]=data[i][cs];
		pnow[1]=data[i][cs+3];
		pnow[2]=data[i][cs+6];
		pnowidx=0;
		for(j=1;j<point.size();j++){
			if(pnow[0]==point[j][0])
				if(pnow[1]==point[j][1])
					if(pnow[2]==point[j][2]){
						pnowidx=j;
						pointValue[j]+=data[i][cs+9];
						pcount[j]+=1;
						break;
					}

		}
		if(pnowidx==0){
			point.push_back(pnow);
			pointValue.push_back(data[i][cs+9]);
			pcount.push_back(1);
			pnowidx=point.size()-1;
		}
		tnow[0]=pnowidx;


		//second point
		pnow[0]=data[i][cs+1];
		pnow[1]=data[i][cs+4];
		pnow[2]=data[i][cs+7];
		pnowidx=0;
		for(j=1;j<point.size();j++){
			if(pnow[0]==point[j][0])
				if(pnow[1]==point[j][1])
					if(pnow[2]==point[j][2]){
						pnowidx=j;
						pointValue[j]+=data[i][cs+9];
						pcount[j]+=1;
						break;
					}

		}
		if(pnowidx==0){
			point.push_back(pnow);
			pointValue.push_back(data[i][cs+9]);
			pcount.push_back(1);
			pnowidx=point.size()-1;
		}
		tnow[1]=pnowidx;



		//third point
		pnow[0]=data[i][cs+2];
		pnow[1]=data[i][cs+5];
		pnow[2]=data[i][cs+8];
		pnowidx=0;
		for(j=1;j<point.size();j++){
			if(pnow[0]==point[j][0])
				if(pnow[1]==point[j][1])
					if(pnow[2]==point[j][2]){
						pnowidx=j;
						pointValue[j]+=data[i][cs+9];
						pcount[j]+=1;
						break;
					}

		}
		if(pnowidx==0){
			point.push_back(pnow);
			pointValue.push_back(data[i][cs+9]);
			pcount.push_back(1);
			pnowidx=point.size()-1;
		}
		tnow[2]=pnowidx;

		//triangle
		triangle.push_back(tnow);

	}

	//interpolate point value using average 
	for(i=1;i<pointValue.size();i++){
		pointValue[i]/=(double)pcount[i];
	}

	calPointValueRange();
	calTriangleValueRange();

	genPointToFaceMap();
	genFaceCentroid();
	genEdge();


}


void surfaceMesh::calPointValueRange()
{
	double *temp=pointValue.data();
	auto result=std::minmax_element(&temp[1],&temp[pointValue.size()]);

	minPointValue=*result.first;
	maxPointValue=*result.second;
}


void surfaceMesh::calTriangleValueRange()
{
		double *temp=triangleValue.data();
		auto result=std::minmax_element(&temp[1],&temp[triangleValue.size()]);

		minTriangleValue=*result.first;
		maxTriangleValue=*result.second;
}