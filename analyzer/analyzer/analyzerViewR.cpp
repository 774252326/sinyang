
// analyzerViewR.cpp : implementation of the CanalyzerViewR class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "analyzer.h"
#endif

#include "analyzerDoc.h"
#include "analyzerViewR.h"
//#include "calgridT.h"
//#include "xRescaleT.h"
//#include "CSpline.h"
//#include "colormapT.h"
#include "typedefine.h"
#include "Header1.h"
#include "PlotSettingPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COLORREF inv(const COLORREF &oc);

// CanalyzerViewR

IMPLEMENT_DYNCREATE(CanalyzerViewR, CFormView)

	BEGIN_MESSAGE_MAP(CanalyzerViewR, CFormView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_PAINT()
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		//		ON_WM_MOUSEMOVE()
		ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerViewR::OnOptionsPlotsettings)
		ON_WM_MOUSEMOVE()
		ON_WM_MOUSEWHEEL()
		ON_COMMAND(ID_VIEW_FITWINDOW, &CanalyzerViewR::OnViewFitwindow)
		ON_NOTIFY(UDN_DELTAPOS, 1, CanalyzerViewR::OnDeltaposSpin)
	END_MESSAGE_MAP()

	// CanalyzerViewR construction/destruction

	CanalyzerViewR::CanalyzerViewR()
		: CFormView(CanalyzerViewR::IDD)
		, xmin(0)
		, xmax(0)
		, ymin(0)
		, ymax(0)
		, m_mouseDownPoint(0)
		//, metricGridLong(5)
		//, metricGridShort(3)
		//, gap0(10)
		//, lidx(0)
	{
		// TODO: add construction code here
		spBtnSize=CSize(23*2,23);
	}

	CanalyzerViewR::~CanalyzerViewR()
	{
	}

	void CanalyzerViewR::DoDataExchange(CDataExchange* pDX)
	{
		CFormView::DoDataExchange(pDX);
	}

	BOOL CanalyzerViewR::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CFormView::PreCreateWindow(cs);
	}

	void CanalyzerViewR::OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

		LONG style0=GetWindowLongW(m_hWnd, GWL_EXSTYLE);
		SetWindowLongW(GetSafeHwnd(), GWL_EXSTYLE, style0|WS_CLIPCHILDREN);

	}

	void CanalyzerViewR::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CanalyzerViewR::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// CanalyzerViewR diagnostics

