
// analyzerView.cpp : implementation of the CanalyzerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "analyzer.h"
#endif

#include "analyzerDoc.h"
#include "analyzerView.h"
#include "Header1.h"
#include "PlotSettingPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COLORREF inv(const COLORREF &oc);

// CanalyzerView

IMPLEMENT_DYNCREATE(CanalyzerView, CFormView)

	BEGIN_MESSAGE_MAP(CanalyzerView, CFormView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_PAINT()
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		//		ON_WM_MOUSEMOVE()
		ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerView::OnOptionsPlotsettings)
		ON_WM_MOUSEMOVE()
		ON_WM_MOUSEWHEEL()
		ON_COMMAND(ID_VIEW_FITWINDOW, &CanalyzerView::OnViewFitwindow)
		ON_NOTIFY(UDN_DELTAPOS, 1, CanalyzerView::OnDeltaposSpin)
	END_MESSAGE_MAP()

	// CanalyzerView construction/destruction

	CanalyzerView::CanalyzerView()
		: CFormView(CanalyzerView::IDD)
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

	CanalyzerView::~CanalyzerView()
	{
	}

	void CanalyzerView::DoDataExchange(CDataExchange* pDX)
	{
		CFormView::DoDataExchange(pDX);
	}

	BOOL CanalyzerView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CFormView::PreCreateWindow(cs);
	}

	void CanalyzerView::OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();
		//GetParentFrame()->RecalcLayout();
		//ResizeParentToFit();

		LONG style0=GetWindowLongW(m_hWnd, GWL_EXSTYLE);
		SetWindowLongW(GetSafeHwnd(), GWL_EXSTYLE, style0|WS_CLIPCHILDREN);

	}

	void CanalyzerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CanalyzerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// CanalyzerView diagnostics

