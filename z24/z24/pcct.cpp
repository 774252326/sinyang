#include "StdAfx.h"
#include "pcct.h"


pcct::pcct(void)
	: AR(0)
	, segmentinfo(_T(""))
	, addVolume(0)
	, stepName(_T(""))
{
}


pcct::~pcct(void)
{
}



int pcct::readFile1(LPCTSTR lpszFileName)
{
	CStdioFile file;
	BOOL readflag;
	readflag=file.Open(lpszFileName, CFile::modeRead);
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
		wchar_t sps8[]=L"Time/sec, Current/A";
		wchar_t sps9[]=L"Potential/V, Current/A";
		long i=0;

		int headertype=-1;

		segment tmp;

		TRACE("\n--Begin to read file");

		FilePath=lpszFileName;

		FileName=wcsrchr(FilePath,L'\\');
		FileName.Delete(0);
		//token=token;

		while(file.ReadString(strRead))
		{	

			if( wcscoll(strRead.GetBuffer(),sps7)==0 )				
			{

				token = wcstok( strRead.GetBuffer(), sps1 );

				if(token!=NULL)
					label.push_back(token);

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					label.push_back(token);

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					label.push_back(token);

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					label.push_back(token);

				headertype=7;

				break;
			}


			if(wcscoll(strRead.GetBuffer(),sps8)==0){

				token = wcstok( strRead.GetBuffer(), sps1 );

				if(token!=NULL)
					label.push_back(token);

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					label.push_back(token);

				headertype=8;

				break;
			}

			if(wcscoll(strRead.GetBuffer(),sps9)==0){

				token = wcstok( strRead.GetBuffer(), sps1 );

				if(token!=NULL)
					label.push_back(token);

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					label.push_back(token);

				headertype=9;

				break;
			}




			segmentinfo=segmentinfo+strRead+L"\n";
			//if(!strRead.IsEmpty())
			seginfo.push_back(strRead);



		}



		switch(headertype){
		case 7:


			while(file.ReadString(strRead))
			{
				token = wcstok( strRead.GetBuffer(), sps1 );
				if(token!=NULL)
					potential.push_back(_wtof(token));

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					current.push_back(_wtof(token));

				token = wcstok( NULL, sps1);
				if(token!=NULL)
					charge.push_back(_wtof(token));

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					time.push_back(_wtof(token));
			}
			break;

		case 8:

			while(file.ReadString(strRead))
			{
				token = wcstok( strRead.GetBuffer(), sps1 );
				if(token!=NULL)				
					time.push_back(_wtof(token));

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					current.push_back(_wtof(token));

				//token = wcstok( NULL, sps1);
				//if(token!=NULL)
				//charge.push_back(_wtof(token));

				//token = wcstok( NULL, sps1 );
				//if(token!=NULL)
				//potential.push_back(_wtof(token));
			}
			break;
		case 9:
			while(file.ReadString(strRead))
			{
				token = wcstok( strRead.GetBuffer(), sps1 );
				if(token!=NULL)
					potential.push_back(_wtof(token));

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					current.push_back(_wtof(token));

				//token = wcstok( NULL, sps1);
				//if(token!=NULL)
				//	charge.push_back(_wtof(token));

				//token = wcstok( NULL, sps1 );
				//if(token!=NULL)
				//	time.push_back(_wtof(token));
			}
			break;

		default:
			break;
		}


		TRACE("\n--End reading");
		file.Close();

	}



	return readflag;
}

int pcct::readFile(LPCTSTR lpszFileName)
{

	CStdioFile file;
	BOOL readflag;
	readflag=file.Open(lpszFileName, CFile::modeRead);
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
		wchar_t sps8[]=L"Potential/V, Current/A";
		long i=0;



		segment tmp;

		TRACE("\n--Begin to read file");

		FilePath=lpszFileName;

		FileName=wcsrchr(FilePath,L'\\');
		FileName.Delete(0);
		//token=token;

		while(file.ReadString(strRead))
		{	

			if(wcscoll(strRead.GetBuffer(),sps7)==0 
				||wcscoll(strRead.GetBuffer(),sps8)==0)
			{

				token = wcstok( strRead.GetBuffer(), sps1 );

				if(token!=NULL)
					label.push_back(token);

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					label.push_back(token);

				token = wcstok( NULL, sps1);
				if(token!=NULL)
					label.push_back(token);

				token = wcstok( NULL, sps1 );
				if(token!=NULL)
					label.push_back(token);
				break;
			}
			else{

				if(wcscoll(strRead.GetBuffer(),sps8)==0){

					token = wcstok( strRead.GetBuffer(), sps1 );

					if(token!=NULL)
						label.push_back(token);

					token = wcstok( NULL, sps1 );
					if(token!=NULL)
						label.push_back(token);

					break;
				}
				else{

					segmentinfo=segmentinfo+strRead+L"\n";
					//if(!strRead.IsEmpty())
					seginfo.push_back(strRead);
				}
			}

		}



		while(file.ReadString(strRead))
		{
			//TRACE("\n--%s", strRead.GetBuffer());

			token = wcstok( strRead.GetBuffer(), sps1 );

			if(token!=NULL)
				potential.push_back(_wtof(token));

			token = wcstok( NULL, sps1 );
			if(token!=NULL)
				current.push_back(_wtof(token));

			token = wcstok( NULL, sps1);
			if(token!=NULL)
				charge.push_back(_wtof(token));

			token = wcstok( NULL, sps1 );
			if(token!=NULL)
				time.push_back(_wtof(token));


			//TRACE("%f,%f,%f,%f,%d",potential[i],current[i],charge[i],time[i],i);



		}


		TRACE("\n--End reading");
		file.Close();

	}



	return readflag;
}


