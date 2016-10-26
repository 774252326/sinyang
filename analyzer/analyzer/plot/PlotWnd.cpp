// PlotWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PlotWnd.h"

#include "LegendDlg.hpp"

PlotWnd::~PlotWnd()
{
	delete td;
};
///
/// \brief PlotWnd::GetLegendRect
/// 计算图例区域并更新图例属性
/// \param legendrect
/// 图例区域
/// \return
///
bool PlotWnd::GetLegendRect(CRect &legendrect)
{
	CDC *pDC=td->GetDC();
	if(pdex!=NULL && pDC!=NULL){
		CRect plotrect;
		GetClientRect(&plotrect);
		pdex->pd.ps.CalPlotRect(plotrect);			
		legendrect=pdex->SetLegendSpec(plotrect,pDC);

		td->ReleaseDC(pDC);

		//this->ClientToScreen(&legendrect);//for dlg only

		return true;
	}
	return false;
}
///
/// \brief PlotWnd::GetLegendPos
/// 取图例位置
/// \param pt
/// 图例位置
/// \return
///
bool PlotWnd::GetLegendPos(CPoint &pt)
{
	if(td!=NULL && pdex!=NULL){
		CRect rc;
		td->GetWindowRect(&rc);
		this->ScreenToClient(&rc);	
		pdex->lgs.position=pt=rc.TopLeft();
		return true;
	}
	return false;
}
///
/// \brief PlotWnd::OnMove
/// 控件移动时绑定图例位置
/// \param x
/// \param y
///
void PlotWnd::OnMove(int x, int y)
{
	CWnd::OnMove(x, y);

	// TODO: Add your message handler code here

	TRACE("%d,%d\n",x,y);

	if(td!=NULL){
		CRect rc;
		td->GetWindowRect(&rc);
		rc.OffsetRect(x-wndPosition.x,y-wndPosition.y);

		TRACE("[%d,%d,%d,%d,%d,%d]\n",rc.left,rc.top,rc.right,rc.bottom,x-wndPosition.x,y-wndPosition.y);

		td->MoveWindow(&rc);

	}

	wndPosition=CPoint(x,y);
}

///
/// \brief PlotWnd::OnSize
/// 控件区域变换时相应更新图例
/// \param nType
/// \param cx
/// \param cy
///
void PlotWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if(td!=NULL){
		CRect legendrect;
		bool flg=GetLegendRect(legendrect);
		td->MoveWindow(&legendrect);
		td->Invalidate();
	}

}
///
/// \brief PlotWnd::SetLegend
///刷新图例

void PlotWnd::SetLegend(void)
{

	if( pdex!=NULL && (pdex->lgc.legendDpMode&LEGEND_DP_SHOW) /*&& !pdex->pd.ls.empty()*/){

		if(td!=NULL){
			//((LegendDlg*)td)->PositionWnd();
			//((LegendDlg*)td)->Invalidate();
			CRect legendrect;
			bool flg=GetLegendRect(legendrect);
			if(flg)
			{
				td->MoveWindow(&legendrect);
				td->Invalidate();
				TRACE(L"move legend");
			}
		}
		else{
			td=new LegendDlg(this);
			td->Create(
				//LegendDlg::IDD
				IDD_DIALOG4
				,this);	

			TRACE(L"create legend");
		}
		td->ShowWindow(SW_SHOW);
	}
	else{
		if(td!=NULL){
			td->ShowWindow(SW_HIDE);
			delete td;
			td=NULL;

			TRACE(L"hide legend");
		}
	}
}

#include "../property/PlotSettingPageB.hpp"
#include "../property/PlotSettingPageC.hpp"
#include "../property/PropertySheetA.hpp"
///
/// \brief PlotWnd::PlotSettingSheet
/// 打开画图属性页
/// \param bTwo
/// 指示是否用双色着色
///
void PlotWnd::PlotSettingSheet(BOOL bTwo)
{
	PropertySheetA1 sheet(IDS_STRING_POLT_SETTINGS);
	PlotSettingPageB fig1setting;
	PlotSettingPageC fig2setting;

	if(pdex!=NULL){
		fig1setting.fs=pdex->pd.ps;
		fig1setting.lgc=pdex->lgc;
		fig1setting.lgs=pdex->lgs;	

		fig2setting.ps.assign(pdex->pd.ls.begin(),pdex->pd.ls.end());
		fig2setting.bTwo=bTwo;
		fig2setting.oldC=pdex->pd.GetOldCr();
		fig2setting.newC=pdex->pd.GetNewCr();
	}
	else{
		fig1setting.fs=blankPS;
	}

	sheet.AddPage(&fig1setting);
	sheet.AddPage(&fig2setting);
	sheet.DoModal();
}
