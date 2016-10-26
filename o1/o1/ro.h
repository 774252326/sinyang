#pragma once

#include <fstream>
//#include <vector>
#include <iostream>
#include "..\..\funT\nrutilT.h"
#include "..\..\funT\paddingT.h"
#include "..\..\funT\findmT.h"
//using namespace std;

//void readf2(char *fn, float **dt, long r, long c){
//
//	ifstream ifs(fn);
//	if(!ifs.bad()){
//		for(long i=1;i<=r;i++){
//			for(long j=1;j<=c;j++){
//				ifs>>dt[i][j];
//				cout<<dt[i][j];
//				
//			}
//
//			if(ifs.eof()){
//					cout<<"end\n";
//					break;
//					break;
//				}
//		}
//		ifs.close();
//	}
//
//}


template <typename T>
T **readf2(char *fn, long *r, long c){
	std::ifstream ifs(fn);
	T **dt;
	T **dtt;
	long nd;

	long i,j;

	r[0]=0;
	if(ifs.bad()){
		return NULL;
	}
	else{

		while(!ifs.eof()){
			if(r[0]==0){
				nd=1;
				dt=matrix<T>(1,nd,1,c);
				for(j=1;j<=c;j++){
					ifs>>dt[1][j];
					//cout<<dt[1][j];

				}
				r[0]=1;
			}
			else{
				if(r[0]+1>nd){

					dtt=matrix<T>(1,nd*2,1,c);
					copymx(dt,nd,c,dtt);
					nd*=2;
					for(j=1;j<=c;j++){
						ifs>>dtt[r[0]+1][j];
						//cout<<dtt[r[0]+1][j];
					}
					free_matrix(dt,1,r[0],1,c);
					std::cout<<"free\n";
					dt=dtt;
					r[0]+=1;
				}
				else{
					for(j=1;j<=c;j++){
						ifs>>dt[r[0]+1][j];
						//cout<<dt[r[0]+1][j];
					}
					r[0]+=1;
				}
			}
			//		if(ifs.eof()){
			//	cout<<"end\n";
			//	break;
			//}




		}

		if(r[0]>0){
			dtt=matrix<T>(1,r[0],1,c);
			copymx(dt,r[0],c,dtt);
			free_matrix(dt,1,nd,1,c);
			return dtt;
		}
		else{
			return NULL;
		}
	}
}


template <typename T>
T **genPFlist(T **mat, long r, long **flist, T *facecolor, long *np){

	T **temp;
	temp=matrix<T>(1,r*3,1,5);
	np[0]=0;
	T pnow[4];
	long pnowidx;
	long i,j,k;
	for(i=1;i<=r;i++){
		pnow[0]=mat[i][10];
		for(j=1;j<=3;j++){

			
			pnow[1]=mat[i][j];
			pnow[2]=mat[i][j+3];
			pnow[3]=mat[i][j+6];

			//if(np[0]==0){
			//	for(k=1;k<=3;k++){
			//		temp[1][k]=pnow[k];
			//	}
			//	temp[1][4]=pnow[0];
			//	temp[1][5]=1;
			//	np[0]+=1;
			//	pnowidx=1;
			//}
			//else{
				pnowidx=0;
				for(k=1;k<=np[0];k++){
					if( (pnow[1]==temp[k][1]) && (pnow[2]==temp[k][2]) && (pnow[3]==temp[k][3]) ){
						pnowidx=k;
						temp[k][4]+=pnow[0];
						temp[k][5]+=1;
						break;					
					}
				}

				if(pnowidx==0){
					np[0]+=1;
					for(k=1;k<=3;k++){
						temp[np[0]][k]=pnow[k];
					}
						temp[np[0]][4]=pnow[0];
						temp[np[0]][5]=1;
					pnowidx=np[0];
				}
			//}

			flist[i][j]=pnowidx;
		}
		facecolor[i]=pnow[0];
	}


	T **plist=matrix<T>(1,np[0],1,4);

	copymx(temp,np[0],3,plist);

	for(i=1;i<=np[0];i++){
		plist[i][4]=temp[i][4]/temp[i][5];
	}


	free_matrix(temp,1,r*3,1,5);
	return plist;
}










