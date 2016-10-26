#ifndef CALGRIDT_H
#define CALGRIDT_H

#include <math.h>



//compute axis grid interval 1E(x) for given length d. return integer x.
template <typename T>
int calgrid(const T d)
{
	const double lg2=0.30102999566398119521373889472449;
	T lgd=log10(d);
	int gridx=floor(lgd);
	if( gridx+lg2>lgd ){
		gridx--;
	}
	return gridx;
}

#endif