#include "stdafx.h"
#include <math.h>

const float lg2=0.30102999566398119521373889472449;

//compute axis grid interval 1E(x) for given length d. return integer x.

int calgrid(float d)
{
	float lgd=log10(d);
	int gridx=floor(lgd);
	if( gridx+lg2>lgd ){
		gridx--;
	}
	return gridx;
}