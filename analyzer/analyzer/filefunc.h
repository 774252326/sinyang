#pragma once

#include <afxcmn.h>
#include <vector>

//#include "analyzerViewL.h"
//#include "analyzerViewR.h"
//#include "OutputWnd.h"
//#include "list\OutputListA.h"
//#include "MainFrm.h"
//#include "WaitDlg.h"

//enum ProcessState{
//	stop,
//	running,
//	pause
//};
//
//typedef struct MYPARA{
//	CanalyzerViewL *leftp;
//	CanalyzerViewR *rightp;
//	//CanalyzerDoc *adoc;
//	//COutputWnd *outw;
//	COutputListA* ol;
//	//CMFCCaptionBarA *cba;
//	CMainFrame *mf;
//	//pcct *data;
//	//pcctB *dataB;
//	//CVPara *p2;
//	//SAPara *p3;
//	//WaitDlg *wd;
//	//ProcessState *psta;
//} mypara;


//typedef struct MYPARA1{
//	//CanalyzerViewL *leftp;
//	//CanalyzerViewR *rightp;
//	CanalyzerDoc *adoc;
//	//COutputWnd *outw;
//	//COutputListA* ol;
//	//CMFCCaptionBarA *cba;
//	//CMainFrame *mf;
//	//pcct *data;
//	//pcctB *dataB;
//	//CVPara *p2;
//	//SAPara *p3;
//	//WaitDlg *wd;
//	ProcessState *psta;
//	SAPara *pp3todo;
//} mypara1;


CString ChooseFolderDlg();
BOOL ReadFileCustom(CObject * co, size_t n, CString fp);
BOOL WriteFileCustom(CObject * co, size_t n, CString fp);
void LoadFileList(const CString &m_filePath, std::vector<CString> &filelist);
//bool GetCalibrationCurve(
//	CString calibrationfilepath, 
//	std::vector<PlotDataEx> &pdl, 
//	COLORREF bkc=RGB(255,255,255));

//UINT PROCESS(LPVOID pParam);

//UINT PROCESS1(LPVOID pParam);

