// PlotWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PlotWnd.h"
#include "drawfunc.h"
#include "resource.h"
#include "LegendDlg.h"

// PlotWnd

IMPLEMENT_DYNAMIC(PlotWnd, CWnd)

	PlotWnd::PlotWnd()
	: xmin(0)
	, xmax(0)
	, ymin(0)
	, ymax(0)
	, pct(0.010)
	, m_mouseDownPoint(CPoint())
	, bMouseCursor(false)
	, selectPIdx(0)
	, zoomrate(0.9)
	, wndPosition(0)
	, legendDpMode(0)
{
	td=NULL;

	lgs.bDock=false;
	//lgs.bkColor=pd.ps.bkgndC;
	//lgs.fontName=pd.ps.fontName;
	//lgs.fontSize=15;
	//lgs.gap=2;
	//lgs.lineLength=40;
	//lgs.position=CPoint(200,200);


	//CRect winrect(0,0,20,30);
	//				
	//				CSize sz=winrect.Size();
	//				//lgrect.right=lgrect.left+sz.cx/2;
	//				//lgrect.bottom=lgrect.top+sz.cy/2;

	//				lgrect.Size()=sz;

	lgrect=CRect(0,0,30,30);

}

PlotWnd::~PlotWnd()
{
}


BEGIN_MESSAGE_MAP(PlotWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// PlotWnd message handlers




void PlotWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages


	CRect rect;
	this->GetClientRect(&rect);	

	//DrawData(rect,&dc,pd,xmin,xmax,ymin,ymax);
	//if(bMouseCursor && !pd.ps.empty()
	//	&& selectPIdx>=0 && selectPIdx<pd.xll.size()){
	//		DrawData1(rect,&dc,pd.xll[selectPIdx],pd.yll[selectPIdx],xmin,xmax,ymin,ymax,inv(pd.psp.bkgndC));
	//}

	CSize winsz=rect.Size();

	CDC dcMem;//���ڻ�����ͼ���ڴ�DC
	dcMem.CreateCompatibleDC(&dc);//��������DC���������ڴ�DC

	CBitmap bmp;//�ڴ��г�����ʱͼ���λͼ
	bmp.CreateCompatibleBitmap(&dc,winsz.cx,winsz.cy);//��������λͼ
	dcMem.SelectObject(&bmp);  	//��λͼѡ����ڴ�DC


	DrawData(rect,&dcMem,pd,xmin,xmax,ymin,ymax);

	if(bMouseCursor && !pd.ls.empty()
		&& selectPIdx>=0 && selectPIdx<pd.raw.xll.size()){
			DrawData1(rect,&dcMem,pd.raw.xll[selectPIdx],pd.raw.yll[selectPIdx],xmin,xmax,ymin,ymax,inv(pd.ps.bkgndC));
	}

	dc.BitBlt(0,0,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
	//pDC->BitBlt(100,100,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨

	dcMem.DeleteDC(); //ɾ��DC
	bmp.DeleteObject(); //ɾ��λͼ




}


void PlotWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetFocus();

	if(!pd.ls.empty()){


		CRect plotrect;
		this->GetClientRect(&plotrect);	

		int re=DownUpdate(plotrect
			, pd.ps.metricSize
			, pd.ps.labelSize
			, pd.ps.metricGridLong
			, pd.ps.gap
			, point
			, m_mouseDownPoint
			, xmin, xmax, ymin, ymax
			, bMouseCursor
			, pd.raw.xll
			, pd.raw.yll
			, selectPIdx);

		switch(re){
		case 1:
			SetCapture();
			break;
		case 2:
			this->ClientToScreen(&m_mouseDownPoint);
			::SetCursorPos(m_mouseDownPoint.x,m_mouseDownPoint.y);
			Invalidate(FALSE);
			break;
		default:
			break;
		}

	}


	CWnd::OnLButtonDown(nFlags, point);
}


void PlotWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	ReleaseCapture();

	HCURSOR hCur  =  LoadCursor( NULL  , IDC_ARROW ) ;
	::SetCursor(hCur);


	CWnd::OnLButtonUp(nFlags, point);
}


void PlotWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	//size_t selectIdx=m_spBtn.GetPos32();
	if(!pd.ls.empty()){

		//if(pd!=NULL && !pd->ps.empty() ){
		CRect plotrect;
		this->GetClientRect(&plotrect);	
		//ScreenToClient(&point);

		if(GetCapture()==this){
			if(MoveUpdateA(plotrect
				, pd.ps.metricSize
				, pd.ps.labelSize
				, pd.ps.metricGridLong
				, pd.ps.gap
				, point
				, this->m_mouseDownPoint
				, xmin,xmax,ymin,ymax))
				this->Invalidate(FALSE);
		}
		else{

			CString str;
			if(MoveUpdateB(plotrect
				, pd.ps.metricSize
				, pd.ps.labelSize
				, pd.ps.metricGridLong
				, pd.ps.gap
				, point
				, this->m_mouseDownPoint
				, xmin,xmax,ymin,ymax
				, str))
				m_tool.UpdateTipText(str,this);

		}

	}


	CWnd::OnMouseMove(nFlags, point);
}


BOOL PlotWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	if(!pd.ls.empty()){

		//if(pd!=NULL && !pd->ps.empty() ){
		ScreenToClient(&pt);
		CRect plotrect;
		this->GetClientRect(&plotrect);
		if( WheelUpdate(plotrect
			, pd.ps.metricSize
			, pd.ps.labelSize
			, pd.ps.metricGridLong
			, pd.ps.gap
			, pt
			, ((zDelta>0)?zoomrate:1/zoomrate)
			,xmin,xmax,ymin,ymax) ){
				this->Invalidate(FALSE);
		}

	}


	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}




void PlotWnd::ResetRange(void)
{
	UpdateRange(pd.raw.xll,xmin,xmax,pct,true);
	UpdateRange(pd.raw.yll,ymin,ymax,pct,true);
}


BOOL PlotWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	m_tool.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}


BOOL PlotWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}


int PlotWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here



	m_tool.Create(this);
	m_tool.AddTool(this);
	m_tool.Activate(true);

	ModifyStyle(0, SS_NOTIFY);
	//this->SetFocus();
	return 0;
}


LRESULT PlotWnd::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//return HTCAPTION;

	return CWnd::OnNcHitTest(point);
}


void PlotWnd::OnMove(int x, int y)
{
	CWnd::OnMove(x, y);

	// TODO: Add your message handler code here

	TRACE("%d,%d\n",x,y);

	if(td!=NULL){
		CRect rc;
		td->GetWindowRect(&rc);
		rc.OffsetRect(x-wndPosition.x,y-wndPosition.y);
		td->MoveWindow(&rc);
	}

	wndPosition=CPoint(x,y);
}


void PlotWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if(td!=NULL){
		((LegendDlg*)td)->PositionWnd();
		((LegendDlg*)td)->Invalidate();
		//td->Invalidate();
	}


}


int PlotWnd::SetLegend(void)
{
	ShowLegend(pd.ps.legendPos!=0);
	return 0;
}


void PlotWnd::ShowLegend(bool bShow)
{

	if(bShow){

		if(td==NULL){
			td=new LegendDlg(this);
			td->Create(IDD_DIALOG1,this);
		}
		else{
			((LegendDlg*)td)->PositionWnd();
			((LegendDlg*)td)->Invalidate();
			//td->Invalidate();
		}
		td->ShowWindow(SW_SHOW);
	}
	else{

		if(td!=NULL){
			td->ShowWindow(SW_HIDE);
			delete td;
			td=NULL;
		}
	}
}



CRect PlotWnd::GetWindowPlotRect(bool bWnd)
{

	CRect plotrect;
	if(bWnd)
		this->GetWindowRect(&plotrect);
	else
		this->GetClientRect(&plotrect);

	GetPlotRect(plotrect,pd.ps.labelSize,pd.ps.metricSize,pd.ps.metricGridLong,pd.ps.gap);

	return plotrect;
}