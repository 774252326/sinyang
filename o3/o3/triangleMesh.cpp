#include "StdAfx.h"
#include "triangleMesh.h"
#include "../../funT\nrutilT.h"
#include "../../funT\paddingT.h"
#include "../../funT\findmT.h"
#include "../..\funT\piksr2T.h"
#include "../../funT/bhsintT.h"

#include <algorithm>
#include <vector>
#include <iostream>

triangleMesh::triangleMesh(void)
	: nface(0)
	, npoint(0)
	, point(NULL)
	, face(NULL)
	, pointValue(NULL)
	, faceValue(NULL)
	, maxPointValue(0)
	, minPointValue(0)
	, maxFaceValue(0)
	, minFaceValue(0)
	, faceNext(NULL)
	, edge(NULL)
	, nedge(0)
	, faceEdge(NULL)
	, edgeFace(NULL)
	, faceCentroid(NULL)
	, pointValueInterp(0)
	, dim(3)
	, maxPointValueInterp(0)
	, minPointValueInterp(0)
{

}

triangleMesh::triangleMesh(long np, long nf)
{
	npoint=np;
	point=matrix<float>(1,npoint,1,dim);
	pointValue=vector<float>(1,npoint);
	std::vector<long> line;
	long i;
	for(i=0;i<=npoint;i++){
		point2face.push_back(line);
		point2face[i].push_back(i);

	}


	nface=nf;

	face=matrix<long>(1,nface,1,dim);
	faceValue=vector<float>(1,nface);
	faceNext=matrix<long>(1,nface,1,dim);
	faceEdge=matrix<long>(1,nface,1,dim);

	nedge=nface+npoint-1;
	edge=matrix<long>(1,nedge,1,2);
	edgeFace=matrix<long>(1,nedge,1,2);





	//contourv1.push_back(point2face);
	//contourv1.push_back(point2face);
	//contourv1[0].push_back(line);
	//contourv1[1].push_back(line);
	//contourv1[0][0].push_back(10);




}


triangleMesh::~triangleMesh(void)
{
	free_matrix(point,1,npoint,1,dim);
	free_matrix(face,1,nface,1,dim);
	free_vector(pointValue,1,npoint);
	free_vector(faceValue,1,nface);
}


bool triangleMesh::loadPoint(float ** pt, long np)
{
	if(np>0){
		if(npoint!=np){
			if(npoint>0)
				free_matrix(point,1,npoint,1,dim);
			npoint=np;
			point=matrix<float>(1,npoint,1,dim);
			pointValue=vector<float>(1,npoint);
			pointValueInterp=vector<float>(1,npoint);
			std::vector<long> line;
			long i;
			for(i=0;i<=npoint;i++){
				point2face.push_back(line);
				point2face[i].push_back(i);

			}
		}

		copymx(pt,npoint,dim,point);
		return true;
	}
	else{
		return false;
	}
}


bool triangleMesh::loadFace(long ** fc, long nf)
{
	if(nf>0){
		if(nface!=nf){
			if(nface>0)
				free_matrix(face,1,nface,1,dim);
			nface=nf;
			face=matrix<long>(1,nface,1,dim);
			faceValue=vector0<float>(1,nface);
			faceNext=matrix0<long>(1,nface,1,dim);
			faceEdge=matrix<long>(1,nface,1,dim);
			faceCentroid=matrix<float>(1,nface,1,dim);

			//nedge=nface+npoint-1;
			//edge=matrix<long>(1,nedge,1,2);
			//edgeFace=matrix0<long>(1,nedge,1,2);
		}
		copymx(fc,nface,dim,face);
		return true;
	}
	else{
		return false;
	}

}



bool triangleMesh::loadPointV(float * ptv, long np)
{
	if(np>0){
		if(npoint!=np){
			if(npoint>0)
				free_vector(pointValue,1,npoint);
			npoint=np;
			pointValue=vector<float>(1,npoint);
		}

		copyvt(ptv,npoint,pointValue);

		maxPointValue=findmax(pointValue,npoint);
		minPointValue=findmin(pointValue,npoint);
		return true;
	}
	else{
		return false;
	}
}


bool triangleMesh::loadFaceV(float * fcv, long nf)
{
	if(nf>0){
		if(nface!=nf){
			if(nface>0)
				free_vector(faceValue,1,nface);
			nface=nf;
			faceValue=vector<float>(1,nface);
		}

		copyvt(fcv,nface,faceValue);

		maxFaceValue=findmax(faceValue,nface);
		minFaceValue=findmin(faceValue,nface);
		return true;
	}
	else{
		return false;
	}
}

