#include "StdAfx.h"
#include "pcloud.h"
#include <fstream>

pcloud::pcloud(void)
	: range(0)
{
}


pcloud::~pcloud(void)
{
}


void pcloud::load(wchar_t * fp)
{

	CStdioFile file;
	BOOL readflag;
	readflag=file.Open(fp, CFile::modeRead);
	point3d p;

	if(readflag)
	{	
		CString strRead;

		wchar_t *token;
		wchar_t seps[] = L" ,";
		wchar_t sps1[]=L",";
		wchar_t sps2[]=L" ";
		wchar_t sps3[]=L"Segment";
		wchar_t sps4[]=L"qwertyuiopasdfghjklzxcvbnm";
		wchar_t sps5[]=L"QWERTYUIOPASDFGHJKLZXCVBNM";
		wchar_t sps6[]=L"1234567890";
		wchar_t sps7[]=L"Potential/V, Current/A, Charge/C, Time/s";
		wchar_t sps8[]=L"]}";
		wchar_t sps9[]=L" \t";
		long i=0;




		TRACE("\n--Begin to read file");



		file.ReadString(strRead);
		file.ReadString(strRead);
		file.ReadString(strRead);
		file.ReadString(strRead);
		file.ReadString(strRead);

		while(file.ReadString(strRead))
		{
			//TRACE("\n--%s", strRead.GetBuffer());

			//token = wcstok( strRead.GetBuffer(), sps8 );
			//if(token!=NULL){
			//	token = wcstok( NULL, sps9 );
			//	if(token==NULL)
			//	break;
			//}

			token = wcstok( strRead.GetBuffer(), sps9 );

			if(token!=NULL)
				p.pt[0]=(_wtof(token));
			else
				break;

			token = wcstok( NULL, sps9 );
			if(token!=NULL)
				p.pt[1]=(_wtof(token));
			else
				break;

			token = wcstok( NULL, sps9);
			if(token!=NULL)
				p.pt[2]=(_wtof(token));
			else
				break;

			/*if(0.2787*p.pt[0]-1*p.pt[2]+0*p.pt[2]+1120.5>0)*/
			plist.push_back(p);

			//TRACE("%f,%f,%f,%f,%d",potential[i],current[i],charge[i],time[i],i);



		}

		file.ReadString(strRead);
		file.ReadString(strRead);
		file.ReadString(strRead);
		file.ReadString(strRead);

		while(file.ReadString(strRead))
		{
			//TRACE("\n--%s", strRead.GetBuffer());

			//token = wcstok( strRead.GetBuffer(), sps8 );
			//if(token==NULL)
			//	break;

			token = wcstok( strRead.GetBuffer(), sps9 );

			if(token!=NULL)
				p.pt[0]=(_wtof(token));
			else
				break;

			token = wcstok( NULL, sps9 );
			if(token!=NULL)
				p.pt[1]=(_wtof(token));
			else
				break;

			token = wcstok( NULL, sps9);
			if(token!=NULL)
				p.pt[2]=(_wtof(token));
			else
				break;

			clist.push_back(p);

			//TRACE("%f,%f,%f,%f,%d",potential[i],current[i],charge[i],time[i],i);



		}


		TRACE("\n--End reading");
		file.Close();

	}



	//return readflag;
	getminmax();
}


void pcloud::getminmax()
{
	if(!plist.empty())
	{
		maxp.pt[0]=plist[0].pt[0];
		maxp.pt[1]=plist[0].pt[1];
		maxp.pt[2]=plist[0].pt[2];

		minp.pt[0]=plist[0].pt[0];
		minp.pt[1]=plist[0].pt[1];
		minp.pt[2]=plist[0].pt[2];

		size_t i;
		for(i=1;i<plist.size();i++){
			if(plist[i].pt[0]<minp.pt[0])
				minp.pt[0]=plist[i].pt[0];
			if(plist[i].pt[0]>maxp.pt[0])
				maxp.pt[0]=plist[i].pt[0];

			if(plist[i].pt[1]<minp.pt[1])
				minp.pt[1]=plist[i].pt[1];
			if(plist[i].pt[1]>maxp.pt[1])
				maxp.pt[1]=plist[i].pt[1];

			if(plist[i].pt[2]<minp.pt[2])
				minp.pt[2]=plist[i].pt[2];
			if(plist[i].pt[2]>maxp.pt[2])
				maxp.pt[2]=plist[i].pt[2];

		}
	}
}

