
// z3Dlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "atltypes.h"


// Cz3Dlg dialog 

class Cz3Dlg : public CDialogEx
{
// Construction
public:
	Cz3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_Z3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton *pBtn;
	CButton *pPlot;
//	CButton cancelbtn;
//	CButton okbtn;
protected:
	
	afx_msg void OnMybut2();
	afx_msg void OnMybut1();
	afx_msg void OnBnClickedOpen();
public:
	virtual ~Cz3Dlg(void);
	//window rectangle
	CRect winrect;
	//button rectangle
	CRect btnrect;
	// distance between button and window edge 
	CSize towinedge;
	// distance between button and button edge
	CSize tobtnedge;
	
	double *x;
	double *y;
	long nd;
	CString m_filePath;
	bool isLoad;
	// number of data point
	long m_n;
	double m_xmin;
	double m_xmax;
	double m_ymin;
	double m_ymax;
	virtual void PreInitDialog();
	bool isInit;
	//plot region
	CRect plotrect;
	// open button rectangle
	CRect openrect;
	// file path edit rectangle
	CRect fprect;
	bool DrawXYAxis(CRect plotrect, CPaintDC * pdc);
	bool DrawXYAxis2(CRect plotrect, CPaintDC * pdc);

	// convert to window coordinate
	CPoint ptRsl(double x, double y, CRect can);
	void DrawPolyline(CRect plotrect, CPaintDC * pdc, CPen * pPen, double * x, double * y, long nd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	bool isMove;
	double xMove;
	double yMove;
	double xminMove;
	double yminMove;
	double xmaxMove;
	double ymaxMove;
	// convert to actual coordinate
	double xRsl(long x);
	// convert to actual coordinate
	double yRsl(long y);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CRect threct;
	// smooth button rectangle
	CRect smrect;
	// knee1 edit rectangle
	CRect k1rect;
	// knee2 edit rectangle
	CRect k2rect;
protected:
	afx_msg void OnBnClickedSmooth(void);
public:
	double threshold;
	double knee1;
	double knee2;
	long plnd;
	// smooth ployline index
	long *plind;
	double *plx;
	double *ply;
	bool isSmooth;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	long span;
	double* ys;
	double** ABSkPeak;
	long peaknd;

	void DrawVLine(CRect rect, CPaintDC * dc, CPen * pen, double x);
	void DrawDiff(CRect rect, CPaintDC * dc, CPen * pPen, double x1, double x2);
	double **xybreak;
	// computation part
	void cmpt(void);

	CPoint mouseDownPoint;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CRect redrawrect;
};
