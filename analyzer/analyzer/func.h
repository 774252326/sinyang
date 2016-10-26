
#include "stdafx.h"
//#include "dlg1.h"
#include "analyzerDoc.h"
#include "analyzerView.h"
#include "analyzerViewR.h"
#include "OutputWnd.h"
#include "MFCCaptionBarA.h"
#include "ANPara.h"
#include "SAPara.h"
#include "VPara.h"
#include "typedefine.h"
//#include "PlotData.h"
#include <vector>
#include <algorithm>

//bool isend=false;
//bool waiting=false;

typedef struct MYPARA{
	CanalyzerView *leftp;
	CanalyzerViewR *rightp;
	CanalyzerDoc *adoc;
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
BOOL ReadFileCustom(CObject * co, size_t n, CString fp);
BOOL SaveFileCustom(CObject * co, size_t n, CString fp);

//void SortInsert( std::vector<double> &xs, std::vector<double> &ys, double x, double y );

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


template <typename T>
bool compless (T c1, T c2) {
	return (c1>c2);
}

template <typename T>
void SortInsert( std::vector<T> &xs, std::vector<T> &ys, T x, T y )
{
	std::vector<T>::iterator it;
	it = std::find_first_of (xs.begin(), xs.end(),	&x, (&x)+1, compless<T>);
	size_t idx=it-xs.begin();
	xs.insert(it,x);
	ys.insert(ys.begin()+idx,y);
}