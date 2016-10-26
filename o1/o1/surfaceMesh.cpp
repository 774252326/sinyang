#include "StdAfx.h"
#include "surfaceMesh.h"


surfaceMesh::surfaceMesh(void)
	: tri(3)
	, isClose(false)
{
}


surfaceMesh::~surfaceMesh(void)
{
}


bool surfaceMesh::loadPoint(double ** pt, long np)
{
	long i,j;
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

		std::vector<long> line;

		for(i=0;i<=np;i++){
			point2triangle.push_back(line);
			//point2triangle[i].push_back(i);
		}

		//for(i=1;i<point.size1();i++){
		//	for(j=0;j<point.size2();j++){
		//		point(i,j)=pt[i][j+1];
		//	}
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

		//long maxi=1;
		//long mini=1;
		//for(long i=1;i<pointValue.size();i++){
		//	pointValue(i)=ptv[i];
		//	if(ptv[i]>ptv[maxi])
		//		maxi=i;
		//	else
		//		if(ptv[i]<ptv[mini])
		//			mini=i;
		//}

		pointValue.assign(&ptv[0],&ptv[pointValue.size()]);

		double *temp=pointValue.data();
		auto result=std::minmax_element(&temp[1],&temp[pointValue.size()]);

		minPointValue=*result.first;
		maxPointValue=*result.second;
		return true;
	}
	else{
		return false;
	}
}


bool surfaceMesh::loadFace(long ** fc, long nf)
{
	long i,j;
	if(nf>0){

		//if(nface!=nf){
		//	if(nface>0)
		//		free_matrix(face,1,nface,1,dim);
		//	nface=nf;
		//	face=matrix<long>(1,nface,1,dim);
		//	faceValue=vector0<double>(1,nface);
		//	faceNext=matrix0<long>(1,nface,1,dim);
		//	faceEdge=matrix<long>(1,nface,1,dim);
		//	faceCentroid=matrix<double>(1,nface,1,dim);

		//}
		//copymx(fc,nface,dim,face);

		//triangle.resize(nf+1,tri);
		//triangleValue.resize(nf+1);
		//triangleNext.resize(nf+1,tri);
		//triangleEdge.resize(nf+1,tri);
		//triangleCentroid.resize(nf+1,tri);

		//for(i=1;i<triangle.size1();i++){
		//	for(j=0;j<triangle.size2();j++){
		//		triangle(i,j)=fc[i][j+1];
		//	}
		//}



		triangle.clear();
		std::vector<long> onetriangle;
		triangle.push_back(onetriangle);
		triangleEdge.push_back(onetriangle);

		for(i=1;i<=nf;i++){
			onetriangle.assign(&fc[i][1],&fc[i][1+tri]);
			triangle.push_back(onetriangle);
			//onetriangle.clear();
			triangleEdge.push_back(onetriangle);
		}

		triangleValue.resize(nf+1);
		triangleNext.resize(nf+1);
		//triangleEdge.resize(nf+1);
		//triangleCentroid.resize(nf+1);



		return true;
	}
	else{
		return false;
	}

}





