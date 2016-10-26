#include "StdAfx.h"
#include "pcloud.h"
#include <fstream>

pcloud::pcloud(void)
	: range(0)

{
	pi=(3.1415926535897932384626433832795);
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


	sortlist();
	uniquelist();

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
		if(eq0(B1)){
			c1.pt[2]=c2.pt[2]=C2/B2;
		}
		else{
			c1.pt[2]=c2.pt[2]=C1/B1;
		}

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





bool pcloud::seedtrg(double r, long &pi1, long &pi2, long &pi3, point3d &c)
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

						long chkpi=voxel[x][y][z][k];
						///////////////////////////////////////////////////////////
						for(size_t i=0;i<nb.size();i++){
							if(nb[i]!=chkpi){

								for(size_t j=i+1;j<nb.size();j++){
									if(nb[j]!=chkpi){


										nc=findBallCenter(plist[nb[i]],plist[nb[j]],plist[chkpi],r,c1,c2);
										if(nc<1){
											continue;
										}



										accept=true;


										for(size_t l=0;l<nb.size();l++){
											if(l!=i && l!=j && nb[l]!=chkpi){
												if(leq0(dist(c1,plist[nb[l]])-r)){
													accept=false;
													break;
												}
											}
										}

										if(accept){
											pi1=nb[i];
											pi2=nb[j];
											pi3=chkpi;
											c=c1;
											return true;
										}

										if(nc>1){

											for(size_t l=0;l<nb.size();l++){
												if(l!=i && l!=j && nb[l]!=chkpi){
													if(leq0(dist(c2,plist[nb[l]])-r)){
														accept=false;
														break;
													}
												}
											}

											if(accept){
												pi1=nb[i];
												pi2=nb[j];
												pi3=chkpi;
												c=c2;
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


void pcloud::ballPivot(long fi, long &npi, point3d &nn, double r, point3d &newc)
{
	std::vector<long> nb;
	nb.clear();
	double d=2*r;
	long pi1=F[fi][0];
	long pi2=F[fi][1];
	long pi3=F[fi][2];
	long ni=F[fi][3];
	long ci=F[fi][4];


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

	int nr;
	point3d c1,c2;
	point3d midp;

	double rotangle1,rotangle2;

	double rotmin=8;

	point3d v12=vt21(plist[pi1],plist[pi2]);

	for(size_t i=0;i<nb.size();i++){

		if( nb[i]!=pi1 && nb[i]!=pi2 && nb[i]!=pi3 ){

			nr=findBallCenter(plist[pi1],plist[pi2],plist[nb[i]],r,c1,c2);
			if(nr>0){
				midp.pt[0]=(plist[pi1].pt[0]+plist[pi2].pt[0])/2;
				midp.pt[1]=(plist[pi1].pt[1]+plist[pi2].pt[1])/2;
				midp.pt[2]=(plist[pi1].pt[2]+plist[pi2].pt[2])/2;

				rotangle1=rotateAngle(midp,normalList[ni],centerList[ci],c1);
				if(rotangle1<=rotmin){
					rotmin=rotangle1;
					npi=nb[i];
					newc=c1;
					if(nr>1){
						nn=vt21(c1,c2);
					}
					else{
						point3d v1n=vt21(plist[pi1],plist[nb[i]]);
						crosspro(v12,v1n,nn);
						if(dotpro(nn,normalList[ni])<0){
							nn.pt[0]=-nn.pt[0];
							nn.pt[1]=-nn.pt[1];
							nn.pt[2]=-nn.pt[2];
						}
					}
				}

				if(nr>1){
					rotangle2=rotateAngle(midp,normalList[ni],centerList[ci],c2);
					if(rotangle2<=rotmin){
						rotmin=rotangle2;
						npi=nb[i];
						newc=c2;
						nn=vt21(c2,c1);
					}
				}

			}
		}
	}
}




void pcloud::gunqiu(double r)
{
	if(F.empty())
		return;


	long npi;
	point3d newn;
	point3d newc;

	std::vector<long> onetriangle(3,0);
	std::vector<long> Fitem1(5,0);
	std::vector<long> Fitem2(5,0);
	bool flg1,flg2;

	long fi1lo,fi2lo;

	npi=-1;
	for(size_t i=0;i<F.size();){
		ballPivot(i,npi,newn,r,newc);
		if(npi>=0){
			onetriangle[0]=F[i][0];
			onetriangle[1]=F[i][1];
			onetriangle[2]=npi;
			triangleList.push_back(onetriangle);
			normalList.push_back(newn);
			centerList.push_back(newc);

			Fitem1[0]=onetriangle[0];
			Fitem1[1]=onetriangle[2];
			Fitem1[2]=onetriangle[1];
			Fitem1[3]=normalList.size()-1;
			Fitem1[4]=centerList.size()-1;

			Fitem2[0]=onetriangle[2];
			Fitem2[1]=onetriangle[1];
			Fitem2[2]=onetriangle[0];
			Fitem2[3]=normalList.size()-1;
			Fitem2[4]=centerList.size()-1;



			

			if(pstatelist[npi]==0){
				pstatelist[npi]=2;
				F.push_back(Fitem1);
				F.push_back(Fitem2);
				F.erase(F.begin()+i);
			}
			else{
				flg1=flg2=true;

				for(size_t j=0;j<F.size() && (flg1||flg2);j++){
					if( ( F[j][0]==Fitem1[1] && F[j][1]==Fitem1[0] ) ){
						flg1=false;
						//F.erase(F.begin()+j);
						fi1lo=j;
						//continue;
					}
					if( ( F[j][0]==Fitem2[1] && F[j][1]==Fitem2[0] ) ){
						flg2=false;
						//F.erase(F.begin()+j);
						fi2lo=j;
						//continue;
					}
					//j++;
				}


				if(flg1){
					if(flg2){
						FBoudary.push_back(F[i]);
					}
					else{
						F.erase(F.begin()+fi2lo);
						F.push_back(Fitem1);
					}
				}
				else{
					if(flg2){
						F.erase(F.begin()+fi1lo);
						F.push_back(Fitem2);
					}
					else{
						if(fi1lo>fi2lo){
						F.erase(F.begin()+fi1lo);
						F.erase(F.begin()+fi2lo);
						}
						else{
						F.erase(F.begin()+fi2lo);
						F.erase(F.begin()+fi1lo);
						}
					}
				}

				F.erase(F.begin()+i);

			}



			//if(!flg1 && !flg2){
			//	pstatelist[onetriangle[0]]=2;
			//	pstatelist[onetriangle[1]]=2;
			//	pstatelist[npi]=1;
			//}
			//else{
			//	pstatelist[onetriangle[0]]=1;
			//	pstatelist[onetriangle[1]]=1;
			//	pstatelist[npi]=1;
			//}


		}
		else{
			FBoudary.push_back(F[i]);
			//F.erase(F.begin()+i);
		}

		//TRACE("[%d]%d,%d,%d,%d,%d\n",F.size(),F.front()[0],F.front()[1],F.front()[2],F.front()[3],F.front()[4]);
	}









}


void pcloud::initialBall(double r)
{
	align();
	genvoxel(r*2);

	point3d c;
	point3d nor;
	long pi1,pi2,pi3;
	bool flg=seedtrg(r,pi1,pi2,pi3,c);

	if(!flg)
		return;

	triangleNormal(plist[pi1],plist[pi2],plist[pi3],c,nor);

	std::vector<long> onetriangle(3,0);

	onetriangle[0]=pi1;
	onetriangle[1]=pi2;
	onetriangle[2]=pi3;
	triangleList.push_back(onetriangle);
	normalList.push_back(nor);
	centerList.push_back(c);

	std::vector<long> Fitem(5,0);

	Fitem[0]=onetriangle[0];
	Fitem[1]=onetriangle[1];
	Fitem[2]=onetriangle[2];
	Fitem[3]=normalList.size()-1;
	Fitem[4]=centerList.size()-1;

	F.push_back(Fitem);
	Fitem[0]=onetriangle[1];
	Fitem[1]=onetriangle[2];
	Fitem[2]=onetriangle[0];

	F.push_back(Fitem);

	Fitem[0]=onetriangle[2];
	Fitem[1]=onetriangle[0];
	Fitem[2]=onetriangle[1];

	F.push_back(Fitem);


	std::vector<int> ps(plist.size(),0);

	ps[pi1]=ps[pi2]=ps[pi3]=2;

	pstatelist=ps;

}





void pcloud::sortlist()
{
	std::sort(plist.begin(), plist.end(), p3dcp);
	std::sort(clist.begin(), clist.end(), p3dcp);
}


void pcloud::uniquelist()
{
	// using default comparison:
	std::vector<point3d>::iterator it;
	// using predicate comparison:
	it = std::unique (plist.begin(), plist.end(), p3deq); 
	plist.resize( std::distance(plist.begin(),it) );

	// using default comparison:
	std::vector<point3d>::iterator it2;
	// using predicate comparison:
	it2 = std::unique (clist.begin(), clist.end(), p3deq); 
	clist.resize( std::distance(clist.begin(),it2) );

}