void pcloud::align()
{
	size_t i;
	for(i=0;i<plist.size();i++){
		plist[i].pt[0]-=minp.pt[0];
		plist[i].pt[1]-=minp.pt[1];
		plist[i].pt[2]-=minp.pt[2];
	}

	maxp.pt[0]-=minp.pt[0];
	maxp.pt[1]-=minp.pt[1];
	maxp.pt[2]-=minp.pt[2];
	minp.pt[0]=minp.pt[1]=minp.pt[2]=0;
}


void pcloud::unity(double a)
{
	//point3d maxp,minp;



	double w=maxp.pt[0]-minp.pt[0];

	if(maxp.pt[1]-minp.pt[1]>w)
		w=maxp.pt[1]-minp.pt[1];
	if(maxp.pt[2]-minp.pt[2]>w)
		w=maxp.pt[2]-minp.pt[2];


	w=a/w;
	size_t i;
	for(i=0;i<plist.size();i++){
		plist[i].pt[0]-=(maxp.pt[0]+minp.pt[0])/2;
		plist[i].pt[1]-=(maxp.pt[1]+minp.pt[1])/2;
		plist[i].pt[2]-=(maxp.pt[2]+minp.pt[2])/2;

		plist[i].pt[0]*=w;
		plist[i].pt[1]*=w;
		plist[i].pt[2]*=w;
	}

	range=a;

	//getminmax();
}
void pcloud::warp(double x, double y, double z)
{

	//for (auto it = plist.cbegin() ; it != plist.cend(); ++it){
	//	if( (fabs((*it).pt[0])>x)||(fabs((*it).pt[1])>y)||(fabs((*it).pt[2])>z) ){
	//		plist.erase(it);
	//	}
	//}

	size_t i,j;
	j=0;
	while(true){

		for(i=j;i<plist.size();i++){
			j=i;

			if( (fabs(plist[i].pt[0])>x)||(fabs(plist[i].pt[1])>y)||(fabs(plist[i].pt[2])>z) ){
				plist.erase(plist.begin()+i);			
				break;
			}
		}
		TRACE("%d\n",plist.size());
		if(i>=plist.size()-1)
			break;

	}




}
void pcloud::warp1()
{



	size_t i;

	for(i=0;i<plist.size();){
		if(0.2787*plist[i].pt[0]-1*plist[i].pt[2]+0*plist[i].pt[2]+1130.5>0){
			plist.erase(plist.begin()+i);
			clist.erase(clist.begin()+i);
		}
		else{
			i++;
			TRACE("%d,",i);
		}
	}



}


void pcloud::genvoxel(double d)
{
	if(minp.pt[0]!=0||minp.pt[1]!=0||minp.pt[2]!=0){
		align();
	}

	std::vector<long> v1;
	//std::vector< std::vector<long> > v2(ceil(maxp.pt[2]/d),v1);
	//long lz=v2.size();
	//std::vector< std::vector< std::vector<long> > > v3(ceil(maxp.pt[1]/d),v2);
	//long ly=v3.size();
	//std::vector< std::vector< std::vector< std::vector<long> > > > voxel1(ceil(maxp.pt[0]/d),v3);
	//long lx=voxel1.size();


	std::vector< std::vector<long> > v2(1+floor(maxp.pt[2]/d),v1);
	std::vector< std::vector< std::vector<long> > > v3(1+floor(maxp.pt[1]/d),v2);
	std::vector< std::vector< std::vector< std::vector<long> > > > voxel1(1+floor(maxp.pt[0]/d),v3);
	size_t i;
	//for(i=0;i<ceil(maxp.pt[2]/d);i++){
	//	v2.push_back(v1);
	//}

	//for(i=0;i<ceil(maxp.pt[1]/d);i++){
	//	v3.push_back(v2);
	//}
	//for(i=0;i<ceil(maxp.pt[0]/d);i++){
	//	voxel.push_back(v3);
	//}

	for(i=0;i<plist.size();i++){
		long x=plist[i].pt[0]/d;
		long y=plist[i].pt[1]/d;
		long z=plist[i].pt[2]/d;
		voxel1[x][y][z].push_back((long)i);
	}

	voxel=voxel1;


}