//check if contour v pass through triangle, return true for yes.
//p1,p2 are two intersection of contour and triangle edge
//if p1[0] and p2[0] > 0, p1,p2 is at edge p1[0] and p2[0] of triangle
//if p1[0] and p2[0] < 0, p1,p2 is at vertex p1[0] and p2[0] of triangle
//bool triangleMesh::findLine(long * triangle, float v, float * p1, float * p2)
//{	
//	std::vector<float> px;
//	std::vector<float> py;
//	std::vector<float> pz;
//	std::vector<float> pid;
//
//	long i;
//	float judge;
//	float v1,v2;
//
//	bool p2line=0;
//
//	for(i=1;i<dim;i++){
//		v1=pointValue[triangle[i]];
//		v2=pointValue[triangle[i+1]];
//		judge=(v1-v)*(v2-v);
//		if(judge<0){
//			px.push_back(rsl(v1,v2,point[triangle[i]][1],point[triangle[i+1]][1],v));
//			py.push_back(rsl(v1,v2,point[triangle[i]][2],point[triangle[i+1]][2],v));
//			pz.push_back(rsl(v1,v2,point[triangle[i]][3],point[triangle[i+1]][3],v));
//			pid.push_back(i);
//		}
//		else{
//			if(judge==0){
//				if(v1==v){
//					px.push_back(point[triangle[i]][1]);
//					py.push_back(point[triangle[i]][2]);
//					pz.push_back(point[triangle[i]][3]);
//					pid.push_back(-i);
//				}
//			}
//		}
//	}
//
//	v1=pointValue[triangle[3]];
//	v2=pointValue[triangle[1]];
//	judge=(v1-v)*(v2-v);
//	if(judge<0){
//		px.push_back(rsl(v1,v2,point[triangle[3]][1],point[triangle[1]][1],v));
//		py.push_back(rsl(v1,v2,point[triangle[3]][2],point[triangle[1]][2],v));
//		pz.push_back(rsl(v1,v2,point[triangle[3]][3],point[triangle[1]][3],v));
//		pid.push_back(3);
//	}
//	else{
//		if(judge==0){
//			if(v1==v){
//				px.push_back(point[triangle[3]][1]);
//				py.push_back(point[triangle[3]][2]);
//				pz.push_back(point[triangle[3]][3]);
//				pid.push_back(-3);
//			}
//		}
//	}
//
//	if(px.size()==2){
//		p1[0]=pid[0];
//		p1[1]=px[0];
//		p1[2]=py[0];
//		p1[3]=pz[0];
//
//		p2[0]=pid[1];
//		p2[1]=px[1];
//		p2[2]=py[1];
//		p2[3]=pz[1];
//		return true;
//	}
//	else{
//		return false;
//	}
//
//
//}


// sort triangle vertex in face matrix according to point value
void triangleMesh::sortFaceVertex(void)
{
	long i,j;
	long nd=dim;
	float *pv=vector<float>(1,nd);

	for(i=1;i<=nface;i++){		
		for(j=1;j<=nd;j++)
			pv[j]=pointValue[face[i][j]];

		piksr2(nd,pv,face[i]);
	}
}




void triangleMesh::getFaceNext(void)
{
	long i,j,k;
	long lastedge[2];
	long nvertex=3;
	long loc;
	for(i=1;i<=nface;i++){



		for(k=1;k<nvertex;k++){

			for(j=i+1;j<=nface;j++){
				//loc=isBelongToFace(&face[i][k],face[j],nvertex);
				loc=isBelongToTriangle(&face[i][k],face[j]);
				if(loc>0){
					faceNext[i][k]=j;
					faceNext[j][loc]=i;
					break;
				}
			}

		}

		lastedge[0]=face[i][nvertex];
		lastedge[1]=face[i][1];

		for(j=i+1;j<=nface;j++){
			//loc=isBelongToFace(lastedge,face[j],nvertex);
			loc=isBelongToTriangle(lastedge,face[j]);
			if(loc>0){
				faceNext[i][nvertex]=j;
				faceNext[j][loc]=i;
				break;
			}
		}



	}

}

