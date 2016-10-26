#ifndef BANMATT_H
#define BANMATT_H

#include "nrutilT.h"
//#include "nerror.h"

typedef struct INDEX {long ri; long ci;} index;



template <typename T>
T **bmatrix(long nr, long nc, long m1, long m2){
	T **m;
	if( nc>=nr ){
		m=matrix<T>(1,nr,1,m1+m2+nc-nr+1);
	}
	else{
		m=matrix<T>(1,nc,1,m1+m2+nr-nc+1);
	}
	return m;
}

template <typename T>
void free_bmatrix( T **m, long nr, long nc, long m1, long m2){
	if( nc>=nr ){
		free_matrix(m,1,nr,1,m1+m2+nc-nr+1);
	}
	else{
		free_matrix(m,1,nc,1,m1+m2+nr-nc+1);
	}
}

template <typename T>
index m2mp(index mi, T r, T c, T a, T b){//T for int&long only 
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

template <typename T>
index mp2m(index mpi, T r, T c, T a, T b){//T for int&long only 
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


template <typename T>
T getbmvalue(T **m, long nr, long nc, long m1, long m2, long ri, long ci, bool *zeroflag){
	//return m[ri][ci] of band diagonal matrix m[1..nr][1..nc], if index (ri,ci) outside diagonal band, zeroflag=true
	if( ri<1 || ri>nr || ci<1 || ci>nc ){
		nrerror("out of range");
		return 0;
	}
	//else{
		T value;
		if( nc>=nr ){
			if( nr-nc-m2<=ri-ci && ri-ci<=m1 ){
				value=m[ri][m1+1+ci-ri];
				zeroflag[0]=false;
			}
			else{
				value=0;
				zeroflag[0]=true;
			}
		}
		else{
			if( -m2<=ri-ci && ri-ci<=nr-nc+m1 ){
				value=m[ci][m1+nr-nc+1+ci-ri];
				zeroflag[0]=false;
			}
			else{
				value=0;
				zeroflag[0]=true;
			}
		}
		return value;
	//}

}


template <typename T>
bool setbmvalue(T **m, long nr, long nc, long m1, long m2, long ri, long ci, T value){
	//set the value of m[ri][ci] of band diagonal matrix m[1..nr][1..nc], if index (ri,ci) outside diagonal band, zeroflag=true
	if( ri<1 || ri>nr || ci<1 || ci>nc ){
		nrerror("out of range");
	}
	//else{
		//T value;
		bool zeroflag;
		if( nc>=nr ){
			if( nr-nc-m2<=ri-ci && ri-ci<=m1 ){
				m[ri][m1+1+ci-ri]=value;
				zeroflag=false;
			}
			else{
				zeroflag=true;
			}
		}
		else{
			if( -m2<=ri-ci && ri-ci<=nr-nc+m1 ){
				m[ci][m1+nr-nc+1+ci-ri]=value;
				zeroflag=false;
			}
			else{
				zeroflag=true;
			}
		}
		return zeroflag;
	//}

}


template <typename T>
void bmattp( T **mi, long nr, long nc, long m1, long m2, T **mo ){
	//transpose band diagonal matrix
	long i,j,ki,kj;
	long iup;
	if(nc>nr){
		for(i=1;i<=nr;i++){
			for(j=1;j<=m1+m2+nc-nr+1;j++){
				mo[i][j]=mi[i][m1+m2+nc-nr+1-j+1];
			}
		}
	}
	else{
		if(nc<nr){
			for(i=1;i<=nc;i++){
				for(j=1;j<=m1+m2+nr-nc+1;j++){
					mo[i][j]=mi[i][m1+m2+nr-nc+1-j+1];
				}
			}
		}
		else{
			for(j=1;j<=m1+m2+1;j++){
				iup=MIN(nr,nr+m2+1-j);
				for(i=1;i<=iup;i++){

					ki=i+j-m2-1;
					kj=m1+m2+1-j+1;
					//if(ki<=nr)
					mo[i][j]=mi[ki][kj];
				}
			}
		}
	}
}




template <typename T>
bool bmatmul(T **bm1, long r1, long c1, long a1, long b1, 
	T **bm2, long r2, long c2, long a2, long b2, 
	T **bm3, long r3, long c3, long a3, long b3){
		//band diagonal matrix multiply
		//mat3=mat1 x mat2
		if(c1!=r2)
			return false;
		else{
			long i,j,k;

			//compute upper and lower limits of each band diagonal matrix
			long up3,dn3;
			if( r3-c3>0 ){
				up3=a3+r3-c3;
				dn3=-b3;
			}
			else{
				up3=a3;
				dn3=r3-c3-b3;
			}

			long up1,dn1;
			if( r1-c1>0 ){
				up1=a1+r1-c1;
				dn1=-b1;
			}
			else{
				up1=a1;
				dn1=r1-c1-b1;
			}

			long up2,dn2;
			if( r2-c2>0 ){
				up2=a2+r2-c2;
				dn2=-b2;
			}
			else{
				up2=a2;
				dn2=r2-c2-b2;
			}
			


			long jup3,jdn3;
			long kup,kdn;

			T sum;
			bool fg;

			for( i=1; i<=r3; i++ ){

				jdn3=MAX(i-up3,(long)1);
				jup3=MIN(i-dn3,c3);

				for( j=jdn3; j<=jup3; j++ ){

					kup=MIN(MIN(i-dn1,j+up2),c1);
					kdn=MAX(MAX(i-up1,j+dn2),(long)1);

					sum=0;
					for( k=kdn; k<=kup; k++ ){
						sum+=getbmvalue(bm1,r1,c1,a1,b1,i,k,&fg)*getbmvalue(bm2,r2,c2,a2,b2,k,j,&fg);
					}
					fg=setbmvalue(bm3,r3,c3,a3,b3,i,j,sum);
				}
			}
			return true;
		}
}


template <typename T>
T calupbw(T r1, T c1, T a1, T r2, T c2, T a2, T r3, T c3){//T for int&long only 
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


	T a3=a1+a2;
	if(r1>c1)
		a3+=r1-c1;
	if(r2>c2)
		a3+=r2-c2;
	if(r3>c3)
		a3-=r3-c3;
	return a3;
}

template <typename T>
T caldnbw(T r1, T c1, T b1, T r2, T c2, T b2, T r3, T c3){//T for int&long only 
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

	T b3=b1+b2;
	if(r1<=c1)
		b3-=r1-c1;
	if(r2<=c2)
		b3-=r2-c2;
	if(r3<=c3)
		b3+=r3-c3;
	return b3;
}


template <typename T>
bool bmatadd(T **bm1, long r1, long c1, long a1, long b1, T p1,
	T **bm2, long a2, long b2, T p2, T **bm3, long a3, long b3){

		//band diagonal matrix weighted addition
			//bm3=p1*bm1+p2*bm2

		long i,j;
		long diagind;
		if(r1>c1){
			diagind=(r1-c1)+1;
			for(j=1;j<=c1;j++){
				for(i=1;i<=a3+b3+diagind;i++){
					bm3[j][i]=0;
				}
				for(i=1;i<=diagind;i++){

					bm3[j][a3+i]=p1*bm1[j][a1+i]+p2*bm2[j][a2+i];

				}
				for(i=0;i>=1-a1;i--){
					bm3[j][a3+i]+=p1*bm1[j][a1+i];
				}
				for(i=0;i>=1-a2;i--){
					bm3[j][a3+i]+=p2*bm2[j][a2+i];
				}
				for(i=1;i<=b1;i++){
					bm3[j][a3+diagind+i]+=p1*bm1[j][a1+diagind+i];
				}
				for(i=1;i<=b2;i++){
					bm3[j][a3+diagind+i]+=p2*bm2[j][a2+diagind+i];
				}
			}
		}
		else{
			diagind=(c1-r1)+1;
			for(j=1;j<=r1;j++){
				for(i=1;i<=a3+b3+diagind;i++){
					bm3[j][i]=0;
				}
				for(i=1;i<=diagind;i++){

					bm3[j][a3+i]=p1*bm1[j][a1+i]+p2*bm2[j][a2+i];

				}
				for(i=0;i>=1-a1;i--){
					bm3[j][a3+i]+=p1*bm1[j][a1+i];
				}
				for(i=0;i>=1-a2;i--){
					bm3[j][a3+i]+=p2*bm2[j][a2+i];
				}
				for(i=1;i<=b1;i++){
					bm3[j][a3+diagind+i]+=p1*bm1[j][a1+diagind+i];
				}
				for(i=1;i<=b2;i++){
					bm3[j][a3+diagind+i]+=p2*bm2[j][a2+diagind+i];
				}
			}
		}
		return true;
}





#endif