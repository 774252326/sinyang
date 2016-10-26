#include "StdAfx.h"
#include "LegendDlgB.h"


LegendDlgB::LegendDlgB(CWnd* pParent)
	: pwnd(pParent)
{
}


LegendDlgB::~LegendDlgB(void)
{
}
BEGIN_MESSAGE_MAP(LegendDlgB, LegendDlgA)
	ON_WM_MOVE()
END_MESSAGE_MAP()


void LegendDlgB::OnMove(int x, int y)
{

	LegendDlgA::OnMove(x, y);

	// TODO: Add your message handler code here

	CRect pwrc;
	pwnd->GetWindowRect(&pwrc);

	if(pwrc.PtInRect(CPoint(x,y))){
		TRACE("\nin");
		//CDialogEx::OnMove(x, y);
	}
	else{
		TRACE("\nout");
	}

}


BOOL LegendDlgB::OnInitDialog()
{
	LegendDlgA::OnInitDialog();

	// TODO:  Add extra initialization here




	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void LegendDlgB::Position(void)
{
	CPoint pt(lgs.position);
	pwnd->ClientToScreen(&pt);

	LegendDlgA::Position(pt);

	Invalidate();
}


CPoint LegendDlgB::GetPos(void)
{
	CRect rc;
	GetWindowRect(&rc);
	pwnd->ScreenToClient(&rc);
	lgs.position=rc.TopLeft();
	return lgs.position;
}


CPoint LegendDlgB::CalAlignPos(bool bTop, bool bLeft, int axisWidth)
{
	//CRect plotrect=;

	CSize sz=GetExtent();

		plotrect.DeflateRect(2,0,0,2);

		if( legendDpMode&LEGEND_DP_LEFT ){
			td->lgs.position.x=plotrect.left;
		}
		else{
			td->lgs.position.x=plotrect.right-sz.cx;
		}

		if( legendDpMode&LEGEND_DP_TOP ){
			td->lgs.position.y=plotrect.top;
		}
		else{
			td->lgs.position.y=plotrect.bottom-sz.cy;
		}

	return CPoint();
}
