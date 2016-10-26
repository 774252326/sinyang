
#include "stdafx.h"
#include "dlg1.h"
#include "OutputWnd.h"
#include "MFCCaptionBarA.h"
#include "ANPara.h"
#include "SAPara.h"
#include "VPara.h"
#include "typedefine.h"
//#include "PlotData.h"


//bool isend=false;
//bool waiting=false;

typedef struct MYPARA{
	dlg1 *leftp;
	dlg1 *rightp;
	COutputWnd *outw;
	CMFCCaptionBarA *cba;
	//CMainFrame *mf;
	//pcct *data;
	//pcctB *dataB;
	//CVPara *p2;
	//SAPara *p3;
	ProcessState *psta;
} mypara;

const CString AnalysisSetupINI=L"as.stp.txt";

BOOL writeini( ANPara &p1t, CVPara &p2t, SAPara &p3t, CString fp=AnalysisSetupINI);
BOOL readini( ANPara &p1t, CVPara &p2t, SAPara &p3t, CString fp=AnalysisSetupINI);

UINT PROCESS(LPVOID pParam);


//UINT RCCS(LPVOID pParam);
//UINT ASDTM(LPVOID pParam);
//UINT RIVLATM(LPVOID pParam);
//UINT AALATM(LPVOID pParam);
//UINT CRCL(LPVOID pParam);
//UINT ALRCM(LPVOID pParam);
//
//
//DWORD WINAPI RCCS2(LPVOID pParam);
//DWORD WINAPI ASDTM2(LPVOID pParam);
//DWORD WINAPI RIVLATM2(LPVOID pParam);
//DWORD WINAPI AALATM2(LPVOID pParam);