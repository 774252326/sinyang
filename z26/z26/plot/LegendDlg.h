#pragma once
#include "PlotWnd.h"



// LegendDlg dialog




class LegendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LegendDlg)
public:

public:
	LegendDlg(PlotWnd * pParent);   // standard constructor
	virtual ~LegendDlg();

	// Dialog Data
	//enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void PositionWnd(void);
	//int maxFsz;
	//int minFsz;
	//int axisW;
	////float ratio;
	CPoint GetPos(void);

protected:

	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	//CSize GetExtent(void);
	//int GetAutoFontSize(CSize limitsz, int maxFontSize=15, int minFontSize=1);

	afx_msg void OnMove(int x, int y);
	PlotWnd * ppw;
	virtual BOOL OnInitDialog();

private:
	bool bInitComplete;
};
