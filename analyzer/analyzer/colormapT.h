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


template <typename T>
void GetrgbT(COLORREF ic, T rgb[3]){
	BYTE b=ic>>16;
	BYTE g=ic>>8;
	BYTE r=ic;
	rgb[0]=r;
	rgb[1]=g;
	rgb[2]=b;

	rgb[0]/=255;
	rgb[1]/=255;
	rgb[2]/=255;

}


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
void rgb2hsv(T hsv[3], TT rgb[3])
{
     // r,g,b values are from 0 to 1
    // h = [0,360], s = [0,1], v = [0,1]
    // if s == 0, then h = -1 (undefined)
    TT delta,min,max;

	if(rgb[0]>rgb[1]){
		min=rgb[1];
		max=rgb[0];
	}
	else{
		max=rgb[1];
		min=rgb[0];
	}
	max=(rgb[2]>max)?rgb[2]:max;
	min=(rgb[2]<min)?rgb[2]:min;


    hsv[2] = max; // v
	delta = max - min;
    if( max == 0 || delta == 0 ){
    	// r = g = b = 0 // s = 0, v is undefined
		hsv[1] = 0;
		hsv[0] = 0;
    	return;
    }	
	hsv[1] = delta / max; // s

	if(rgb[0] == max){
		if (rgb[1] >= rgb[2])
        	hsv[0] = (rgb[1] - rgb[2]) / delta; // between yellow & magenta
		else
			hsv[0] = (rgb[1] - rgb[2]) / delta + 6;
	}
	else if( rgb[1] == max )
        hsv[0] = 2 + ( rgb[2] - rgb[0] ) / delta; // between cyan & yellow
	else if (rgb[2] == max)
        hsv[0] = 4 + ( rgb[0] - rgb[1] ) / delta; // between magenta & cyan
    hsv[0] /= 6; // degrees
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
	int gray=colorv*255;
	return RGB(gray,gray,gray);
}

#endif