void readf3(char *fn, float ***dt, long r, long c, long h){

	std::ifstream ifs(fn);
	if(!ifs.bad()){
		for(long i=1;i<=r;i++){
			for(long j=1;j<=c;j++){
				for(long k=1;k<=h;k++){				

					ifs>>dt[i][k][j];
				}
			}
		}
		ifs.close();
	}
}

//void readf3(wchar_t *fn, float ***dt, long r, long c, long h){
//
//	wifstream ifs(fn);
//	if(!ifs.bad()){
//		for(long i=1;i<=r;i++){
//			for(long j=1;j<=c;j++){
//				for(long k=1;k<=h;k++){				
//
//					ifs>>dt[i][k][j];
//
//					cout<<dt[i][k][j]<<'\n';
//				}
//			}
//		}
//		ifs.close();
//	}
//}


void readf3(wchar_t *fn, float ***dt, long r, long c, long h){

	std::ifstream ifs(fn);
	if(!ifs.bad()){
		for(long i=1;i<=r;i++){
			for(long j=1;j<=c;j++){
				for(long k=1;k<=h;k++){				

					ifs>>dt[i][j][k];
				}
			}
		}
		ifs.close();
	}
}

template <typename T>
void pdt(T ***dt, long r, long c, long h){
	for(long i=1;i<=r;i++){
		for(long j=1;j<=c;j++){
			for(long k=1;k<=h;k++){
				std::cout<<dt[i][j][k]<<',';
			}
			std::cout<<'\n';
		}
		std::cout<<'\n';
		std::cout<<'\n';
	}
}

template <typename T>
void pdt(T **dt, long r, long c){
	for(long i=1;i<=r;i++){
		for(long j=1;j<=c;j++){
			std::cout<<dt[i][j]<<',';
		}
		std::cout<<'\n';
	}
}


template <typename T>
void pdt(T *dt, long r){
	for(long i=1;i<=r;i++){
		std::cout<<dt[i]<<',';	
	}
}

void hsv2rgb(float hsv[3], float rgb[3]){

	//if(hsv[2]==0){
	//	rgb[0]=rgb[1]=rgb[2]=0;
	//}
	//else{
	rgb[0]=rgb[1]=rgb[2]=hsv[2];
	if(hsv[1]!=0){


		float h,p,t,n;
		int k;
		h=hsv[0]*6;
		k=floor(h);
		p=h-k;
		t=1-hsv[1];
		n=1-hsv[1]*p;
		p=1-hsv[1]*(1-p);

		switch(k){
		case 0:
		case 6:
			rgb[0]*=1;
			rgb[1]*=p;
			rgb[2]*=t;
			break;
		case 1:
			rgb[0]*=n;
			rgb[1]*=1;
			rgb[2]*=t;
			break;
		case 2:
			rgb[0]*=t;
			rgb[1]*=1;
			rgb[2]*=p;
			break;
		case 3:
			rgb[0]*=t;
			rgb[1]*=n;
			rgb[2]*=1;
			break;
		case 4:
			rgb[0]*=p;
			rgb[1]*=t;
			rgb[2]*=1;
			break;
		case 5:
			rgb[0]*=1;
			rgb[1]*=t;
			rgb[2]*=n;
			break;
		default:
			break;
		}
	}

	//}
}



void genColor(float rgbcolor[4], float colorv){

	float hsv[3]={colorv,1,1};
	if(colorv>1 || colorv<0)
		hsv[0]=1;

	float rgb[3];
	hsv2rgb(hsv,rgb);
	rgbcolor[0]=rgb[0];
	rgbcolor[1]=rgb[1];
	rgbcolor[2]=rgb[2];
	rgbcolor[3]=1;
}