bool pcloud::eq0(double x)
{
	return (fabs(x)<1e-40);
}

int pcloud::slvrt2(double a, double b, double c, double &r1, double &r2)
{
	double tmp=b*b-4*a*c;
	if(eq0(tmp)){
		r1=r2=-b/2/a;
		return 1;
	}
	else{
		if(tmp<0){
			return 0;
		}
		else{
			r1=(-b+sqrt(tmp))/2/a;
			r2=(-b-sqrt(tmp))/2/a;
			return 2;
		}
	}

}

int pcloud::findBallCenter(point3d p1, point3d p2, point3d p3, double r, point3d &c1, point3d &c2)
{
	double r1=p1.pt[0]*p1.pt[0]+p1.pt[1]*p1.pt[1]+p1.pt[2]*p1.pt[2];
	double r2=p2.pt[0]*p2.pt[0]+p2.pt[1]*p2.pt[1]+p2.pt[2]*p2.pt[2];
	double r3=p3.pt[0]*p3.pt[0]+p3.pt[1]*p3.pt[1]+p3.pt[2]*p3.pt[2];

	//A1*x+B1*z=C1
	//A2*y+B2*z=C2
	double A1=(p1.pt[0]-p2.pt[0])*(p1.pt[1]-p3.pt[1])-(p1.pt[0]-p3.pt[0])*(p1.pt[1]-p2.pt[1]);
	double A2=(p1.pt[1]-p2.pt[1])*(p1.pt[0]-p3.pt[0])-(p1.pt[1]-p3.pt[1])*(p1.pt[0]-p2.pt[0]);
	//A2=-A1;
	double B1=(p1.pt[2]-p2.pt[2])*(p1.pt[1]-p3.pt[1])-(p1.pt[2]-p3.pt[2])*(p1.pt[1]-p2.pt[1]);
	double B2=(p1.pt[2]-p2.pt[2])*(p1.pt[0]-p3.pt[0])-(p1.pt[2]-p3.pt[2])*(p1.pt[0]-p2.pt[0]);

	double C1=((r1-r2)*(p1.pt[1]-p3.pt[1])-(r1-r3)*(p1.pt[1]-p2.pt[1]))/2;
	double C2=((r1-r2)*(p1.pt[0]-p3.pt[0])-(r1-r3)*(p1.pt[0]-p2.pt[0]))/2;



	if(eq0(A1)){
		//solve z
		c1.pt[2]=c2.pt[2]=C1/B1;
		//x=K*y+B
		double K,B;
		if(eq0(p1.pt[0]-p2.pt[0])){
			K=-(p1.pt[1]-p3.pt[1])/(p1.pt[0]-p3.pt[0]);
			B=(r1-r3)/2/(p1.pt[0]-p3.pt[0])-(p1.pt[2]-p3.pt[2])*c1.pt[2]/(p1.pt[0]-p3.pt[0]);
		}
		else{
			K=-(p1.pt[1]-p2.pt[1])/(p1.pt[0]-p2.pt[0]);
			B=(r1-r2)/2/(p1.pt[0]-p2.pt[0])-(p1.pt[2]-p2.pt[2])*c1.pt[2]/(p1.pt[0]-p2.pt[0]);
		}

		//solve y
		int rc=slvrt2(K*K+1, 2*(K*(B-p1.pt[0])-p1.pt[1]), (B-p1.pt[0])*(B-p1.pt[0])+p1.pt[1]*p1.pt[1]+(c1.pt[2]-p1.pt[2])*(c1.pt[2]-p1.pt[2])-r*r, c1.pt[1], c2.pt[1] );
		//solve x
		if(rc>0){
			c1.pt[0]=K*c1.pt[1]+B;
			c2.pt[0]=K*c2.pt[1]+B;
		}
		return rc;
	}

	if(eq0(B1)){
		//solve x
		c1.pt[0]=c2.pt[0]=C1/A1;
		//y=K*z+B
		double K,B;
		K=-B2/A2;
		B=C2/A2;

		//solve z
		int rc=slvrt2(K*K+1, 2*(K*(B-p1.pt[1])-p1.pt[2]), (B-p1.pt[1])*(B-p1.pt[1])+p1.pt[2]*p1.pt[2]+(c1.pt[0]-p1.pt[0])*(c1.pt[0]-p1.pt[0])-r*r, c1.pt[2], c2.pt[2] );
		//solve y
		if(rc>0){
			c1.pt[1]=K*c1.pt[2]+B;
			c2.pt[1]=K*c2.pt[2]+B;
		}
		return rc;
	}

	if(eq0(B2)){
		//solve y
		c1.pt[1]=c2.pt[1]=C2/A2;
		//x=K*z+B
		double K,B;
		K=-B1/A1;
		B=C1/A1;

		//solve z
		int rc=slvrt2(K*K+1, 2*(K*(B-p1.pt[0])-p1.pt[2]), (B-p1.pt[0])*(B-p1.pt[0])+p1.pt[2]*p1.pt[2]+(c1.pt[1]-p1.pt[1])*(c1.pt[1]-p1.pt[1])-r*r, c1.pt[2], c2.pt[2] );
		//solve x
		if(rc>0){
			c1.pt[0]=K*c1.pt[2]+B;
			c2.pt[0]=K*c2.pt[2]+B;
		}
		return rc;
	}


	//x=K1*z+D1
	//y=K2*z+D2
	double K1=-B1/A1;
	double K2=-B2/A2;
	double D1=C1/A1;
	double D2=C2/A2;

	//solve z
	int rc=slvrt2(K1*K1+K2*K2+1, 2*(K1*(D1-p1.pt[0])+K2*(D2-p1.pt[1])-p1.pt[2]), (D1-p1.pt[0])*(D1-p1.pt[0])+(D2-p1.pt[1])*(D2-p1.pt[1])+p1.pt[2]*p1.pt[2]-r*r, c1.pt[2], c2.pt[2] );

	if(rc>0){
		//solve x
		c1.pt[0]=K1*c1.pt[2]+D1;
		c2.pt[0]=K1*c2.pt[2]+D1;
		//solve y
		c1.pt[1]=K2*c1.pt[2]+D2;
		c2.pt[1]=K2*c2.pt[2]+D2;
	}
	return rc;

}


