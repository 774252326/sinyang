

#include <fstream>
//#include <vector>
#include <iostream>
#include "funT\nrutilT.h"
using namespace std;

void readf2(char *fn, float **dt, long r, long c){

	ifstream ifs(fn);
	if(!ifs.bad()){
		for(long i=1;i<=r;i++){
			for(long j=1;j<=c;j++){
				ifs>>dt[i][j];
			}
		}
		ifs.close();
	}

}

void readf3(wchar_t *fn, float ***dt, long r, long c, long h){

	ifstream ifs(fn);
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

void readf3(wchar_t *fn, float ***pt, float *color, long r, long c, long h){

	wifstream ifs(fn);
	if(!ifs.bad()){
		for(long i=1;i<=r;i++){
			//read x,y,z
			for(long j=1;j<=c;j++){
				for(long k=1;k<=h;k++){
					ifs>>pt[i][k][j];
				}
			}

			//read value
			ifs>>color[i];

		}
		ifs.close();
	}
}

template <typename T>
void pdt(T ***dt, long r, long c, long h){
	for(long i=1;i<=r;i++){
		for(long j=1;j<=c;j++){
			for(long k=1;k<=h;k++){
				cout<<dt[i][j][k]<<',';
			}
			cout<<'\n';
		}
		cout<<'\n';
		cout<<'\n';
	}
}

void pdt(float ***dt, long r, long c){
	for(long i=1;i<=r;i++){
		for(long j=1;j<=c;j++){
			cout<<dt[i][j]<<',';
		}
		cout<<'\n';
	}
}


void selectNormal(float normal[3], float point[3], float inner[3], bool outflag){
	float s;
	s=normal[0]*(point[0]-inner[0])+normal[1]*(point[1]-inner[1])+normal[2]*(point[2]-inner[2]);
	if( (s>0 && !outflag)||(s<0 && outflag) ){
		normal[0]=-normal[0];
		normal[1]=-normal[1];
		normal[2]=-normal[2];
	}
}


bool uniVector(float v[3]){
	float s=v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
	if(s<=0)
		return false;
	else{
		s=sqrt(s);
		v[0]/=s;
		v[1]/=s;
		v[2]/=s;
		return true;
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

short light0=GL_LIGHT0;
float ambient0[4]={0,0,0,1};
float diffuse0[4]={1,1,1,1};
float specular0[4]={0,0,0,1};
float position0[4]={0,0,1,0};
float direction0[3]={0,0,-1};
float cutoff0=180;


// turn on one light
void lighting(short light=light0, float *ambient=ambient0, float *diffuse=diffuse0, float *specular=specular0, float *position=position0, float *direction=direction0, float cutoff=cutoff0)
{
	glLightfv(light, GL_AMBIENT, ambient);
	glLightfv(light, GL_DIFFUSE, diffuse);
	glLightfv(light, GL_SPECULAR, specular);

	if(position[3]==0){
		glLightfv(light, GL_POSITION, position); 
	}
	else{
		if(position[3]==1){
			glLightfv(light, GL_POSITION, position); 
			glLightfv(light, GL_SPOT_DIRECTION, direction);
			glLightf(light, GL_SPOT_CUTOFF, cutoff);

		}
		else{
			cout<<"error";
		}
	}
	glEnable(light);
}



//read matrix from file
template <typename T>
T **readf2(wchar_t *fn, long *r, long c){
	wifstream ifs(fn);
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
					dt=dtt;					
				}
				else{
					for(j=1;j<=c;j++){
						ifs>>dt[r[0]+1][j];
						//cout<<dt[r[0]+1][j];
					}					
				}
				r[0]+=1;
			}
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




//generate face list and point list and face color list from matrix
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
