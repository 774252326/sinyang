#include "stdafx.h"
#include "colormap.h"
#include <math.h>

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



COLORREF genColor(float colorv){
	float hsv[3]={colorv,1,1};
	if(colorv>1 || colorv<0)
		hsv[0]=1;

	float rgb[3];
	hsv2rgb(hsv,rgb);

	return RGB(rgb[0]*255,rgb[1]*255,rgb[2]*255);
}


float genColorvFromIndex(unsigned long idx){
	div_t dr;
	dr = div(idx,3);
	unsigned long a=dr.quot;
	unsigned long b=1;
	unsigned long c=0;
	unsigned long d=1;

	while(a){
		c<<=1;
		c|=(a&b);
		a>>=1;
		d<<=1;
	}
	return ((float)dr.rem+(float)c/(float)d)/3.0;
}
