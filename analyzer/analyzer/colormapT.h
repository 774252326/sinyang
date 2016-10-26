#ifndef COLORMAPT_H
#define COLORMAPT_H

//color
#define black RGB(0,0,0)
#define red RGB(255,0,0)
#define green RGB(0,255,0)
#define blue RGB(0,0,255)
#define yellow RGB(255,255,0)
#define cyan RGB(0,255,255)
#define magenta RGB(255,0,255)
#define white RGB(255,255,255)

#include <math.h>


template <typename T, typename TT>
void hsv2rgb(T hsv[3], TT rgb[3]){
	
	rgb[0]=rgb[1]=rgb[2]=hsv[2];
	if(hsv[1]!=0){
		TT h,p,t,n;
		int k;
		h=hsv[0]*6;
		k=(int)floor(h);
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


}


template <typename T, typename TT>
void genColor(T rgbcolor[4], TT colorv){
	TT hsv[3]={colorv,1,1};
	if(colorv>1 || colorv<0)
		hsv[0]=1;
	hsv2rgb(hsv,rgbcolor);
	rgbcolor[3]=1;
}


template <typename T>
COLORREF genColor(T colorv){
	T hsv[3]={colorv,1,1};
	if(colorv>1 || colorv<0)
		hsv[0]=1;

	T rgb[3];
	hsv2rgb(hsv,rgb);

	return RGB(rgb[0]*255,rgb[1]*255,rgb[2]*255);
}

template <typename T>
T genColorvFromIndex(unsigned long idx){
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
	return ((T)dr.rem+(T)c/(T)d)/((T)3);
}


template <typename T>
COLORREF genColorGray(T colorv){
	//T hsv[3]={colorv,1,1};
	//if(colorv>1 || colorv<0)
	//	hsv[0]=1;

	//T rgb[3];
	//hsv2rgb(hsv,rgb);

	int gray=colorv*255;

	return RGB(gray,gray,gray);
}

#endif