//return n if edge is nth edge of facet, else return 0;
long triangleMesh::isBelongToFace(long * edget, long * facet, long nv)
{
	long location=0;
	std::vector<long> faceloop(&facet[1],&facet[1+nv]);
	faceloop.push_back(facet[1]);

	std::vector<long>::iterator it;

	it=std::search(faceloop.begin(),faceloop.end(),&edget[0],&edget[0+2]);

	if(it!=faceloop.end()){
		location=1+(it-faceloop.begin());
	}
	else{
		long redge[2]={edget[1],edget[0]};
		it=std::search(faceloop.begin(),faceloop.end(),&redge[0],&redge[0+2]);
		if(it!=faceloop.end()){
			location=1+(it-faceloop.begin());
		}
	}
	return location;
}


double triangleMesh::rsl(double x1, double x2, double y1, double y2, double x)
{
	double dx=x2-x1;

	if(dx==0)
		return 0;
	else
		return (x*(y2-y1)+x2*y1-x1*y2)/dx;
}


//float ** triangleMesh::findOneContour(float v, bool *faceOfInterest, long * np)
//{
//	float pnow[4];
//	float pend[4];
//
//	std::vector<float> px;
//	std::vector<float> py;
//	std::vector<float> pz;
//	bool in;
//	long i,j;
//	for(i=1;i<=nface;i++){
//		if(faceOfInterest[i]){
//			in=findLine(face[i],v,pnow,pend);
//			if(in){
//				px.push_back(pend[1]);
//				py.push_back(pend[2]);
//				pz.push_back(pend[3]);
//
//				px.push_back(pnow[1]);
//				py.push_back(pnow[2]);
//				pz.push_back(pnow[3]);
//
//				faceOfInterest[i]=false;
//
//				while(true){
//
//					for(j=1;j<=3;j++){
//						//in=findLine(
//					}}}}}
//
//	return NULL;
//}


//long * triangleMesh::faceAtVertex(long pt, long *nfav)
//{
//	std::vector<long> fav;
//	long i;
//	for(i=1;i<=nface;i++){
//		if( (face[i][1]==pt) || (face[i][2]==pt) || (face[i][3]==pt) )
//			fav.push_back(i);
//	}
//	nfav[0]=fav.size();
//	if(nfav[0]>0){
//		long *faceatv=vector<long>(1,nfav[0]);
//		copyvt(&fav[0]-1,nfav[0],faceatv);
//		return faceatv;
//	}
//	else{
//		return NULL;
//	}
//}


// generate edge list and face to edge list from faceNext matrix
void triangleMesh::genEdge(BOOL isvolume)
{
	//isvolume=1, if input a volumn grid;
	//isvolumn=0, if input a planar grid
	nedge=nface+npoint-1-isvolume;
	edge=matrix<long>(1,nedge,1,2);
	edgeFace=matrix0<long>(1,nedge,1,2);

	long ** facenextcopy=matrix<long>(1,nface,1,dim);
	copymx(faceNext,nface,dim,facenextcopy);

	long i,j;
	long edgei;
	long *p;
	long nextfi;
	edgei=0;
	for(i=1;i<=nface;i++){

		for(j=1;j<dim;j++){

			if(facenextcopy[i][j]>0){
				edgei++;
				edge[edgei][1]=face[i][j];
				edge[edgei][2]=face[i][j+1];

				//edge[edgei][3]=i;
				//edge[edgei][4]=facenextcopy[i][j];

				edgeFace[edgei][1]=i;
				faceEdge[i][j]=edgei;

				edgeFace[edgei][2]=facenextcopy[i][j];
				p=std::find(&facenextcopy[edgeFace[edgei][2]][1],&facenextcopy[edgeFace[edgei][2]][1+dim],i);
				nextfi=p-&facenextcopy[edgeFace[edgei][2]][1]+1;				
				faceEdge[edgeFace[edgei][2]][nextfi]=edgei;

				*p=-1;
			}
			else{
				if(facenextcopy[i][j]==0){
					edgei++;
					edge[edgei][1]=face[i][j];
					edge[edgei][2]=face[i][j+1];
					edgeFace[edgei][1]=i;
					faceEdge[i][j]=edgei;

					edgeFace[edgei][2]=0;
				}
			}
		}

		if(facenextcopy[i][dim]>0){
			edgei++;
			edge[edgei][1]=face[i][dim];
			edge[edgei][2]=face[i][1];

			//edge[edgei][3]=i;
			//edge[edgei][4]=facenextcopy[i][3];

			edgeFace[edgei][1]=i;
			faceEdge[i][dim]=edgei;

			edgeFace[edgei][2]=facenextcopy[i][dim];

			p=std::find(&facenextcopy[edgeFace[edgei][2]][1],&facenextcopy[edgeFace[edgei][2]][1+dim],i);

			nextfi=p-&facenextcopy[edgeFace[edgei][2]][1]+1;

			faceEdge[edgeFace[edgei][2]][nextfi]=edgei;

			*p=-1;
		}
		else{
			if(facenextcopy[i][dim]==0){
				edgei++;
				edge[edgei][1]=face[i][dim];
				edge[edgei][2]=face[i][1];
				edgeFace[edgei][1]=i;
				faceEdge[i][dim]=edgei;

				edgeFace[edgei][2]=0;
			}
		}
	}

	free_matrix(facenextcopy,1,nface,1,dim);


}