#ifdef _DEBUG
	void CanalyzerView::AssertValid() const
	{
		CFormView::AssertValid();
	}

	void CanalyzerView::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
	}

	CanalyzerDoc* CanalyzerView::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CanalyzerDoc)));
		return (CanalyzerDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CanalyzerView message handlers


	void CanalyzerView::OnPaint()
	{
		CPaintDC dc(this); // device context for painting
		// TODO: Add your message handler code here
		// Do not call CFormView::OnPaint() for painting messages

		CanalyzerDoc* pDoc=GetDocument();
		if(!pDoc->lp.empty()){
			//int ci=m_spBtn.GetPos32();
			CDC dcMem;                                                  //用于缓冲作图的内存DC
			CBitmap bmp;                                                 //内存中承载临时图象的位图
			CRect rect;
			GetClientRect(&rect);
			dcMem.CreateCompatibleDC(&dc);               //依附窗口DC创建兼容内存DC
			bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());//创建兼容位图
			dcMem.SelectObject(&bmp);                          //将位图选择进内存DC
			//dcMem.FillSolidRect(rect,pDoc->lp[ci].psp.bkgndC);//按原来背景填充客户区，不然会是黑色
			CRect plotrect=rect;

			DrawData(plotrect,&dcMem,pDoc->lp[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);

			dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
			dcMem.DeleteDC(); //删除DC
			bmp.DeleteObject(); //删除位图
		}
	}


	int CanalyzerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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


	void CanalyzerView::OnSize(UINT nType, int cx, int cy)
	{
		CFormView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		m_spBtn.MoveWindow(CRect(CPoint(cx,cy)-spBtnSize,spBtnSize));
		Invalidate();
	}

	void CanalyzerView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default
		m_mouseDownPoint=point;
		SetCapture();

		CFormView::OnLButtonDown(nFlags, point);
	}


	void CanalyzerView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default
		m_mouseDownPoint=CPoint(0,0);
		ReleaseCapture();

		CFormView::OnLButtonUp(nFlags, point);
	}


	void CanalyzerView::clear(void)
	{
		xmin=xmax=ymin=ymax=0;
		m_spBtn.SetRange32(0,0);
		m_spBtn.SetPos32(0);
		m_spBtn.ShowWindow(SW_HIDE);
		CanalyzerDoc* pDoc=GetDocument();
		pDoc->lp.clear();
	}


	bool CanalyzerView::updatePlotRange(int plotIndex, const std::vector<double> &x, const std::vector<double> &y, bool flg)
	{
		if(plotIndex==m_spBtn.GetPos32()){
			double pct=0.02;
			UpdateRange(x,xmin,xmax,pct,flg);
			UpdateRange(y,ymin,ymax,pct,flg);
			return true;
		}
		return false;
	}

	void CanalyzerView::OnOptionsPlotsettings()
	{
		// TODO: Add your command handler code here

		//AfxMessageBox(L"left");


		// 创建属性表对象   
		CString str;
		str.LoadStringW(IDS_STRING_POLT_SETTINGS);
		CPropertySheet sheet(str);
		//abc sheet(777);
		// 设置属性对话框为向导对话框   
		//sheet.SetWizardMode();   
		//sheet.SetWindowPos(&CWnd::wndTopMost,10,10,800,600,SWP_SHOWWINDOW);

		CanalyzerDoc* pDoc=GetDocument();
		if(!pDoc->lp.empty()){
			int il=m_spBtn.GetPos32();
			str.LoadStringW(IDS_STRING_FIGURE1);
			PlotSettingPage fig1setting(str
				,pDoc->lp[il].psp
				,pDoc->lp[il].ps
				,pDoc->lp[il].xlabel
				,pDoc->lp[il].ylabel);

			sheet.AddPage(&fig1setting);

			// 打开模态向导对话框   
			if(sheet.DoModal()==IDOK){

				pDoc->lp[il].xlabel=fig1setting.xlabel;
				pDoc->lp[il].ylabel=fig1setting.ylabel;
				pDoc->lp[il].psp=fig1setting.fs;
				pDoc->lp[il].ps.clear();
				pDoc->lp[il].ps.assign(fig1setting.ps.begin(),fig1setting.ps.end());
				Invalidate(FALSE);
			}
		}
	}


	void CanalyzerView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		CanalyzerDoc* pDoc=GetDocument();
		int idx=m_spBtn.GetPos32();

		if (GetCapture()==this && !pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){
			CRect plotrect;
			this->GetClientRect(&plotrect);
			if( MoveUpdate(plotrect
				, pDoc->lp[idx].psp.metricSize
				, pDoc->lp[idx].psp.labelSize
				, point
				, this->m_mouseDownPoint
				,xmin,xmax,ymin,ymax) ){
					this->Invalidate(FALSE);
			}
		}


		CFormView::OnMouseMove(nFlags, point);
	}


	BOOL CanalyzerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: Add your message handler code here and/or call default


		CanalyzerDoc* pDoc=GetDocument();
		int idx=m_spBtn.GetPos32();

		if(!pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){
		ScreenToClient(&pt);
		CRect plotrect;
		this->GetClientRect(&plotrect);
			if( WheelUpdate(plotrect
				, pDoc->lp[idx].psp.metricSize
				, pDoc->lp[idx].psp.labelSize
				, pt
				, ((zDelta>0)?0.8:1.25)
				,xmin,xmax,ymin,ymax) ){
					this->Invalidate(FALSE);
			}

		}

		return CFormView::OnMouseWheel(nFlags, zDelta, pt);
	}


	// update xmin,xmax,ymin,ymax
	bool CanalyzerView::updatePlotRange(int plotIndex, bool flg)
	{
		CanalyzerDoc* pDoc=GetDocument();
		if(pDoc->lp.empty())
			return false;

		return updatePlotRange(plotIndex,pDoc->lp[plotIndex].xll,pDoc->lp[plotIndex].yll,flg);
	}

	// update xmin,xmax,ymin,ymax
	bool CanalyzerView::updatePlotRange(bool flg)
	{
		CanalyzerDoc* pDoc=GetDocument();
		if(pDoc->lp.empty())
			return false;

		double pct=0.02;
		int ci=m_spBtn.GetPos32();
		UpdateRange(pDoc->lp[ci].xll,xmin,xmax,pct,flg);
		UpdateRange(pDoc->lp[ci].yll,ymin,ymax,pct,flg);
		return true;
	}


	int CanalyzerView::AddPlot(const PlotData & pda)
	{
		CanalyzerDoc* pDoc=GetDocument();
		pDoc->lp.push_back(pda);
		int newi=pDoc->lp.size()-1;
		SetSpin(newi);
		return newi;
	}

	void CanalyzerView::OnViewFitwindow()
	{
		// TODO: Add your command handler code here
	
		if(this->updatePlotRange())
			this->Invalidate(FALSE);

	}

	void CanalyzerView::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult)
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
	Invalidate(FALSE);
	
	*pResult = 0;
	}

	void CanalyzerView::SetSpin(int newi)
	{
		m_spBtn.SetRange32(0,newi);
		m_spBtn.SetPos32(newi);
		m_spBtn.ShowWindow( (newi>0 ? SW_SHOW : SW_HIDE) );
	}
