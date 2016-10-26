#pragma once

#include <vector>
#include <algorithm>

typedef struct SEGMENT{
	double Ep;
	double ip;
	double Ah;
	wchar_t EpUnit;
	wchar_t ipUnit;
	wchar_t AhUnit;
} segment;

class pcct
{

public:
	std::vector<segment> segmentList;
	//Potential/V, Current/A, Charge/C, Time/s
	std::vector<double> potential;
	std::vector<double> current;
	std::vector<double> charge;
	std::vector<double> time;
	std::vector<size_t> xBreakIndex;
	std::vector<CString> label;
	std::vector<CString> seginfo;

	CString FilePath;
	CString FileName;
	double AR;
	CString segmentinfo;
	double addVolume;
	CString stepName;

public:
	pcct(void)
		: AR(0)
		, segmentinfo(_T(""))
		, addVolume(0)
		, stepName(_T(""))
		, FilePath(_T(""))
		, FileName(_T(""))
	{};
	~pcct(void){};
	
	void clear(void)
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

		if(!segmentinfo.IsEmpty())
			segmentinfo.Empty();
		if(!stepName.IsEmpty())
			stepName.Empty();

		if(!FilePath.IsEmpty())
			FilePath.Empty();
		if(!FileName.IsEmpty())
			FileName.Empty();

	};

	void seperate(void)	
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

	};

	double intg(double xtop, size_t xibegin, size_t xiend)
	{


		std::vector<double> xintg(potential.begin()+(xibegin),potential.begin()+(xiend) );
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
	};


	void copyPrefix(pcct a)
	{
		segmentList=a.segmentList;
		segmentinfo=a.segmentinfo;
		label=a.label;
		FilePath=a.FilePath;
		FileName=a.FileName;
		seginfo=a.seginfo;
	};

	void copyData(pcct a)
	{
		potential=a.potential;
		current=a.current;
		charge=a.charge;
		time=a.time;
	};	


	void copy(pcct a)
	{
		copyPrefix(a);
		copyData(a);
		xBreakIndex=a.xBreakIndex;
		AR=a.AR;
	};


	size_t popData(std::vector<double> & x, std::vector<double> & y, size_t nd)
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
	};

	void TomA(void)
	{
		for(size_t i=0;i<current.size();i++){
			current[i]*=1e3;
		}
		label[1]=L"Current/mA";
	};


	///////////////////////////////////////////////////////
	int readFile(LPCTSTR lpszFileName)
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
	};


	double intg1()
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
	};

	double intg2(double xtop)
	{

		seperate();

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

	};


	void SetTimeIntv(double intv=0)
	{
		if(time.size()>2){	
			size_t i=1;
			if(intv==0){
				intv=time[1]-time[0];
				i=2;
			}
			for(;i<time.size();i++){
				time[i]=intv+time[i-1];
			}
		}
	};

};