// next edge list from edgei
//long * triangleMesh::nextEdgeFromEdge(long edgei, long *nne)
//{
//	//long *temp1;
//	//temp1=vector<long>(1,3);
//	//copyvt(faceEdge[edgeFace[edgei][1]],3,temp1);
//	//std::sort(&temp1[1],&temp1[1+3]);
//
//	//long *temp2;
//	//temp2=vector<long>(1,3);
//	//copyvt(faceEdge[edgeFace[edgei][2]],3,temp2);
//	//std::sort(&temp2[1],&temp2[1+3]);
//
//
//	//std::vector<long> v(6);
//	//std::vector<long>::iterator it;
//
//	//it=std::set_union(&temp1[1],&temp1[1+3],&temp2[1],&temp2[1+3],v.begin());
//
//	//v.resize(it-v.begin());
//
//	//free_vector(temp1,1,3);
//	//free_vector(temp2,1,3);
//
//	//temp1=vector<long>(1,v.size());
//	//copyvt(&v[0]-1,v.size(),temp1);
//
//	//temp2=&edgei;
//
//	//it=std::set_difference(&temp1[1],&temp1[1+v.size()],temp2,temp2+1,v.begin());
//
//	//v.resize(it-v.begin());
//
//
//
//	//if(v.size()>0){
//	//	free_vector(temp1,1,3);
//
//	//	temp1=vector<long>(1,v.size());
//	//	copyvt(&v[0]-1,v.size(),temp1);
//	//	nne[0]=v.size();
//	//	return temp1;
//	//}
//	//else{
//	//	nne[0]=0;
//	//	return NULL;
//	//}
//
//	nne[0]=4;
//	long *nextedgeloop=vector<long>(1,nne[0]);
//	long face1=edgeFace[edgei][1];
//	long face2=edgeFace[edgei][2];
//	long p11,p12,p21,p22;
//	if(faceEdge[face1][1]==edgei){
//		p11=face[face1][1];
//		p12=face[face1][2];
//		nextedgeloop[1]=faceEdge[face1][2];
//		nextedgeloop[2]=faceEdge[face1][3];
//	}
//	else{
//		if(faceEdge[face1][2]==edgei){
//		p11=face[face1][2];
//		p12=face[face1][3];
//		nextedgeloop[1]=faceEdge[face1][3];
//		nextedgeloop[2]=faceEdge[face1][1];
//		}
//	}
//
//}


//long * triangleMesh::nextEdgeFromPoint(long edgei, long pointi, long *nne)
//{
//	long currentface=edgeFace[edgei][1];
//	long pidx=edge[edgei][pointi];
//	long nextface1,nextface2;
//	long *p;
//	long pidx4face,prevpidx,nextpidx;
//	long ed;
//	long prevface=currentface;
//
//	std::vector<long> edgelist;
//
//	do{
//		p=std::find(&face[currentface][1],&face[currentface][1+3],pidx);
//
//		pidx4face=p-&face[currentface][1]+1;
//
//
//		nextpidx=((pidx4face==3)?1:pidx4face+1);
//		prevpidx=((pidx4face==1)?3:pidx4face-1);
//		nextface1=faceNext[currentface][pidx4face];
//		nextface2=faceNext[currentface][prevpidx];
//		ed=faceEdge[currentface][nextpidx];
//		edgelist.push_back(ed);
//		if(nextface1==prevface){
//			prevface=currentface;
//			currentface=nextface2;
//		}
//		else{
//			prevface=currentface;
//			currentface=nextface1;
//		}
//		//currentface=nextface;
//
//	}while(currentface!=edgeFace[edgei][1]);
//
//	nne[0]=edgelist.size();
//
//	if(nne[0]==0){
//		return NULL;
//	}
//	else{
//		long *ne=vector<long>(1,nne[0]);
//		copyvt(&edgelist[0]-1,nne[0],ne);
//		return ne;
//	}
//
//
//
//}




