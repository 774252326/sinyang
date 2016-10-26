#pragma once

#include <afxcmn.h>
#include <vector>

#include "analyzerViewL.h"
#include "analyzerViewR.h"
//#include "OutputWnd.h"
#include "list\OutputListA.h"
//#include "MainFrm.h"


enum ProcessState{
	stop,
	running,
	pause
};

typedef struct MYPARA{
	CanalyzerViewL *leftp;
	CanalyzerViewR *rightp;
	//CanalyzerDoc *adoc;
	//COutputWnd *outw;
	COutputListA* ol;
	//CMFCCaptionBarA *cba;
	//CMainFrame *mf;
	//pcct *data;
	//pcctB *dataB;
	//CVPara *p2;
	//SAPara *p3;
	ProcessState *psta;
} mypara;


BOOL ReadFileCustom(CObject * co, size_t n, CString fp);
BOOL WriteFileCustom(CObject * co, size_t n, CString fp);
void LoadFileList(const CString &m_filePath, std::vector<CString> &filelist);
UINT PROCESS(LPVOID pParam);