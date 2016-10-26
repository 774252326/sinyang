#include "stdafx.h"

//#include "afxmt.h"



#include "dlg1.h"
#include "MFCCaptionBarA.h"
#include "OutputWnd.h"
#include "pcct.h"
#include "pcctB.h"

#include "typedefine.h"
#include "colormapT.h"


CSemaphore semaphoreWrite(3,6); //资源最多访问线程2个，当前可访问线程数2个 
char g_Array[10]; 
int intv=100;
bool isend=false;


typedef struct MYPARA{
	dlg1 *leftp;
	dlg1 *rightp;
	COutputWnd *ow;
	CMFCCaptionBarA *cba;
	pcct *data;
	pcctB *dataB;
} mypara;

UINT RCCS(LPVOID pParam)
{
	//CEdit *pEdit=(CEdit*)pParam;
	//pEdit->SetWindowText(L"");



	dlg1 *leftp=((mypara*)pParam)->leftp;
	dlg1 *rightp=((mypara*)pParam)->rightp;
	COutputWnd *ow=((mypara*)pParam)->ow;
	CMFCCaptionBarA *cba=((mypara*)pParam)->cba;
	pcct *data=((mypara*)pParam)->data;
	pcctB *dataB=((mypara*)pParam)->dataB;


	WaitForSingleObject(semaphoreWrite.m_hObject,INFINITE);



	isend=false;

	size_t n1=80;

	std::vector<double> x;
	std::vector<double> y;
	size_t rn;
	//load n1 points
	rn=data->popData(x,y,n1);
	//plot points on plot1


	plotspec ps1;
	CString strTemp;
	ps1.colour=genColor( genColorvFromIndex<float>( leftp->pd.ps.size() ) ) ;
	//ps1.colour=genColorGray( genColorvFromIndex<float>( stepCount ) ) ;
	ps1.dotSize=-1;
	ps1.name=data->stepName;
	ps1.showLine=true;
	ps1.smoothLine=0;
	ps1.traceLast=true;
	leftp->pd.AddNew(x,y,ps1,data->label[0],data->label[1]);
	leftp->updatePlotRange();


	Sleep(intv);


	while(rn>0){
		rn=data->popData(x,y,n1);
		leftp->pd.AddFollow(x,y);
		leftp->updatePlotRange(x,y);

		Sleep(intv);
	}


	isend=true;

	ReleaseSemaphore(semaphoreWrite.m_hObject,1,NULL);
	return 0;

}



UINT freshp(LPVOID pParam)
{
	dlg1 *leftp=((dlg1*)pParam);
	//dlg1 *rightp=((mypara*)pParam)->rightp;
	//COutputWnd *ow=((mypara*)pParam)->ow;
	//CMFCCaptionBarA *cba=((mypara*)pParam)->cba;
	//pcct *data=((mypara*)pParam)->data;
	//pcctB *dataB=((mypara*)pParam)->dataB;


	WaitForSingleObject(semaphoreWrite.m_hObject,INFINITE);

	while(!isend){
		leftp->Invalidate();
		Sleep(40);
	}

	leftp->Invalidate();

	ReleaseSemaphore(semaphoreWrite.m_hObject,1,NULL);
	return 0;

}