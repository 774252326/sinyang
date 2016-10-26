
#include "stdafx.h"
#include "dlg1.h"
#include "MainFrm.h"


//bool isend=false;
//bool waiting=false;

typedef struct MYPARA{
	dlg1 *leftp;
	dlg1 *rightp;
	//COutputWnd *ow;
	//CMFCCaptionBarA *cba;
	CMainFrame *mf;
	//pcct *data;
	//pcctB *dataB;
	//CVPara *p2;
	//SAPara *p3;
} mypara;

BOOL writeini(CString fp, ANPara &p1t, CVPara &p2t, SAPara &p3t);
BOOL readini(CString fp, ANPara &p1t, CVPara &p2t, SAPara &p3t);
UINT RCCS(LPVOID pParam);
UINT ASDTM(LPVOID pParam);

UINT RIVLATM(LPVOID pParam);
UINT AALATM(LPVOID pParam);