//float * triangleMesh::getNextPoint(long * edgelist, long nel, float v, long * edgeBelong)
//{
//	std::vector<float> px;
//	std::vector<float> py;
//	std::vector<float> pz;
//	std::vector<long> pid;
//
//	long i;
//	float temp;
//	float templo;
//	long pointindex1,pointindex2;
//	long edgei;
//	float p1[4];
//	float p2[4];
//	long p1idx;
//	long p2idx;
//	for(i=1;i<=nel;i++){
//		edgei=edgelist[i];
//		getPointFromOneEdge(edgei,v,p1,&p1idx,p2,&p2idx);
//
//		£ý
//
//	return NULL;
//}


//void triangleMesh::getPointFromOneEdge(long edgei, float v, float * p1, long *p1idx, float * p2, long *p2idx)
//{
//	float temp;
//	long k;
//	p1idx[0]=-1;
//	p2idx[0]=-1;
//	temp=(pointValue[edge[edgei][1]]-v)*(pointValue[edge[edgei][2]]-v);
//	if(temp<0){
//		for(k=1;k<=3;k++){
//			p1[k]=rsl(pointValue[edge[edgei][1]],pointValue[edge[edgei][2]],point[edge[edgei][1]][k],point[edge[edgei][2]][k],v);
//		}
//		p1idx[0]=0;
//	}
//	else{
//		if(pointValue[edge[edgei][1]]==v){
//			p1idx[0]=edge[edgei][1];
//			for(k=1;k<=3;k++){
//				p1[k]=point[p1idx[0]][k];
//			}
//		}
//		if(pointValue[edge[edgei][2]]==v){
//			p2idx[0]=edge[edgei][2];
//			for(k=1;k<=3;k++){
//				p2[k]=point[p2idx[0]][k];
//			}
//		}
//	}
//}


//long * triangleMesh::edgeLoopToPointLoop(long * edgelist, long nel)
//{
//	long *pointloop=vector<long>(1,nel);
//	long i;
//	pointloop[1]=edge[edgelist[1]][1];
//	pointloop[2]=edge[edgelist[1]][2];
//	for(i=3;i<=nel;i++){
//		if(edge[edgelist[i-1]][1]!=pointloop[i-1])
//			pointloop[i]=edge[edgelist[i-1]][1];
//		else
//			pointloop[i]=edge[edgelist[i-1]][2];
//	}
//	return pointloop;
//}


void triangleMesh::genPointToFaceMap(void)
{
	long i,j;
	for(i=1;i<=nface;i++){
		for(j=1;j<=dim;j++){
			point2face[face[i][j]].push_back(i);
		}
	}

	//for(i=1;i<=npoint;i++){
	//	for(j=1;j<point2face[i].size();j++)
	//		std::cout<<point2face[i][j]<<" ";
	//	std::cout<<"\n";
	//}


}


