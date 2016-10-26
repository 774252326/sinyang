#pragma once
#include <vector>
#include <Windows.h>
//#include <WinGDI.h>
//#include <math.h>
//#include <cmath>
#include "roundT.h"

typedef struct
{
	COLORREF oc;
	float v;
} ColorNode;

class ColorMap	
{



protected:
	std::vector<ColorNode> cmap;
public:	

	ColorMap(COLORREF c0, COLORREF c1)
	{
		ColorNode ci;
		ci.oc=c0;
		ci.v=0;

		cmap.assign(2,ci);

		////ci.oc=c1;
		////ci.v=1;
		cmap[1].oc=c1;
		cmap[1].v=1;
	};

	~ColorMap(){};
	

	bool AddNode(COLORREF c, float v)
	{
		if(v>cmap.back().v || v<cmap.front().v)
			return false;
		std::vector<ColorNode>::iterator it=cmap.begin();
		  while ( it != cmap.end() && (*it).v<v )
		  {
			  it++;
		  }

		  ColorNode cn;
		  cn.oc=c;
		  cn.v=v;

		  cmap.insert(it,cn);
		  return true;
	};



	COLORREF GetColor(float v)
	{
		if(v<=cmap.front().v)
			return cmap.front().oc;

		if(v>=cmap.back().v)
			return cmap.back().oc;

		

		for(size_t i=1;i<cmap.size();i++){
			if(v<cmap[i].v){
				float dv=(cmap[i].v-v)/(cmap[i].v-cmap[i-1].v);
				
				float r1=GetRValue(cmap[i].oc);
				float r0=GetRValue(cmap[i-1].oc);
				float r2=r0*dv+r1*(1-dv);
				BYTE r=round(r2);

								float g1=GetGValue(cmap[i].oc);
				float g0=GetGValue(cmap[i-1].oc);
				float g2=g0*dv+g1*(1-dv);
				BYTE g=round(g2);

								float b1=GetBValue(cmap[i].oc);
				float b0=GetBValue(cmap[i-1].oc);
				float b2=b0*dv+b1*(1-dv);
				BYTE b=round(b2);

				return RGB(r,g,b);
			}
		}


		return 0;

	};



};