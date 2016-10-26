#ifndef FITRESULTT_H
#define FITRESULTT_H

template <typename T>
void fitresult( T x[], long ndat, T a[], long ma, T yfit[], void (*funcs)(T, T [], long)){
	T *afunc;
	long i,j;

	afunc=vector<T>(1,ma);

	for (i=1;i<=ndat;i++) { 
		(*funcs)(x[i],afunc,ma);
		yfit[i]=0;
		//printf("%f\n",x[i]);
		//printvt(a,1,ma);
		//printvt(afunc,1,ma);
		for (j=1;j<=ma;j++){
			yfit[i]+=afunc[j]*a[j];
		}
	}

	free_vector(afunc,1,ma);

}

#endif