void triangleMesh::findContourInOneTriangle(long triangleIndex, float v, float * pin, long * pinLocate, float * pout, long * poutLocate)
{
	long *p;
	p=face[triangleIndex];
	long *e;
	e=faceEdge[triangleIndex];

	float e1,e2,e3,e12,e23,e31;
	long i;

	float *pointV;

	pointV=pointValueInterp;
	//pointV=pointValue;

	e1=pointV[p[1]]-v;
	e2=pointV[p[2]]-v;
	e3=pointV[p[3]]-v;

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
				copyvt(point[p[1]],dim,pin);
				pinLocate[0]=-p[1];
				copyvt(point[p[2]],dim,pout);
				poutLocate[0]=-p[2];
			}
		}
		else{
			if(e3==0){
				copyvt(point[p[1]],dim,pin);
				pinLocate[0]=-p[1];
				copyvt(point[p[dim]],dim,pout);
				poutLocate[0]=-p[dim];
			}
			else{
				if(e23<0){
					copyvt(point[p[1]],dim,pin);
					pinLocate[0]=-p[1];

					for(i=1;i<=dim;i++){
						pout[i]=rsl(pointV[p[2]],pointV[p[dim]],point[p[2]][i],point[p[dim]][i],v);
					}
					poutLocate[0]=e[2];
				}
			}
		}

	}
	else{
		if(e2==0){
			if(e3==0){
				copyvt(point[p[2]],dim,pin);
				pinLocate[0]=-p[2];
				copyvt(point[p[dim]],dim,pout);
				poutLocate[0]=-p[dim];
			}
			else{
				if(e31<0){
					copyvt(point[p[2]],dim,pin);
					pinLocate[0]=-p[2];

					for(i=1;i<=dim;i++){
						pout[i]=rsl(pointV[p[1]],pointV[p[dim]],point[p[1]][i],point[p[dim]][i],v);
					}
					poutLocate[0]=e[dim];
				}
			}
		}
		else{
			if(e3==0){
				if(e12<0){
					copyvt(point[p[dim]],dim,pin);
					pinLocate[0]=-p[dim];

					for(i=1;i<=dim;i++){
						pout[i]=rsl(pointV[p[1]],pointV[p[2]],point[p[1]][i],point[p[2]][i],v);
					}
					poutLocate[0]=e[1];
				}
			}
			else{
				if(e12<0){
					if(pinLocate[0]==0){
						for(i=1;i<=dim;i++){
							pin[i]=rsl(pointV[p[1]],pointV[p[2]],point[p[1]][i],point[p[2]][i],v);
						}
						pinLocate[0]=e[1];
					}
					else{
						for(i=1;i<=dim;i++){
							pout[i]=rsl(pointV[p[1]],pointV[p[2]],point[p[1]][i],point[p[2]][i],v);
						}
						poutLocate[0]=e[1];
					}					
				}

				if(e23<0){
					if(pinLocate[0]==0){
						for(i=1;i<=3;i++){
							pin[i]=rsl(pointV[p[3]],pointV[p[2]],point[p[3]][i],point[p[2]][i],v);
						}
						pinLocate[0]=e[2];
					}
					else{
						for(i=1;i<=3;i++){
							pout[i]=rsl(pointV[p[3]],pointV[p[2]],point[p[3]][i],point[p[2]][i],v);
						}
						poutLocate[0]=e[2];
					}					
				}

				if(e31<0){
					if(pinLocate[0]==0){
						for(i=1;i<=3;i++){
							pin[i]=rsl(pointV[p[1]],pointV[p[3]],point[p[1]][i],point[p[3]][i],v);
						}
						pinLocate[0]=e[3];
					}
					else{
						for(i=1;i<=3;i++){
							pout[i]=rsl(pointV[p[1]],pointV[p[3]],point[p[1]][i],point[p[3]][i],v);
						}
						poutLocate[0]=e[3];
					}					
				}

			}
		}
	}
}