#ifdef _DEBUG
	void CanalyzerViewR::AssertValid() const
	{
		CFormView::AssertValid();
	}

	void CanalyzerViewR::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
	}

	CanalyzerDoc* CanalyzerViewR::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CanalyzerDoc)));
		return (CanalyzerDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CanalyzerViewR message handlers


	void CanalyzerViewR::OnPaint()
	{
		CPaintDC dc(this); // device context for painting
		// TODO: Add your message handler code here
		// Do not call CFormView::OnPaint() for painting messages

		CanalyzerDoc* pDoc=GetDocument();
		if(!pDoc->rp.empty()){
			//int ci=m_spBtn.GetPos32();
			CDC dcMem;                                                  //用于缓冲作图的内存DC
			CBitmap bmp;                                                 //内存中承载临时图象的位图
			CRect rect;
			GetClientRect(&rect);
			dcMem.CreateCompatibleDC(&dc);               //依附窗口DC创建兼容内存DC
			bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());//创建兼容位图
			dcMem.SelectObject(&bmp);                          //将位图选择进内存DC
			//dcMem.FillSolidRect(rect,pDoc->rp[ci].psp.bkgndC);//按原来背景填充客户区，不然会是黑色
			CRect plotrect=rect;

			DrawData(plotrect,&dcMem,pDoc->rp[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);

			dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
			dcMem.DeleteDC(); //删除DC
			bmp.DeleteObject(); //删除位图
		}
	}


	int CanalyzerViewR::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CFormView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here
		if(m_spBtn.Create(WS_CHILD
			//|WS_VISIBLE
			|UDS_HORZ
			|UDS_WRAP
			//|WS_CLIPCHILDREN
			//|WS_CLIPSIBLINGS
			, CRect()
			, this
			, 1)==FALSE)
			return -1;

		m_spBtn.SetRange32(0,0);
		return 0;
	}


	void CanalyzerViewR::OnSize(UINT nType, int cx, int cy)
	{
		CFormView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		m_spBtn.MoveWindow(CRect(CPoint(cx,cy)-spBtnSize,spBtnSize));
		Invalidate(FALSE);
	}

	void CanalyzerViewR::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default
		m_mouseDownPoint=point;
		SetCapture();

		CFormView::OnLButtonDown(nFlags, point);
	}


	void CanalyzerViewR::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default
		m_mouseDownPoint=CPoint(0,0);
		ReleaseCapture();

		CFormView::OnLButtonUp(nFlags, point);
	}


	void CanalyzerViewR::clear(void)
	{
		xmin=xmax=ymin=ymax=0;
		m_spBtn.SetRange32(0,0);
		m_spBtn.SetPos32(0);
		m_spBtn.ShowWindow(SW_HIDE);
	}


	bool CanalyzerViewR::updatePlotRange(int plotIndex, const std::vector<double> &x, const std::vector<double> &y, bool flg)
	{
		if(plotIndex==m_spBtn.GetPos32()){
			double pct=0.02;
			UpdateRange(x,xmin,xmax,pct,flg);
			UpdateRange(y,ymin,ymax,pct,flg);
			return true;
		}
		return false;
	}

	void CanalyzerViewR::OnOptionsPlotsettings()
	{
		// TODO: Add your command handler code here

		AfxMessageBox(L"right");


		// 创建属性表对象   
		CString str;
		str.LoadStringW(IDS_STRING_POLT_SETTINGS);
		CPropertySheet sheet(str);
		//abc sheet(777);
		// 设置属性对话框为向导对话框   
		//sheet.SetWizardMode();   
		//sheet.SetWindowPos(&CWnd::wndTopMost,10,10,800,600,SWP_SHOWWINDOW);

		CanalyzerDoc* pDoc=GetDocument();
		if(!pDoc->rp.empty()){
			int il=m_spBtn.GetPos32();
			str.LoadStringW(IDS_STRING_FIGURE1);
			PlotSettingPage fig1setting(str
				,pDoc->rp[il].psp
				,pDoc->rp[il].ps
				,pDoc->rp[il].xlabel
				,pDoc->rp[il].ylabel);

			sheet.AddPage(&fig1setting);

			// 打开模态向导对话框   
			if(sheet.DoModal()==IDOK){

				pDoc->rp[il].xlabel=fig1setting.xlabel;
				pDoc->rp[il].ylabel=fig1setting.ylabel;
				pDoc->rp[il].psp=fig1setting.fs;
				pDoc->rp[il].ps.clear();
				pDoc->rp[il].ps.assign(fig1setting.ps.begin(),fig1setting.ps.end());
				Invalidate(FALSE);
			}
		}
	}


	void CanalyzerViewR::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		CanalyzerDoc* pDoc=GetDocument();
		int idx=m_spBtn.GetPos32();

		if (GetCapture()==this && !pDoc->rp.empty() && !pDoc->rp[idx].ps.empty()){
			CRect plotrect;
			this->GetClientRect(&plotrect);
			if( MoveUpdate(plotrect
				, pDoc->rp[idx].psp.metricSize
				, pDoc->rp[idx].psp.labelSize
				, point
				, this->m_mouseDownPoint
				,xmin,xmax,ymin,ymax) ){
					this->Invalidate(FALSE);
			}
		}


		CFormView::OnMouseMove(nFlags, point);
	}


	BOOL CanalyzerViewR::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: Add your message handler code here and/or call default


		CanalyzerDoc* pDoc=GetDocument();
		int idx=m_spBtn.GetPos32();

		if(!pDoc->rp.empty() && !pDoc->rp[idx].ps.empty()){
		ScreenToClient(&pt);
		CRect plotrect;
		this->GetClientRect(&plotrect);
			if( WheelUpdate(plotrect
				, pDoc->rp[idx].psp.metricSize
				, pDoc->rp[idx].psp.labelSize
				, pt
				, ((zDelta>0)?0.8:1.25)
				,xmin,xmax,ymin,ymax) ){
					this->Invalidate(FALSE);
			}

		}

		return CFormView::OnMouseWheel(nFlags, zDelta, pt);
	}


	// update xmin,xmax,ymin,ymax
	bool CanalyzerViewR::updatePlotRange(int plotIndex, bool flg)
	{
		CanalyzerDoc* pDoc=GetDocument();
		if(pDoc->rp.empty())
			return false;

		return updatePlotRange(plotIndex,pDoc->rp[plotIndex].xll,pDoc->rp[plotIndex].yll,flg);
	}

	// update xmin,xmax,ymin,ymax
	bool CanalyzerViewR::updatePlotRange(bool flg)
	{
		CanalyzerDoc* pDoc=GetDocument();
		if(pDoc->rp.empty())
			return false;

		double pct=0.02;
		int ci=m_spBtn.GetPos32();
		UpdateRange(pDoc->rp[ci].xll,xmin,xmax,pct,flg);
		UpdateRange(pDoc->rp[ci].yll,ymin,ymax,pct,flg);
		return true;
	}


	int CanalyzerViewR::AddPlot(const PlotData & pda)
	{
		CanalyzerDoc* pDoc=GetDocument();
		pDoc->rp.push_back(pda);
		int newi=pDoc->rp.size()-1;
		m_spBtn.SetRange32(0,newi);
		m_spBtn.SetPos32(newi);
		m_spBtn.ShowWindow( (newi>0 ? SW_SHOW : SW_HIDE) );
		return newi;
	}

	void CanalyzerViewR::OnViewFitwindow()
	{
		// TODO: Add your command handler code here
	
		if(this->updatePlotRange())
			this->Invalidate(FALSE);

	}

	void CanalyzerViewR::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown=(NM_UPDOWN*)pNMHDR;	

	CanalyzerDoc* pDoc=GetDocument();

	int n=pDoc->rp.size();
	int newpos=pNMUpDown->iPos+pNMUpDown->iDelta;
	if(newpos>=n)
		newpos=0;
	if(newpos<0)
		newpos=n-1;
	//int newpos=pNMUpDown->iPos;

	//if(updatePlotRange(newpos))
	//if(updatePlotRange())
	double pct=0.02;
	UpdateRange(pDoc->rp[newpos].xll,xmin,xmax,pct,true);
	UpdateRange(pDoc->rp[newpos].yll,ymin,ymax,pct,true);
	Invalidate(FALSE);
	
	*pResult = 0;
}