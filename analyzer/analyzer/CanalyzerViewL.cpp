#include "StdAfx.h"

//// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
//// and search filter handlers and allows sharing of document code with that project.
//#ifndef SHARED_HANDLERS
//#include "analyzer.h"
//#endif

#include "CanalyzerViewL.h"
#include "typedefine.h"
#include "PlotSettingPage.h"

CanalyzerViewL::CanalyzerViewL(void)
	: idx(0)
{
}


CanalyzerViewL::~CanalyzerViewL(void)
{
}
BEGIN_MESSAGE_MAP(CanalyzerViewL, CanalyzerView)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_COMMAND(ID_VIEW_FITWINDOW, &CanalyzerViewL::OnViewFitwindow)
	ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerViewL::OnOptionsPlotsettings)
END_MESSAGE_MAP()


void CanalyzerViewL::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CanalyzerDoc* pDoc=GetDocument();

	if (GetCapture()==this && !pDoc->lp.empty() && !pDoc->lp[idx].ps.empty())
	{
		MoveUpdate(pDoc->lp[idx].psp.metricSize
			, pDoc->lp[idx].psp.labelSize
			, point);
	}

	CanalyzerView::OnMouseMove(nFlags, point);
}


BOOL CanalyzerViewL::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	ScreenToClient(&pt);

	CanalyzerDoc* pDoc=GetDocument();
	if(!pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){
		WheelUpdate(pDoc->lp[idx].psp.metricSize
			, pDoc->lp[idx].psp.labelSize
			, pt
			, ((zDelta>0)?0.8:1.25));
	}

	return CanalyzerView::OnMouseWheel(nFlags, zDelta, pt);
}


void CanalyzerViewL::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CanalyzerView::OnPaint() for painting messages

	CanalyzerDoc* pDoc=GetDocument();

	CDC dcMem;                                                  //用于缓冲作图的内存DC
	CBitmap bmp;                                                 //内存中承载临时图象的位图
	CRect rect;
	GetClientRect(&rect);
	dcMem.CreateCompatibleDC(&dc);               //依附窗口DC创建兼容内存DC
	bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());//创建兼容位图
	dcMem.SelectObject(&bmp);                          //将位图选择进内存DC
	dcMem.FillSolidRect(rect,pDoc->lp[idx].psp.bkgndC);//按原来背景填充客户区，不然会是黑色

	CRect plotrect;
	GetPlotRect(plotrect, pDoc->lp[idx].psp.labelSize, pDoc->lp[idx].psp.metricSize);

	if(!plotrect.IsRectEmpty() && !pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){

		DrawXYAxis(plotrect
			,&dcMem
			,pDoc->lp[idx].psp
			,pDoc->lp[idx].xlabel
			,pDoc->lp[idx].ylabel);

		CRect mainrt;
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&plotrect);	
		dcMem.GetClipBox(&mainrt);
		dcMem.SelectClipRgn(&rgn);

		std::vector<CPoint> pointlist;
		genPointToPlot(pDoc->lp[idx].xll
			,pDoc->lp[idx].yll
			,plotrect
			,pointlist);

		DrawCurve(&dcMem
			,pointlist
			,pDoc->lp[idx].ll
			,pDoc->lp[idx].ps
			,plotrect.left
			,plotrect.right);

		rgn.SetRectRgn(&mainrt);
		dcMem.SelectClipRgn(&rgn);

		CRect legendrect=DrawLegend( 
			CRect(plotrect.right-plotrect.Width()/4
			,plotrect.top
			,plotrect.right
			,plotrect.top+plotrect.Height()/4)
			, &dcMem
			, pDoc->lp[idx].ps
			, pDoc->lp[idx].psp.bkgndC);

	}

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
	dcMem.DeleteDC(); //删除DC
	bmp.DeleteObject(); //删除位图

}


void CanalyzerViewL::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown=(NM_UPDOWN*)pNMHDR;	

	CanalyzerDoc* pDoc=GetDocument();

	int n=pDoc->lp.size();
	int newpos=pNMUpDown->iPos+pNMUpDown->iDelta;
	if(newpos>=n)
		newpos=0;
	if(newpos<0)
		newpos=n-1;
	//int newpos=pNMUpDown->iPos;

	//if(updatePlotRange(newpos))
	//if(updatePlotRange())
	double pct=0.02;
	UpdateRange(pDoc->lp[newpos].xll,xmin,xmax,pct,true);
	UpdateRange(pDoc->lp[newpos].yll,ymin,ymax,pct,true);
	Invalidate();
	
	*pResult = 0;
}

