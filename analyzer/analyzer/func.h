
//#include "stdafx.h"
//#include "dlg1.h"
#include "analyzerDoc.h"
#include "analyzerView.h"
#include "analyzerViewL.h"
#include "analyzerViewR.h"
#include "OutputWnd.h"
#include "MFCCaptionBarA.h"
#include "ANPara.h"
#include "SAPara.h"
#include "VPara.h"
#include "typedefine.h"
#include "PlotData.h"
#include <vector>
#include <algorithm>

//bool isend=false;
//bool waiting=false;

#define SC_STEP_COMPLETE 0x01
#define SC_NEW_RIGHT_PLOT 0x02
#define SC_NO_PLOT 0x04
#define SC_PLOT_LAST 0x08
#define SC_NEW_LINE 0x10
#define SC_NEW_ONCE 0x20


#define PF_Q_NORMALIZED 0x04
#define PF_CONCERTRATION 0x08
#define PF_S 0x10
#define PF_A 0x20
#define PF_L 0x40
#define PF_SAMPLE 0x80


typedef struct MYPARA{
	CanalyzerViewL *leftp;
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

bool Compute1(const std::vector<DataOutA> &dol, const ANPara &p1, double &Sv, double &z);

bool Compute(const std::vector<DataOutA> &dol, const ANPara &p1, std::vector<CString> &res);

BOOL writeini( ANPara &p1t, CVPara &p2t, SAPara &p3t, CString fp=AnalysisSetupINI);
BOOL readini( ANPara &p1t, CVPara &p2t, SAPara &p3t, CString fp=AnalysisSetupINI);

UINT PROCESS(LPVOID pParam);
BOOL ReadFileCustom(CObject * co, size_t n, CString fp);
BOOL SaveFileCustom(CObject * co, size_t n, CString fp);

void DrawData(CRect &plotrect
	, CDC* pDC
	, const PlotData &pd
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, COLORREF winbkcr=0);

bool WheelUpdate(CRect &plotrect
	, int mtsz
	, int lbsz
	, CPoint pt
	, double k1
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax);

bool MoveUpdateA(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax);

bool MoveUpdateB(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, CString &str);

int MoveUpdateC(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, CString &str
	, const std::vector<double> &xl
	, const std::vector<double> &yl
	, CPoint &pt);

void GetPlotRect(CRect & plotRect
	, int labelSize
	, int metricSize);



int DownUpdate(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, bool bmouse
	, const std::vector<double> &xl
	, const std::vector<double> &yl
	, size_t &index);

void DrawData1(CRect &plotrect
	, CDC* pDC
	, const double &x
	, const double &y
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, COLORREF textColor=0);

inline COLORREF inv(const COLORREF &oc);


HCURSOR BigCross(const CRect &rect, const CPoint & pt);

void AdjustWidth(CListCtrl *ls, int nCol, CString str, int gap=12);
void AdjustWidth(CListCtrl *ls, int nCol, int nRow, int gap=12);

CString TimeString(bool bSeg=false);


//typedef struct ANRESULT{
//	CString itemName;
//	CString itemUnit;
//	double itemData;
//} ANRes;












template <typename T>
void UpdateRange( const std::vector<T> &x, T &xmin, T &xmax, T gapPercent=0, bool bLocalRange=false)
{
	if(x.empty()){
		return;
	}
	
	auto resultx=std::minmax_element(x.begin(),x.end());
	T tmin=*resultx.first;
	T tmax=*resultx.second;
	
	T iv=tmax-tmin;
	iv*=gapPercent;
	tmin-=iv;
	tmax+=iv;
	if(bLocalRange||xmin>tmin)
		xmin=tmin;
	if(bLocalRange||xmax<tmax)
		xmax=tmax;

	if(xmin==xmax){
		xmin-=1;
		xmax+=1;
	}

}




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