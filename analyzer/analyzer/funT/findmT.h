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
	for (i=2; i<=ndat; i++){
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
	for (i=2; i<=ndat; i++){
		if (tmp>x[i]) {
			mi=i;
			tmp=x[i];
			TRACE("x[%d]=%.20f\n",i,tmp);
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
void getlmn( T *x, T *y, long nd, bool * bx, long nmin, T *xlmn, T *ylmn ){

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
void getlmx( T *x, T *y, long nd, bool * bx, long nmin, T *xlmn, T *ylmn ){

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




template <typename T>
void norvt(T *x, T *y, long nd, T *nx, T *ny){
	//normalize vector
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
void restorenx(T **x, long nd, T xmin, T xmax, T *rex){
	long i;
	for(i=1;i<=nd;i++){
		rex[i]=x[i][1]*(xmax-xmin)+xmin;
	}
}

template <typename T>
void reversevt(T *x, long nd){
	long i;
	T tmp;
	for(i=1;i<=nd/2;i++){
		tmp=x[i];
		x[i]=x[nd+1-i];
		x[nd+1-i]=tmp;
	}
}

template <typename T>
void reversemx(T **x, long r, long c, bool rowflag){

	long i,j;
	T tmp;
	if(rowflag){/*reverse by row*/
		for(i=1;i<=r/2;i++){
			for(j=1;j<=c;j++){
				tmp=x[i][j];
				x[i][j]=x[r+1-i][j];
				x[r+1-i][j]=tmp;
			}
		}
	}
	else{/*reverse by column*/
		for(i=1;i<=r;i++){
			for(j=1;j<=c/2;j++){
				tmp=x[i][j];
				x[i][j]=x[i][c+1-j];
				x[i][c+1-j]=tmp;
			}
		}
	}
}



#endif