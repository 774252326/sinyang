#pragma once

#include "LineSpec.hpp"
#include "LegendSpec.h"
#include <vector>

// LegendDlgA dialog

class LegendDlgA : public CDialogEx
{
	DECLARE_DYNAMIC(LegendDlgA)

public:
	LegendDlgA(CWnd* pParent = NULL);   // standard constructor
	virtual ~LegendDlgA();

// Dialog Data
	//enum { IDD = IDD_LEGENDDLGA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	std::vector<LineSpec> ls;
	//CFont font;
	LegendSpec lgs;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CSize GetExtent(void);
	//BYTE legendDpMode;
	int GetAutoFontSize(CSize limitsz, int maxFontSize=15, int minFontSize=1);
	void Position(CPoint pt);
	afx_msg void OnMove(int x, int y);
};