double pcloud::dist(point3d p1, point3d p2)
{
	double d=0;
	for(int i=0;i<3;i++)
		d+=(p1.pt[i]-p2.pt[i])*(p1.pt[i]-p2.pt[i]);
	return sqrt(d);
}


bool pcloud::seedtrg(double r, long &pi1, long &pi2, long &pi3)
{
	long i1;
	std::vector<long> nb;
	for(size_t x=0;x<voxel.size();x++){
		for(size_t y=0;y<voxel[x].size();y++){
			for(size_t z=0;z<voxel[x][y].size();z++){
				nb.clear();

				//for(int i=-1;i<=1;i++){
				//	if( x+i<0 || x+i>=voxel.size()){
				//		continue;
				//	}
				//	else{
				//		for(int j=-1;j<=1;j++){
				//			if( y+j<0 || y+j>=voxel[x+i].size()){
				//				continue;
				//			}
				//			else{
				//				for(int k=-1;k<=1;k++){
				//					if( z+k<0 || z+k>=voxel[x+i][y+j].size()){
				//						continue;
				//					}
				//					else{
				//						nb.insert(nb.end(),voxel[x+i][y+j][z+k].begin(),voxel[x+i][y+j][z+k].end());
				//					}
				//				}
				//			}
				//		}
				//	}
				//}
				nbvidx(x,y,z,nb);


				if(nb.size()>=3 && !voxel[x][y][z].empty()){
					point3d c1,c2;
					int nc;
					bool accept;

					for(size_t k=0;k<voxel[x][y][z].size();k++){

						for(size_t i=0;i<nb.size();i++){
							if(nb[i]!=voxel[x][y][z][k]){

								for(size_t j=i+1;j<nb.size();j++){
									if(nb[i]!=voxel[x][y][z][k]){


										nc=findBallCenter(plist[nb[i]],plist[nb[j]],plist[voxel[x][y][z][k]],r,c1,c2);
										if(nc<1){
											continue;
										}



										accept=true;


										for(size_t l=0;l<nb.size();l++){
											if(l!=i && l!=j && l!=k){
												if(dist(c1,plist[nb[l]])<r){
													accept=false;
													break;
												}
											}
										}

										if(accept){
											pi1=nb[i];
											pi2=nb[j];
											pi3=nb[k];
											return true;
										}

										if(nc>1){

											for(size_t l=0;l<nb.size();l++){
												if(l!=i && l!=j && l!=k){
													if(dist(c2,plist[nb[l]])<r){
														accept=false;
														break;
													}
												}
											}

											if(accept){
												pi1=nb[i];
												pi2=nb[j];
												pi3=nb[k];
												return true;
											}
										}
									}

								}
							}
						}
					}
				}





			}
		}
	}


	return false;

}


