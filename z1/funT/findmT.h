#ifndef FINDMT_H
#define FINDMT_H

template <typename T>
T findmax(T * x, long n)
{
	T xmax=x[1];
	long i;
	for(i=2; i<=n; i++){
		if(x[i]>xmax)
		xmax=x[i];
	}
	return xmax;
}


template <typename T>
T findmin(T * x, long n)
{
	T xmin=x[1];
	long i;
	for(i=2; i<=n; i++){
		
		if(x[i]<xmin)
			xmin=x[i];
	}
	return xmin;
}

template <typename T>
long maxind( T x[], long ndat){
	long i;
	long mi=1;
	T tmp=x[1];
	for (i=1; i<=ndat; i++){
		if (tmp<x[i]) {
			mi=i;
			tmp=x[i];
		}
	}
	return mi;
}


template <typename T>
long minind( T x[], long ndat){
	long i;
	long mi=1;
	T tmp=x[1];
	for (i=1; i<=ndat; i++){
		if (tmp>x[i]) {
			mi=i;
			tmp=x[i];
		}
	}
	return mi;
}



template <typename T>
long findbottomidx( T * x, long nd, T xbottom)
	//return index of first element larger than xbottom
{
	long i;
	for( i=1; i<=nd; i++){
		if(x[i]>=xbottom)
			break;
	}
	return i;
}


template <typename T>
long findtopidx( T * x, long nd, T xtop )
	//return index of last element smaller than xtop
{
	long i;
	for( i=nd; i>=1; i-- ){
		if(x[i]<=xtop)
			break;
	}
	return i;
}


template <typename T>
void norvt(T *x, T *y, long nd, T *nx, T *ny){
	T xmin=findmin(x,nd);
	T xmax=findmax(x,nd);
	T ymin=findmin(y,nd);
	T ymax=findmax(y,nd);

	long i;
	for(i=1;i<=nd;i++){
		nx[i]=(x[i]-xmin)/(xmax-xmin);
		ny[i]=(y[i]-ymin)/(ymax-ymin);
	}

}

template <typename T>
long findlmx( T *x, long nd, bool *bx){
	long i;
	long nmin=0;
	bx[1]=false;
	for( i=2; i<=nd-1; i++ ){
		if( x[i-1]<x[i] && x[i]>x[i+1] ){
			bx[i]=true;
			nmin++;
		}
		else{
			bx[i]=false;
		}
	}
	bx[nd]=false;
	return nmin;
}


template <typename T>
long findlmn( T *x, long nd, bool *bx){
	long i;
	long nmin=0;
	bx[1]=false;

	for( i=2; i<=nd-1; i++ ){
		if( x[i-1]>x[i] && x[i]<x[i+1] ){
			bx[i]=true;
			nmin++;
		}
		else{
			bx[i]=false;
		}
	}

	bx[nd]=false;

	return nmin;
}

template <typename T>
void selectvt1( T *x, long nd, bool * bx, long nmin, T *xlmn ){

	long i;
	long j=1;

	for( i=1; i<=nd; i++ ){
		if( bx[i]){
			xlmn[j]=x[i];
			//ylmn[j]=y[i];
			j++;
		}
	}
}


template <typename T>
void selectvt2( T *x, T *y, long nd, bool * bx, long nmin, T *xlmn, T *ylmn ){

	long i;
	long j=1;

	for( i=1; i<=nd; i++ ){
		if( bx[i]){
			xlmn[j]=x[i];
			ylmn[j]=y[i];
			j++;
		}
	}
}


#endif