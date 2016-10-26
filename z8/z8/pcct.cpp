#include "StdAfx.h"
#include "pcct.h"


pcct::pcct(void)
{
}


pcct::~pcct(void)
{
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
		long i=0;


		segment tmp;

		TRACE("\n--Begin to read file");

		while(file.ReadString(strRead))
		{	
			if(wcscoll(strRead.GetBuffer(),sps7)==0){
				break;
			}
			//token = wcstok( strRead.GetBuffer(), sps2 );
			//if(wcscoll(token,sps3)==0){
			//	file.ReadString(strRead);
			//	if(wcscoll(strRead.GetBuffer(),L"")==0){
			//	}
			//	else{

			//		//file.ReadString(strRead);
			//		token = wcstok( strRead.GetBuffer(), sps2 );
			//		token = wcstok( NULL, sps2 );
			//		token = wcstok( NULL, sps5 );
			//		tmp.Ep=_wtof(token);
			//		token = wcstok( NULL, sps2 );
			//		tmp.EpUnit=token[0];

			//		file.ReadString(strRead);
			//		token = wcstok( strRead.GetBuffer(), sps2 );
			//		token = wcstok( NULL, sps2 );
			//		token = wcstok( NULL, sps5 );
			//		tmp.ip=_wtof(token);
			//		token = wcstok( NULL, sps2 );
			//		tmp.ipUnit=token[0];

			//		file.ReadString(strRead);
			//		token = wcstok( strRead.GetBuffer(), sps2 );
			//		token = wcstok( NULL, sps2 );
			//		token = wcstok( NULL, sps5 );
			//		tmp.Ah=_wtof(token);
			//		token = wcstok( NULL, sps2 );
			//		tmp.AhUnit=token[0];
			//		segmentList.push_back(tmp);
			//	}
			//}

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
}