void pcct::clear(void)
{
	segmentList.clear();
	//Potential/V, Current/A, Charge/C, Time/s
	potential.clear();
	current.clear();
	charge.clear();
	time.clear();
	xBreakIndex.clear();
	label.clear();
	seginfo.clear();
}


void pcct::seperate(void)
{
	size_t i;
	double diff1,diff2;
	xBreakIndex.push_back(0);
	diff2=potential[0]-potential[1];
	for(i=1;i<potential.size()-1;i++){
		diff1=diff2;
		diff2=potential[i]-potential[i+1];
		if(diff1*diff2<0){
			xBreakIndex.push_back(i);
		}
	}
	xBreakIndex.push_back(potential.size()-1);

}

double pcct::intg1()
{
	double ar=0;


	std::reverse(current.begin(),current.end());
	std::reverse(time.begin(),time.end());



	for(size_t i=1;i<current.size();i++){
		if(current[i-1]*current[i]>=0){
			ar+=(current[i-1]+current[i])*(time[i-1]-time[i]);
		}
		else{
			break;
		}
	}

	ar/=2;

	return ar;
}


double pcct::intg(double xtop, size_t xibegin, size_t xiend)
{
	//std::vector<double> xintg(potential.begin()+(*(xBreakIndex.end()-2)),potential.begin()+(*(xBreakIndex.end()-1)) );
	//std::vector<double> xintg( time.begin()+(*(xBreakIndex.end()-2)),time.begin()+(*(xBreakIndex.end()-1)) );
	//std::vector<double> yintg( current.begin()+(*(xBreakIndex.end()-2)),current.begin()+(*(xBreakIndex.end()-1)) );

	std::vector<double> xintg(potential.begin()+(xibegin),potential.begin()+(xiend) );
	//std::vector<double> xintg( time.begin()+(*(xBreakIndex.end()-2)),time.begin()+(*(xBreakIndex.end()-1)) );
	std::vector<double> yintg( current.begin()+(xibegin),current.begin()+(xiend) );

	size_t i;
	double ar=0;
	for(i=0;i<xintg.size()-1;i++){

		if(xintg[i+1]<xtop){
			if( (yintg[i]>0) || (yintg[i+1]>0) ){
				if(yintg[i]<0){
					ar+=(xintg[i+1]-xintg[i])*yintg[i+1]*yintg[i+1]/(yintg[i+1]-yintg[i]);
				}
				else{
					if(yintg[i+1]<0){
						ar+=(xintg[i+1]-xintg[i])*yintg[i]*yintg[i]/(yintg[i]-yintg[i+1]);
					}
					else{
						ar+=(xintg[i+1]-xintg[i])*(yintg[i]+yintg[i+1]);
					}
				}
			}
		}
		else{
			ar+=(xtop-xintg[i])*(2*yintg[i]+(yintg[i+1]-yintg[i])*(xtop-xintg[i])/(xintg[i+1]-xintg[i]));
			break;
		}

	}

	return ar;
}


void pcct::copy(pcct a)
{
	copyPrefix(a);

	copyData(a);

	xBreakIndex=a.xBreakIndex;
	AR=a.AR;

}


void pcct::copyPrefix(pcct a)
{
	segmentList=a.segmentList;
	segmentinfo=a.segmentinfo;
	label=a.label;
	FilePath=a.FilePath;
	FileName=a.FileName;
	seginfo=a.seginfo;
}


void pcct::copyData(pcct a)
{
	potential=a.potential;
	current=a.current;
	charge=a.charge;
	time=a.time;
}


size_t pcct::popData(std::vector<double> & x, std::vector<double> & y, size_t nd)
{
	if(potential.size()<nd){
		x.assign(potential.begin(),potential.end());
		potential.erase(potential.begin(),potential.end());
		y.assign(current.begin(),current.end());
		current.erase(current.begin(),current.end());
	}
	else{
		x.assign(potential.begin(),potential.begin()+nd);
		potential.erase(potential.begin(),potential.begin()+nd);
		y.assign(current.begin(),current.begin()+nd);
		current.erase(current.begin(),current.begin()+nd);
	}

	return potential.size();
}


void pcct::TomA(void)
{
	for(size_t i=0;i<current.size();i++){
		current[i]*=1e3;
	}

	label[1]=L"Current/mA";
}


double pcct::intg2(double xtop)
{

	this->seperate();

	size_t xe=xBreakIndex.back();
	xBreakIndex.pop_back();

	while(potential[xBreakIndex.back()]>potential[xe]){
		xe=xBreakIndex.back();
		xBreakIndex.pop_back();
	}

	if(xBreakIndex.empty()){
		return 0;
	}

	size_t xs=xBreakIndex.back();
	xBreakIndex.clear();

	return intg(xtop,xs,xe);


	return 0;
}
