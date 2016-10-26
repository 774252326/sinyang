#pragma once
#include "atltypes.h"


// CUpDlg dialog

class CUpDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpDlg)

public:
	CUpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUpDlg();

	// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_xbottom;
	double m_xtop;
	CString m_fileName;
	afx_msg void OnChangeEdit2();
	afx_msg void OnPaint();
	afx_msg void OnChangeEdit3();
	float piby50;
//	float tf(void);
//	float tf2(float x);
	afx_msg void OnBnClickedButton1();
	double*x;
	double*y;
	long m_n;
	afx_msg void OnChangeEdit4();
//	bool readcsv(int n, LPCTSTR lpszFileName, float * x, float * y);
	//	float findmax(int n, float * x);
	//	float findmin(int n, float* x);
//	CPoint ptRescale(CPoint pt, CRect can, CRect plotcan);
	double m_xmin;
	double m_xmax;
	double m_ymin;
	double m_ymax;
	afx_msg void OnChangeEdit5();
	afx_msg void OnChangeEdit6();
	afx_msg void OnChangeEdit7();
	afx_msg void OnChangeEdit8();
	bool isLoad;
//	CPoint ptRescale(float x, float y, CRect can, float xmin, float xmax, float ymin, float ymax);

private:
//	bool DrawAxis(CRect rect);
public:
	afx_msg void OnChangeEdit1();
	bool DrawAxis2(CRect rect, CPaintDC * dc);
	bool DrawCurve(CRect rect, CPaintDC * dc);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool isSelectBottom;
	int lineWidth;
	bool isSelectTop;
	bool DrawCursor(CRect rect, CPaintDC * dc);
	CPoint ptRsl(double x, double y,CRect can);
	CRect plotrect;
	double xRsl(long x);
	double yRsl(long y);
//	float x1;
	/*xbottom*/
//	float x2;
	/*xtop*/
	float reso;
	//	int calgrid(float d);
	//int m_m;
	double m_m; /*threshold*/
	afx_msg void OnBnClickedButton2();
	double *a;
	double xp;
	//	long findbottomidx(float xbottom, float * x, long nd);
	//	long findtopidx(float xtop, float * x, long nd);
	bool DrawFittingCurve(CRect rect, CPaintDC * dc);
	bool isFit;
	long m_span;
	long m_degree;
	double *ys;
	bool DrawSmoothCurve(CRect rect, CPaintDC * dc);
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnBnClickedButton3();
	bool isSmooth;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	double **covar;
	double chisq;/*sum of square error*/
	double chisqpp;/*average square error*/
	double xp2;
	// draw a vertical line at x of rectangle rect
	void DrawVLine(CRect rect, CPaintDC * dc, CPen * pen, double x);
	long nknee;
	double *xknee;
	double *yknee;
	void DrawPoint(CRect rect, CPaintDC * pdc, CPen * pPen, double x, double y);
	bool isMove;
	CPoint ptMove;
	double xminMove;
	double yminMove;
	double xmaxMove;
	double ymaxMove;
	double xMove;
	double yMove;
	void DrawPolyline(CRect rect, CPaintDC * dc, CPen * pPen, double * x, double * y, long nd);
	void DrawFunc(CRect rect, CPaintDC * pdc, CPen * pPen);
	double **coefs;
	double *xbreak;
	double *nxlmx;
	double *nxlmn;
	double *knee;
	long nlmx;
	long nlmn;
	long nd;
	double **nc;
	double *nlcm;
	double *xelbow;
	long nelbow;
	double *nx;
	double *nys;
	double **lmn;
	double **lmx;
	void DrawFunc2(CRect rect, CPaintDC * dc, CPen * pPen);
	void DrawDiff(CRect rect, CPaintDC * dc, CPen * pPen, double x1, double x2);
	// curvature
	double *curv;
};

