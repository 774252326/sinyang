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
	float m_xbottom;
	float m_xtop;
	CString m_fileName;
	afx_msg void OnChangeEdit2();
	afx_msg void OnPaint();
	afx_msg void OnChangeEdit3();
	float piby50;
	float tf(void);
	float tf2(float x);
	afx_msg void OnBnClickedButton1();
	float *x;
	float* y;
	long m_n;
	afx_msg void OnChangeEdit4();
	bool readcsv(int n, LPCTSTR lpszFileName, float * x, float * y);
	float findmax(int n, float * x);
	float findmin(int n, float* x);
	CPoint ptRescale(CPoint pt, CRect can, CRect plotcan);
	float m_xmin;
	float m_xmax;
	float m_ymin;
	float m_ymax;
	afx_msg void OnChangeEdit5();
	afx_msg void OnChangeEdit6();
	afx_msg void OnChangeEdit7();
	afx_msg void OnChangeEdit8();
	bool isLoad;
	CPoint ptRescale(float x, float y, CRect can, float xmin, float xmax, float ymin, float ymax);

private:
	bool DrawAxis(CRect rect);
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
	CPoint ptRescale(float x, float y, CRect rect);
	CRect plotrect;
	float xRescale(int x);
	float yRescale(int y);
	float x1;/*xbottom*/
	float x2;/*xtop*/
	float reso;
	int calgrid(float d);
	int m_m;
	afx_msg void OnBnClickedButton2();
	float *a;
	float xp;
	long findbottomidx(float xbottom, float * x, long nd);
	long findtopidx(float xtop, float * x, long nd);
	bool DrawFittingCurve(CRect rect, CPaintDC * dc);
	bool isFit;
};

float nfun(float x[], float y[], long starti, long endi, int m, float a[]);

float calp(float a[], int ma, float x);