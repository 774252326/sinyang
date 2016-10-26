#ifndef BANMATIND_H
#define BANMATIND_H

#include "nrutilT.h"

index m2mp(index mi, long r, long c, long a, long b){
	if( mi.ri<1 || mi.ri>r || mi.ci<1 || mi.ci>c || mi.ri-mi.ci<a || mi.ri-mi.ci>b ){
		nrerror("out of range");
	}
	else{
		index mpi;
		mpi.ri=MIN(mi.ri,mi.ci);
		mpi.ci=mi.ri-mi.ci-a+1;
		return mpi;
	}
}

index mp2m(index mpi, long r, long c, long a, long b){
	index mi;
	if(r>=c){
		mi.ci=mpi.ri;
		mi.ri=mpi.ci+mpi.ri+a-1;
	}
	else{
		mi.ri=mpi.ri;
		mi.ci=mpi.ri-mpi.ci-a+1;
	}
	if( mi.ri<1 || mi.ri>r || mi.ci<1 || mi.ci>c || (mi.ri-mi.ci)<a || (mi.ri-mi.ci)>b ){
		nrerror("out of range");
	}
	else{
		return mi;
	}
}




long calupbw(long r1, long c1, long a1, long r2, long c2, long a2, long r3, long c3){
	//compute the upper band width for product matrix



	//long tmpup;
	//long a3;
	//if(r1-c1>0){
	//	tmpup=a1+r1-c1;
	//	//tmpdn=-b1;
	//}
	//else{
	//	tmpup=a1;
	//	//tmpdn=r1-c1-b1;
	//}

	//if(r2-c2>0){
	//	tmpup+=a2+r2-c2;
	//	//tmpdn+=-b2;
	//}
	//else{
	//	tmpup+=a2;
	//	//tmpdn+=r2-c2-b2;
	//}

	//if(r3-c3>0){
	//	a3=tmpup-r3+c3;
	//	//b3=-tmpdn;
	//}
	//else{
	//	a3=tmpup;
	//	//b3=r3-c3-tmpdn;
	//}


	long a3=a1+a2;
	if(r1>c1)
		a3+=r1-c1;
	if(r2>c2)
		a3+=r2-c2;
	if(r3>c3)
		a3-=r3-c3;
	return a3;
}

long caldnbw(long r1, long c1, long b1, long r2, long c2, long b2, long r3, long c3){
	//long tmpdn;
	//long b3;
	//if(r1-c1>0){
	//	//tmpup=a1+r1-c1;
	//	tmpdn=-b1;
	//}
	//else{
	//	//tmpup=a1;
	//	tmpdn=r1-c1-b1;
	//}

	//if(r2-c2>0){
	//	//tmpup+=a2+r2-c2;
	//	tmpdn+=-b2;
	//}
	//else{
	//	//tmpup+=a2;
	//	tmpdn+=r2-c2-b2;
	//}

	//if(r3-c3>0){
	//	//a3=tmpup-r3+c3;
	//	b3=-tmpdn;
	//}
	//else{
	//	//a3=tmpup;
	//	b3=r3-c3-tmpdn;
	//}

	//compute the lower band width for product matrix

	long b3=b1+b2;
	if(r1<=c1)
		b3-=r1-c1;
	if(r2<=c2)
		b3-=r2-c2;
	if(r3<=c3)
		b3+=r3-c3;
	return b3;
}

#endif