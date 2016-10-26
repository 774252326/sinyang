void bcucof(float y[], float y1[], float y2[], float y12[], float d1, float d2, float **c)
	//Given arrays y[1..4], y1[1..4], y2[1..4], and y12[1..4], containing the function, gradients,
	//and cross derivative at the four grid points of a rectangular grid cell (numbered counterclockwise
	//from the lower left), and given d1 and d2, the length of the grid cell in the 1- and
	//2-directions, this routine returns the table c[1..4][1..4] that is used by routine bcuint
	//for bicubic interpolation.
{
	static int wt[16][16]=
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	-3,0,0,3,0,0,0,0,-2,0,0,-1,0,0,0,0,
	2,0,0,-2,0,0,0,0,1,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
	0,0,0,0,-3,0,0,3,0,0,0,0,-2,0,0,-1,
	0,0,0,0,2,0,0,-2,0,0,0,0,1,0,0,1,
	-3,3,0,0,-2,-1,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,-3,3,0,0,-2,-1,0,0,
	9,-9,9,-9,6,3,-3,-6,6,-6,-3,3,4,2,1,2,
	-6,6,-6,6,-4,-2,2,4,-3,3,3,-3,-2,-1,-1,-2,
	2,-2,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,-2,0,0,1,1,0,0,
	-6,6,-6,6,-3,-3,3,3,-4,4,2,-2,-2,-2,-1,-1,
	4,-4,4,-4,2,2,-2,-2,2,-2,-2,2,1,1,1,1};
	int l,k,j,i;
	float xx,d1d2,cl[16],x[16];
	d1d2=d1*d2;
	for (i=1;i<=4;i++) { /*Pack a temporary vector x.*/
		x[i-1]=y[i];
		x[i+3]=y1[i]*d1;
		x[i+7]=y2[i]*d2;
		x[i+11]=y12[i]*d1d2;
	}
	for (i=0;i<=15;i++) { /*Matrix multiply by the stored table.*/
		xx=0.0;
		for (k=0;k<=15;k++) xx += wt[i][k]*x[k];
		cl[i]=xx;
	}
	l=0;
	for (i=1;i<=4;i++) /*Unpack the result into the output table.*/
		for (j=1;j<=4;j++) c[i][j]=cl[l++];
}