void pcloud::nbvidx(long x, long y, long z, std::vector<long> &nb)
{
	for(int i=-1;i<=1;i++){
		if( x+i<0 || x+i>=voxel.size()){
			continue;
		}
		else{
			for(int j=-1;j<=1;j++){
				if( y+j<0 || y+j>=voxel[x+i].size()){
					continue;
				}
				else{
					for(int k=-1;k<=1;k++){
						if( z+k<0 || z+k>=voxel[x+i][y+j].size()){
							continue;
						}
						else{
							nb.insert(nb.end(),voxel[x+i][y+j][z+k].begin(),voxel[x+i][y+j][z+k].end());
						}
					}
				}
			}
		}
	}

}


long pcloud::ballPivot(long pi1, long pi2, point3d oldc, double r, point3d &newc)
{
	std::vector<long> nb;
	nb.clear();
	double d=2*r;
	long x1=plist[pi1].pt[0]/d;
	long y1=plist[pi1].pt[1]/d;
	long z1=plist[pi1].pt[2]/d;
	long x2=plist[pi2].pt[0]/d;
	long y2=plist[pi2].pt[1]/d;
	long z2=plist[pi2].pt[2]/d;


	nbvidx(x1,y1,z1,nb);
	if(x2!=x1 || y2!=y1 ||z2!=z1){
		nbvidx(x2,y2,z2,nb);
		std::sort(nb.begin(),nb.end());
		std::vector<long>::iterator it;
		it = std::unique (nb.begin(), nb.end());
		nb.resize( std::distance(nb.begin(),it) ); 

	}


	//for(



}

double pcloud::dotpro(point3d v1, point3d v2)
{
	return v1.pt[0]*v2.pt[0]+v1.pt[1]*v2.pt[1]+v1.pt[2]*v2.pt[2];
}

void pcloud::crosspro(point3d v1, point3d v2, point3d &vp)
{
	vp.pt[0]=v1.pt[1]*v2.pt[2]-v1.pt[2]*v2.pt[1];
	vp.pt[1]=v1.pt[2]*v2.pt[0]-v1.pt[0]*v2.pt[2];
	vp.pt[2]=v1.pt[0]*v2.pt[1]-v1.pt[1]*v2.pt[0];
}

void pcloud::triangleNormal(point3d p1, point3d p2, point3d p3, point3d c, point3d &nv)
{
	point3d v1,v2,vc;
	v1.pt[0]=p1.pt[0]-p3.pt[0];
	v1.pt[1]=p1.pt[1]-p3.pt[1];
	v1.pt[2]=p1.pt[2]-p3.pt[2];

	v2.pt[0]=p2.pt[0]-p3.pt[0];  
	v2.pt[1]=p2.pt[1]-p3.pt[1];
	v2.pt[2]=p2.pt[2]-p3.pt[2];

	vc.pt[0]=c.pt[0]-p3.pt[0];
	vc.pt[1]=c.pt[1]-p3.pt[1];
	vc.pt[2]=c.pt[2]-p3.pt[2];

	crosspro(v1,v2,nv);

	if(dotpro(nv,vc)<0){
		nv.pt[0]=-nv.pt[0];
		nv.pt[1]=-nv.pt[1];
		nv.pt[2]=-nv.pt[2];
	}

}


double pcloud::triangleRadius(point3d p1, point3d p2, point3d p3)
{
	double d12=dist(p1,p2);
	double d23=dist(p2,p3);
	double d31=dist(p3,p1);
	double a=(d12+d23+d31)*(d12+d23-d31)*(d12-d23+d31)*(-d12+d23+d31);
	if(eq0(a))
		return -1;
	else
		return d12*d23*d31/sqrt(a);
}


