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
				p.pt3[0]=(_wtof(token));
			else
				break;

			token = wcstok( NULL, sps9 );
			if(token!=NULL)
				p.pt3[1]=(_wtof(token));
			else
				break;

			token = wcstok( NULL, sps9);
			if(token!=NULL)
				p.pt3[2]=(_wtof(token));
			else
				break;

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
				p.pt3[0]=(_wtof(token));
			else
				break;

			token = wcstok( NULL, sps9 );
			if(token!=NULL)
				p.pt3[1]=(_wtof(token));
			else
				break;

			token = wcstok( NULL, sps9);
			if(token!=NULL)
				p.pt3[2]=(_wtof(token));
			else
				break;

			clist.push_back(p);

			//TRACE("%f,%f,%f,%f,%d",potential[i],current[i],charge[i],time[i],i);



		}


		TRACE("\n--End reading");
		file.Close();

	}



	//return readflag;

}


void pcloud::unity(double a)
{
	point3d maxp,minp;

	maxp.pt3[0]=plist[0].pt3[0];
	maxp.pt3[1]=plist[0].pt3[1];
	maxp.pt3[2]=plist[0].pt3[2];

	minp.pt3[0]=plist[0].pt3[0];
	minp.pt3[1]=plist[0].pt3[1];
	minp.pt3[2]=plist[0].pt3[2];

	size_t i;
	for(i=1;i<plist.size();i++){
		if(plist[i].pt3[0]<minp.pt3[0])
			minp.pt3[0]=plist[i].pt3[0];
		if(plist[i].pt3[0]>maxp.pt3[0])
			maxp.pt3[0]=plist[i].pt3[0];

		if(plist[i].pt3[1]<minp.pt3[1])
			minp.pt3[1]=plist[i].pt3[1];
		if(plist[i].pt3[1]>maxp.pt3[1])
			maxp.pt3[1]=plist[i].pt3[1];

		if(plist[i].pt3[2]<minp.pt3[2])
			minp.pt3[2]=plist[i].pt3[2];
		if(plist[i].pt3[2]>maxp.pt3[2])
			maxp.pt3[2]=plist[i].pt3[2];

	}

	double w=maxp.pt3[0]-minp.pt3[0];

	if(maxp.pt3[1]-minp.pt3[1]>w)
		w=maxp.pt3[1]-minp.pt3[1];
	if(maxp.pt3[2]-minp.pt3[2]>w)
		w=maxp.pt3[2]-minp.pt3[2];

	range=a;
	w=range/w;

	for(i=0;i<plist.size();i++){
		plist[i].pt3[0]-=(maxp.pt3[0]+minp.pt3[0])/2;
		plist[i].pt3[1]-=(maxp.pt3[1]+minp.pt3[1])/2;
		plist[i].pt3[2]-=(maxp.pt3[2]+minp.pt3[2])/2;

		plist[i].pt3[0]*=w;
		plist[i].pt3[1]*=w;
		plist[i].pt3[2]*=w;
	}
}


void pcloud::warp(double x, double y, double z)
{

	//for (auto it = plist.cbegin() ; it != plist.cend(); ++it){
	//	if( (fabs((*it).pt3[0])>x)||(fabs((*it).pt3[1])>y)||(fabs((*it).pt3[2])>z) ){
	//		plist.erase(it);
	//	}
	//}

	size_t i,j;
	j=0;
	while(true){

		for(i=j;i<plist.size();i++){
			j=i;
			
			if( (fabs(plist[i].pt3[0])>x)||(fabs(plist[i].pt3[1])>y)||(fabs(plist[i].pt3[2])>z) ){
				plist.erase(plist.begin()+i);			
				break;
			}
		}
		TRACE("%d\n",plist.size());
		if(i>=plist.size()-1)
			break;

	}

}