bool surfaceMesh::loadFaceV(double * fcv, long nf)
{
	if (nf==(triangleValue.size()-1) ){
		//if(nface!=nf){
		//	if(nface>0)
		//		free_vector(faceValue,1,nface);
		//	nface=nf;
		//	faceValue=vector<double>(1,nface);
		//}

		//copyvt(fcv,nface,faceValue);

		//maxFaceValue=findmax(faceValue,nface);
		//minFaceValue=findmin(faceValue,nface);


		//long maxi=1;
		//long mini=1;
		//for(long i=1;i<triangleValue.size();i++){
		//	triangleValue(i)=fcv[i];
		//	if(fcv[i]>fcv[maxi])
		//		maxi=i;
		//	else
		//		if(fcv[i]<fcv[mini])
		//			mini=i;
		//}

		//maxTriangleValue=triangleValue(maxi);
		//minTriangleValue=triangleValue(mini);




		triangleValue.assign(&fcv[0],&fcv[triangleValue.size()]);
		double *temp=triangleValue.data();
		auto result=std::minmax_element(&temp[1],&temp[triangleValue.size()]);

		minTriangleValue=*result.first;
		maxTriangleValue=*result.second;



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
	//isvolumn=0, if input a planar grid

	std::vector<long> currentedge(2);
	edge.push_back(currentedge);
	long nedge=0;

	std::vector<long> newedge;
	edgeTriangle.push_back(newedge);

	std::vector<long> onetriangle;

	long i,j,jn;
	long edgei;
	long *p;
	long nextfi;
	edgei=0;
	for(i=1;i<triangle.size();i++){

		for(j=0;j<triangle[i].size();j++){

			jn=( (j==triangle[i].size()-1)? 0:j+1 );
			currentedge[0]=triangle[i][j];
			currentedge[1]=triangle[i][jn];

			edgei=findEdge(currentedge);

			if(edgei==0){
				edgei=edge.size();
				edge.push_back(currentedge);				
				triangleEdge[i][j]=edgei;
				edgeTriangle.push_back(newedge);
				edgeTriangle[edgei].push_back(i);
			}
			else{
				triangleEdge[i][j]=edgei;
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
	for(i=1;i<triangle.size();i++){
		for(j=0;j<triangle[i].size();j++){
			point2triangle[triangle[i][j]].push_back(i);
		}
	}

}


std::vector< std::vector<double> > surfaceMesh::findContourInOneTriangle(long triangleIndex, double v, long * pinLocate, long * poutLocate)
{
	long *p;
	std::vector<long> ptriangle;
	ptriangle=triangle[triangleIndex];
	long *e;
	std::vector<long> etriangle;
	etriangle=triangleEdge[triangleIndex];

	double e1,e2,e3,e12,e23,e31;
	long i;

	std::vector<double> pin;
	std::vector<double> pout;

	std::vector< std::vector<double> > pinout;

	e1=pointValue[ptriangle[0]]-v;
	e2=pointValue[ptriangle[1]]-v;
	e3=pointValue[ptriangle[2]]-v;

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
				//copyvt(point[p[1]],dim,pin);				
				pinout.push_back(point[ptriangle[0]]);
				pinLocate[0]=-ptriangle[0];

				//copyvt(point[p[2]],dim,pout);
				pinout.push_back(point[ptriangle[1]]);
				poutLocate[0]=-ptriangle[1];
			}
		}
		else{
			if(e3==0){
				//copyvt(point[p[1]],dim,pin);
				//pinLocate[0]=-p[1];
				//copyvt(point[p[dim]],dim,pout);
				//poutLocate[0]=-p[dim];

				pinout.push_back(point[ptriangle[0]]);
				pinLocate[0]=-ptriangle[0];
				pinout.push_back(point[ptriangle[2]]);
				poutLocate[0]=-ptriangle[2];


			}
			else{
				if(e23<0){
					//copyvt(point[p[1]],dim,pin);
					//pinLocate[0]=-p[1];
					pinout.push_back(point[ptriangle[0]]);
					pinLocate[0]=-ptriangle[0];

					for(i=0;i<tri;i++){
						pout.push_back(rsl(pointValue[ptriangle[1]],pointValue[ptriangle[2]],point[ptriangle[1]][i],point[ptriangle[2]][i],v));
					}
					pinout.push_back(pout);
					pout.clear();
					poutLocate[0]=etriangle[1];
				}
			}
		}

	}
	else{
		if(e2==0){
			if(e3==0){
				//copyvt(point[p[2]],dim,pin);
				//pinLocate[0]=-p[2];
				//copyvt(point[p[dim]],dim,pout);
				//poutLocate[0]=-p[dim];

				pinout.push_back(point[ptriangle[1]]);
				pinLocate[0]=-ptriangle[1];
				pinout.push_back(point[ptriangle[2]]);
				poutLocate[0]=-ptriangle[2];

			}
			else{
				if(e31<0){
					//copyvt(point[p[2]],dim,pin);
					//pinLocate[0]=-p[2];

					pinout.push_back(point[ptriangle[1]]);
					pinLocate[0]=-ptriangle[1];

					//for(i=1;i<=dim;i++){
					//pout[i]=rsl(pointV[p[1]],pointV[p[dim]],point[p[1]][i],point[p[dim]][i],v);
					//}
					//poutLocate[0]=e[dim];

					for(i=0;i<tri;i++){
						pout.push_back(rsl(pointValue[ptriangle[0]],pointValue[ptriangle[2]],point[ptriangle[0]][i],point[ptriangle[2]][i],v));
					}
					pinout.push_back(pout);
					pout.clear();
					poutLocate[0]=etriangle[2];


				}
			}
		}
		else{
			if(e3==0){
				if(e12<0){
					//copyvt(point[p[dim]],dim,pin);
					//pinLocate[0]=-p[dim];

					pinout.push_back(point[ptriangle[2]]);
					pinLocate[0]=-ptriangle[2];


					//for(i=1;i<=dim;i++){
					//	pout[i]=rsl(pointV[p[1]],pointV[p[2]],point[p[1]][i],point[p[2]][i],v);
					//}
					//poutLocate[0]=e[1];

					for(i=0;i<tri;i++){
						pout.push_back(rsl(pointValue[ptriangle[0]],pointValue[ptriangle[1]],point[ptriangle[0]][i],point[ptriangle[1]][i],v));
					}
					pinout.push_back(pout);
					pout.clear();
					poutLocate[0]=etriangle[0];

				}
			}
			else{
				if(e12<0){
					if(pinLocate[0]==0){
						//for(i=1;i<=dim;i++){
						//pin[i]=rsl(pointV[p[1]],pointV[p[2]],point[p[1]][i],point[p[2]][i],v);
						//}
						//pinLocate[0]=e[1];

						for(i=0;i<tri;i++){
							pin.push_back(rsl(pointValue[ptriangle[0]],pointValue[ptriangle[1]],point[ptriangle[0]][i],point[ptriangle[1]][i],v));
						}
						pinLocate[0]=etriangle[0];


					}
					else{
						//for(i=1;i<=dim;i++){
						//	pout[i]=rsl(pointV[p[1]],pointV[p[2]],point[p[1]][i],point[p[2]][i],v);
						//}
						//poutLocate[0]=e[1];


						for(i=0;i<tri;i++){
							pout.push_back(rsl(pointValue[ptriangle[0]],pointValue[ptriangle[1]],point[ptriangle[0]][i],point[ptriangle[1]][i],v));
						}
						poutLocate[0]=etriangle[0];

					}					
				}

				if(e23<0){
					if(pinLocate[0]==0){
						//for(i=1;i<=3;i++){
						//	pin[i]=rsl(pointV[p[3]],pointV[p[2]],point[p[3]][i],point[p[2]][i],v);
						//}
						//pinLocate[0]=e[2];

						for(i=0;i<tri;i++){
							pin.push_back(rsl(pointValue[ptriangle[2]],pointValue[ptriangle[1]],point[ptriangle[2]][i],point[ptriangle[1]][i],v));
						}
						pinLocate[0]=etriangle[1];

					}
					else{
						//for(i=1;i<=3;i++){
						//	pout[i]=rsl(pointV[p[3]],pointV[p[2]],point[p[3]][i],point[p[2]][i],v);
						//}
						//poutLocate[0]=e[2];

						for(i=0;i<tri;i++){
							pout.push_back(rsl(pointValue[ptriangle[2]],pointValue[ptriangle[1]],point[ptriangle[2]][i],point[ptriangle[1]][i],v));
						}
						poutLocate[0]=etriangle[2];
					}					
				}

				if(e31<0){
					if(pinLocate[0]==0){
						//for(i=1;i<=3;i++){
						//	pin[i]=rsl(pointV[p[1]],pointV[p[3]],point[p[1]][i],point[p[3]][i],v);
						//}
						//pinLocate[0]=e[3];

						for(i=0;i<tri;i++){
							pin.push_back(rsl(pointValue[ptriangle[0]],pointValue[ptriangle[2]],point[ptriangle[0]][i],point[ptriangle[2]][i],v));
						}
						pinLocate[0]=etriangle[2];

					}
					else{
						//for(i=1;i<=3;i++){
						//	pout[i]=rsl(pointV[p[1]],pointV[p[3]],point[p[1]][i],point[p[3]][i],v);
						//}
						//poutLocate[0]=e[3];


						for(i=0;i<tri;i++){
							pout.push_back(rsl(pointValue[ptriangle[0]],pointValue[ptriangle[2]],point[ptriangle[0]][i],point[ptriangle[2]][i],v));
						}
						poutLocate[0]=etriangle[2];

					}					
				}

				pinout.push_back(pin);
				pinout.push_back(pout);

			}
		}
	}

	return pinout;
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
	long i,j,k,nd;
	//double pin[4];
	//double pout[4];
	//double ptemp[4];
	long pinLocate,poutLocate,temp;

	long currentPlocate=0,prevPlocate,startLocate;

	//std::vector< std::vector<double> > contourp(tri,std::vector<double>(0));
	std::vector< std::vector<double> > contourp;

	std::vector<long> nextfacelist;

	std::vector< std::vector<double> > nextp;
	std::vector<double> onept;


	//bool *faceChecked=vector1<bool>(1,nface);

	std::vector<bool> triangleUnchecked;
	triangleUnchecked.assign(triangle.size(),true);
	bool lost;

	for(i=1;i<triangle.size();i++){
		if(triangleUnchecked[i]){
			nextp=findContourInOneTriangle(i,v,&pinLocate,&poutLocate);
			triangleUnchecked[i]=false;
			if( (pinLocate!=0) && (poutLocate!=0) ){
				contourp.clear();
				//contourp.push_back(nextp[0]);
				//onept=nextp[1];
				contourp=nextp;
				startLocate=pinLocate;

				lost=false;
				while( (poutLocate!=0) && (poutLocate!=startLocate) && !lost ){
					//prevPlocate=pinLocate;
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
							triangleUnchecked[nextfacelist[j]]=false;
							if(poutLocate!=0){
								onept=calPointAtLocate(poutLocate,v);
								contourp.push_back(onept);
								lost=false;
								break;
							}
						}
					}

					//std::cout<<v<<'\n';

				}

				//print contour
				//if(!contourp.empty()){

				contourv.push_back(contourp);
				contourValue.push_back(v);
				contourp.clear();

			}


		}
	}

}

