
#ifndef READCSVT_H
#define READCSVT_H

//read *.csv file. first column to x[1..n], second column to y[1..n]

#include "funT\paddingT.h"

template <typename T>
bool readcsv(long n, LPCTSTR lpszFileName, T * x, T * y)
{
	CStdioFile file;
	if(file.Open(lpszFileName, CFile::modeRead))
	{	
		CString strRead;

		wchar_t *token;
		wchar_t seps[] = L",";
		long i=1;
		TRACE("\n--Begin to read file");
		while(file.ReadString(strRead))
		{
			//TRACE("\n--%s", strRead.GetBuffer());

			token = wcstok( strRead.GetBuffer(), seps );
			if(NULL!=token)
				x[i]= _wtof(token);

			token = wcstok( NULL, seps );
			if(NULL!=token)
				y[i] = _wtof(token);

			//TRACE("%f,%f,%d",x[i],y[i],i);
			i++;
			if(i>n)
				break;

		}
		TRACE("\n--End reading");
		file.Close();
		return true;
	}
	else
		return false;

}


template <typename T>
T *readcsv2(long *nx, LPCTSTR lpszFileName, bool *readflag)
{
	//read two column csv file
	//first column to tmp[1...nx[0]], second column to tmp[nx[0]+1...nx[0]+nx[0]]

	CStdioFile file;
	readflag[0]=file.Open(lpszFileName, CFile::modeRead);
	if(readflag)
	{	
		CString strRead;
		T *tmp;
		T *tmpk;
		wchar_t *token;
		wchar_t seps[] = L",";
		//long i=1;
		T x,y;
		TRACE("\n--Begin to read file");
		nx[0]=0;
		while(file.ReadString(strRead))
		{
			//TRACE("\n--%s", strRead.GetBuffer());

			token = wcstok( strRead.GetBuffer(), seps );

			if(NULL!=token)
				x= _wtof(token);

			token = wcstok( NULL, seps );
			if(NULL!=token)
				y = _wtof(token);

			//TRACE("%f,%f,%d",x[i],y[i],i);
			
			if(nx[0]<1){
				tmp=vector<T>(1,2);
				tmp[1]=x;
				tmp[2]=y;
			}
			else{
				tmpk=vector<T>(1,nx[0]+nx[0]+2);
				copyvt(tmp,nx[0],tmpk);
				tmpk[nx[0]+1]=x;
				copyvt(&tmp[nx[0]],nx[0],&tmpk[nx[0]+1]);
				tmpk[nx[0]+nx[0]+2]=y;
				free_vector(tmp,1,nx[0]+nx[0]);
				tmp=tmpk;
			}
			nx[0]++;	

		}
		TRACE("\n--End reading");
		file.Close();
		return tmp;
	}
	else{		
		return NULL;
	}

}



#endif