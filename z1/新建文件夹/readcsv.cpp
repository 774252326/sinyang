#include "stdafx.h"

//read *.csv file. first column to x[1..n], second column to y[1..n]

bool readcsv(int n, LPCTSTR lpszFileName, float * x, float * y)
{
	CStdioFile file;
	if(file.Open(lpszFileName, CFile::modeRead))
	{	
		CString strRead;

		wchar_t *token;
		wchar_t seps[] = L",";
		int i=1;
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