// update xmin,xmax,ymin,ymax
bool CanalyzerViewL::updatePlotRange(int plotIndex, bool flg)
{
	CanalyzerDoc* pDoc=GetDocument();
	return CanalyzerView::updatePlotRange(plotIndex,pDoc->lp[plotIndex].xll,pDoc->lp[plotIndex].yll,flg);
}

// update xmin,xmax,ymin,ymax
bool CanalyzerViewL::updatePlotRange(bool flg)
{
	CanalyzerDoc* pDoc=GetDocument();
	double pct=0.02;
	int ci=m_spBtn.GetPos32();
	UpdateRange(pDoc->lp[ci].xll,xmin,xmax,pct,flg);
	UpdateRange(pDoc->lp[ci].yll,ymin,ymax,pct,flg);
	return true;
}


int CanalyzerViewL::AddPlot(const PlotData & pda)
{
	CanalyzerDoc* pDoc=GetDocument();
	pDoc->lp.push_back(pda);
	int newi=pDoc->lp.size()-1;
	m_spBtn.SetRange32(0,newi);
	m_spBtn.SetPos32(newi);
	m_spBtn.ShowWindow( (newi>0 ? SW_SHOW : SW_HIDE) );
	return newi;
}

void CanalyzerViewL::OnViewFitwindow()
{
	// TODO: Add your command handler code here

	if(this->updatePlotRange())
		this->Invalidate();

}


void CanalyzerViewL::OnOptionsPlotsettings()
{
	// TODO: Add your command handler code here


	AfxMessageBox(L"dfas");

	// 创建属性表对象   
	CString str;
	str.LoadStringW(IDS_STRING_POLT_SETTINGS);
	CPropertySheet sheet(str);
	//abc sheet(777);
	// 设置属性对话框为向导对话框   
	//sheet.SetWizardMode();   
	//sheet.SetWindowPos(&CWnd::wndTopMost,10,10,800,600,SWP_SHOWWINDOW);

	CanalyzerDoc* pDoc=GetDocument();
	int il=m_spBtn.GetPos32();
	str.LoadStringW(IDS_STRING_FIGURE1);
	PlotSettingPage fig1setting(str
		,pDoc->lp[il].psp
		,pDoc->lp[il].ps
		,pDoc->lp[il].xlabel
		,pDoc->lp[il].ylabel);

	//int ir=RightPlotPointer()->m_spBtn.GetPos32();
	//str.LoadStringW(IDS_STRING_FIGURE2);
	//PlotSettingPage fig2setting(str
	//	,RightPlotPointer()->fs
	//	,RightPlotPointer()->pdl[ir].ps
	//	,RightPlotPointer()->pdl[ir].xlabel
	//	,RightPlotPointer()->pdl[ir].ylabel);

	sheet.AddPage(&fig1setting);
	//sheet.AddPage(&fig2setting);

	// 打开模态向导对话框   
	if(sheet.DoModal()==IDOK){

		pDoc->lp[il].xlabel=fig1setting.xlabel;
		pDoc->lp[il].ylabel=fig1setting.ylabel;
		pDoc->lp[il].psp=fig1setting.fs;
		pDoc->lp[il].ps.clear();
		pDoc->lp[il].ps.assign(fig1setting.ps.begin(),fig1setting.ps.end());
		Invalidate();

		//RightPlotPointer()->pdl[ir].xlabel=fig2setting.xlabel;
		//RightPlotPointer()->pdl[ir].ylabel=fig2setting.ylabel;
		//copyfs(fig2setting.fs,RightPlotPointer()->fs);
		//RightPlotPointer()->pdl[ir].ps.clear();
		//RightPlotPointer()->pdl[ir].ps.assign(fig2setting.ps.begin(),fig2setting.ps.end());
		//RightPlotPointer()->Invalidate();
	}
}