void triangleMesh::findContour(float v)
{
	long i,j,k,nd;
	float pin[4];
	float pout[4];
	float ptemp[4];
	long pinLocate,poutLocate,temp;

	long currentPlocate=0,prevPlocate,startLocate;

	std::vector< std::vector<float> > contourp(dim,std::vector<float>(0));

	std::vector<long> nextfacelist;

	std::vector< std::vector<float> > nextp(dim,std::vector<float>(0));

	nd=0;

	bool *faceChecked=vector1<bool>(1,nface);

	bool lost;

	for(i=1;i<=nface;i++){
		if(faceChecked[i]){
			findContourInOneTriangle(i,v,pin,&pinLocate,pout,&poutLocate);
			faceChecked[i]=false;

			startLocate=pinLocate;
			while( (pinLocate!=0) && (poutLocate!=0) && (poutLocate!=startLocate) ){
				if(contourp[0].empty()){
					for(j=1;j<=dim;j++){
						contourp[j-1].push_back(pin[j]);
					}				
				}
				for(j=1;j<=dim;j++){				
					contourp[j-1].push_back(pout[j]);
				}

				prevPlocate=pinLocate;
				currentPlocate=poutLocate;


				nextfacelist.clear();
				if(poutLocate>0){
					nextfacelist.push_back(0);
					nextfacelist.push_back(edgeFace[poutLocate][1]);
					if(edgeFace[poutLocate][2]>0){
						nextfacelist.push_back(edgeFace[poutLocate][2]);
					}
				}
				else{
					nextfacelist=point2face[-poutLocate];
				}

				//for(j=0;j<3;j++){
				//	nextp[j].clear();
				//}

				lost=true;

				for(j=1;j<nextfacelist.size();j++){

					if(faceChecked[nextfacelist[j]]){
						findContourInOneTriangle(nextfacelist[j],v,pin,&pinLocate,pout,&poutLocate);
						faceChecked[nextfacelist[j]]=false;

						if( (pinLocate!=0) && (poutLocate!=0) ){//we get two point
							//check direction and reverse it if needed;
							if( currentPlocate==poutLocate ){
								copyvt(pin,dim,ptemp);
								copyvt(pout,dim,pin);
								copyvt(ptemp,dim,pout);
								temp=pinLocate;
								pinLocate=poutLocate;
								poutLocate=temp;
							}
							lost=false;
							break;

						}
					}


				}

				if(lost)
					break;

				if(poutLocate==startLocate){
					for(j=1;j<=dim;j++){
						contourp[j-1].push_back(pout[j]);
					}
				}


			}





			//print contour
			if(!contourp[0].empty()){
				//for(k=0;k<contourp[0].size();k++){
				//	for(j=0;j<3;j++){
				//		std::cout<<contourp[j][k]<<" ";
				//	}
				//	std::cout<<"\n";
				//}
				//std::cout<<"\n";


				contourv.push_back(contourp);
				contourValue.push_back(v);
				for(j=0;j<dim;j++){
					contourp[j].clear();
				}
			}


		}
	}






}


long triangleMesh::isBelongToTriangle(long * edget, long * facet)
{
	long location=0;

	long edgev1=0,edgev2=0;
	long i;
	for(i=1;i<=dim;i++){
		if(edget[0]==facet[i])
			edgev1=i;
		else 
			if(edget[1]==facet[i])
				edgev2=i;
	}

	if( ((edgev1==1)&&(edgev2==2)) || ((edgev1==2)&&(edgev2==1)) ){
		location=1;
	}
	else{
		if( ((edgev1==1)&&(edgev2==3)) || ((edgev1==3)&&(edgev2==1)) ){
			location=3;
		}
		else{
			if( ((edgev1==2)&&(edgev2==3)) || ((edgev1==3)&&(edgev2==2)) ){
				location=2;
			}
		}
	}

	return location;
}


void triangleMesh::genContourMap(long contourNumber)
{
	contourv.clear();
	contourValue.clear();

	float cvalue;
	float vstep=(maxPointValue-minPointValue)/(float)(contourNumber+1);

	for(cvalue=minPointValue;cvalue<=maxPointValue;cvalue+=vstep){
		findContour(cvalue);
	}

}


void triangleMesh::genFaceCentroid(void)
{
	float temp;

	long i,j,k;
	for(i=1;i<=nface;i++){

		for(k=1;k<=dim;k++){
			temp=0;
			for(j=1;j<=dim;j++){
				temp+=point[face[i][j]][k];
			}
			faceCentroid[i][k]=temp/dim;
		}

	}

}


// interpolate point value 
void triangleMesh::interpPointV(void)
{
	////pointValueInterp=vector<float>(1,npoint);
	//long i;
	//long j,k;
	//long nneighbor;
	//float **pointAround;
	//float *pointVAround;

		//for(i=1;i<=npoint;i++){

	//	nneighbor=point2face[i].size();
	//	nneighbor-=1;
	//	pointAround=matrix<float>(1,nneighbor,1,dim);
	//	pointVAround=vector<float>(1,nneighbor);

	//	for(j=1;j<=nneighbor;j++){
	//		for(k=1;k<=dim;k++){
	//			pointAround[j][k]=faceCentroid[point2face[i][j]][k];
	//		}
	//		pointVAround[j]=faceValue[point2face[i][j]];
	//	}

	//	pointValueInterp[i]=bhsint(pointAround,nneighbor,2,pointVAround,point[i]);

	//	free_matrix(pointAround,1,nneighbor,1,dim);
	//	free_vector(pointVAround,1,nneighbor);

	//}



	bhsint(faceCentroid,nface,2,faceValue,point,npoint,pointValueInterp);
	maxPointValueInterp=findmax(pointValueInterp,npoint);
	minPointValueInterp=findmin(pointValueInterp,npoint);
}