long surfaceMesh::findOutPointLocateInOneTriangle(long triangleIndex, double v, long pinLocate)
{


	std::vector<long> tp;
	tp=triangle[triangleIndex];

	std::vector<long> te;
	te=triangleEdge[triangleIndex];
	double e0,e1,e2;
	//poutLocate[0]=0;

	//std::vector<double> pout;


	e0=pointValue[tp[0]]-v;
	e1=pointValue[tp[1]]-v;
	e2=pointValue[tp[2]]-v;

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
		for(long i=0;i<tri;i++){
			pout.push_back(rsl(pointValue[edge[pLocate][0]],pointValue[edge[pLocate][1]],point[edge[pLocate][0]][i],point[edge[pLocate][1]][i],v));
		}			
	}
	else{
		if(pLocate<0){
			pout=point[-pLocate];
		}
	}
	return pout;

}

void surfaceMesh::genContourMap(long contourNumber)
{
	contourv.clear();
	contourValue.clear();

	double cvalue;
	double vstep=(maxPointValue-minPointValue)/(double)(contourNumber+1);

	for(cvalue=minPointValue;cvalue<=maxPointValue;cvalue+=vstep){
		findContour(cvalue);
	}

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

void surfaceMesh::loadPointAndFace(double ** pt, long np, long ** fc, long nf)
{
	loadPoint(pt,np);
	loadFace(fc,nf);
	//getFaceNext();
	genEdge();
	genPointToFaceMap();
	genFaceCentroid();

}