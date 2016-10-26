
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "property\PlotSettingPageB.h"
#include "property\PlotSettingPageC.h"
#include "property\PropertySheetA.h"
#include "export\ExportDataDlg.hpp"




// CanalyzerView

IMPLEMENT_DYNCREATE(CanalyzerView, CView)

	BEGIN_MESSAGE_MAP(CanalyzerView, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CanalyzerView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_MESSAGE(MESSAGE_CHANGE_APPLOOK, &CanalyzerView::OnMessageChangeApplook)
		ON_MESSAGE(MESSAGE_UPDATE_VIEW, &CanalyzerView::OnMessageUpdateView)
		ON_NOTIFY(UDN_DELTAPOS, SPIN_ID, &CanalyzerView::OnDeltaposSpin)
		ON_COMMAND(ID_VIEW_FITWINDOW, &CanalyzerView::OnViewFitwindow)
		ON_COMMAND(ID_EDIT_COPY, &CanalyzerView::OnEditCopy)
		ON_COMMAND(ID_VIEW_DATACURSOR, &CanalyzerView::OnViewDatacursor)
		ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerView::OnOptionsPlotsettings)
		ON_COMMAND(ID_ANALYSIS_EXPORTDATA, &CanalyzerView::OnAnalysisExportdata)
	END_MESSAGE_MAP()

	// CanalyzerView construction/destruction

	CanalyzerView::CanalyzerView()
		: spBtnSize(CSize(23*2,23))
		, bPrint(false)
	{
		// TODO: add construction code here

	}

	CanalyzerView::~CanalyzerView()
	{
	}

	BOOL CanalyzerView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CView::PreCreateWindow(cs);
	}

	// CanalyzerView drawing

	void CanalyzerView::OnDraw(CDC* pDC)
	{
		CanalyzerDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: add draw code for native data here

		if(bPrint && pw.pdex!=NULL){
			//CRect rect;
			//this->GetClientRect(&rect);
			//pw.pdex->DrawEx(rect,pDC);
			//bPrint=false;
		}
	}


	// CanalyzerView printing


	void CanalyzerView::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL CanalyzerView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		//return TRUE;
		return DoPreparePrinting(pInfo);

	}

	void CanalyzerView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
	{
		// TODO: add extra initialization before printing
		//if(pw.pdex!=NULL){
		//	pw.pdex->SaveImage(L"a.bmp",CSize(800,600),pDC);
		//	ShellExecute(NULL, L"print", L"a.bmp", NULL, NULL, SW_SHOW);	
		//}
		bPrint=true;
	}

	void CanalyzerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
		bPrint=false;
	}

	void CanalyzerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CanalyzerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
		CMenu menu;
		menu.LoadMenuW(IDR_POPUP_EDIT);

#ifndef SHARED_HANDLERS
		//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
		theApp.GetContextMenuManager()->ShowPopupMenu(menu.GetSubMenu(0)->GetSafeHmenu(), point.x, point.y, this, TRUE);
#endif
	}


	// CanalyzerView diagnostics

#ifdef _DEBUG
	void CanalyzerView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CanalyzerView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CanalyzerDoc* CanalyzerView::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CanalyzerDoc)));
		return (CanalyzerDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CanalyzerView message handlers

	int CanalyzerView::UpdateSpinButton(int np, bool bLast)
	{
		int selecti;	
		if(np>1){
			m_spBtn.SetRange32(0,np-1);
			m_spBtn.ShowWindow(SW_SHOW);
			if(bLast){
				selecti=np-1;
			}
			else{
				selecti=m_spBtn.GetPos32();
				if(selecti<0 || selecti>=np){
					selecti=0;				
				}
			}
		}
		else{
			m_spBtn.SetRange32(0,0);
			m_spBtn.ShowWindow(SW_HIDE);
			selecti=np-1;			
		}

		m_spBtn.SetPos32(selecti);
		return selecti;

	}

	int CanalyzerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here


		LONG style0=GetWindowLongW(this->GetSafeHwnd(), GWL_EXSTYLE);
		SetWindowLongW(GetSafeHwnd(), GWL_EXSTYLE, style0|WS_CLIPCHILDREN);

		//pw.pdex=NULL;
		if(pw.Create(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE /*| WS_DISABLED*/, CRect(), this, PWIN_ID)==FALSE)
			return -1;

		if(	m_spBtn.CreateEx(
			WS_EX_TOPMOST			
			//|WS_EX_TRANSPARENT
			,WS_CHILD
			|WS_VISIBLE
			|UDS_HORZ
			|UDS_WRAP
			, CRect()
			, this
			, SPIN_ID
			)==FALSE )
			return -1;

		m_spBtn.SetRange32(0,0);


		return 0;
	}


	void CanalyzerView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		m_spBtn.MoveWindow(CRect(CPoint(cx,cy)-spBtnSize,spBtnSize));
		pw.SetWindowPos(&CWnd::wndBottom, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE );
	}


	afx_msg LRESULT CanalyzerView::OnMessageChangeApplook(WPARAM wParam, LPARAM lParam)
	{
		COLORREF oc=(COLORREF)wParam;
		pw.blankPS.RefreshWinCr(oc);

		for(size_t i=0;i<pdl.size();i++){
			pdl[i].pd.ps.RefreshWinCr(oc);
		}

		pw.Invalidate(FALSE);
		return 0;
	}

	afx_msg LRESULT CanalyzerView::OnMessageUpdateView(WPARAM wParam, LPARAM lParam)
	{

		int selecti=UpdateSpinButton(pdl.size(),wParam&PW_LAST);
		pw.pdex=(selecti<0)? NULL : &pdl[selecti];
		pw.selectPIdx=0;
		pw.bMouseCursor=false;

		if(wParam&PW_SHOW_ALL){
			pw.ResetRange();
		}

		pw.SetLegend();
		pw.Invalidate(FALSE);

		return 0;
	}

	void CanalyzerView::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult)
	{
		NM_UPDOWN* pNMUpDown=(NM_UPDOWN*)pNMHDR;	

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,NULL,NULL);

		*pResult = 0;
	}

	void CanalyzerView::OnViewFitwindow()
	{
		// TODO: Add your command handler code here

		pw.ResetRange();
		pw.Invalidate(FALSE);

	}

	void CanalyzerView::OnEditCopy()
	{
		// TODO: Add your command handler code here

		if(m_spBtn.IsWindowVisible()){
			m_spBtn.ShowWindow(SW_HIDE);
			this->UpdateWindow();
			CopyWndToClipboard(this);
			m_spBtn.ShowWindow(SW_SHOW);
		}
		else{
			CopyWndToClipboard(this);
		}

	}

	void CanalyzerView::OnViewDatacursor()
	{
		// TODO: Add your command handler code here
		pw.bMouseCursor=!pw.bMouseCursor;
	}


	void CanalyzerView::OnOptionsPlotsettings()
	{
		// TODO: Add your command handler code here

		PropertySheetA1 sheet(IDS_STRING_POLT_SETTINGS);
		PlotSettingPageB fig1setting;
		if(pdl.empty()){		
			fig1setting.fs=pw.blankPS;
		}
		else{
			fig1setting.fs=pw.pdex->pd.ps;

			fig1setting.lgc=pw.pdex->lgc;
			fig1setting.lgs=pw.pdex->lgs;			
		}
		PlotSettingPageC fig2setting;
		if(!pdl.empty()){
			fig2setting.ps.assign(pw.pdex->pd.ls.begin(),pw.pdex->pd.ls.end());
			fig2setting.bTwo=FALSE;
		}
		sheet.AddPage(&fig1setting);
		sheet.AddPage(&fig2setting);
		sheet.DoModal();

	}




	void CanalyzerView::OnAnalysisExportdata()
	{
		// TODO: Add your command handler code here

		ExportDataDlg edd;

		for(size_t i=0;i<pdl.size();i++){
			RawDataEx rdex(pdl[i].pd.raw,L"c",pdl[i].pd.ps.xlabel,pdl[i].pd.ps.ylabel);
			for(size_t j=0;j<pdl[i].pd.ls.size();j++){
				rdex.title[j]=pdl[i].pd.ls[j].name;
			}
			edd.lcl.rde.AppendData(rdex);
		}

		edd.DoModal();
	}


	void CanalyzerView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
	{
		// TODO: Add your specialized code here and/or call the base class



		pDC->SetMapMode(MM_ANISOTROPIC); //转换坐标映射方式
		CRect rect;
		this->GetClientRect(&rect);
		
		if(rect.IsRectEmpty()==TRUE)
			return;

		CSize wsize = rect.Size(); 
		pDC->SetWindowExt(wsize); 

		HDC hdc=::GetDC(this->GetSafeHwnd());
		int wmm=::GetDeviceCaps(hdc,HORZSIZE);
		int hmm=::GetDeviceCaps(hdc,VERTSIZE);
		int wpxl=::GetDeviceCaps(hdc,HORZRES);
		int hpxl=::GetDeviceCaps(hdc,VERTRES);
		int xLogPixPerInch0 = ::GetDeviceCaps(hdc,LOGPIXELSX); 
		int yLogPixPerInch0 = ::GetDeviceCaps(hdc,LOGPIXELSY); 
		::ReleaseDC(this->GetSafeHwnd(),hdc);

		//得到实际设备每逻辑英寸的象素数量
		int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX); 
		int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY); 
		//得到设备坐标和逻辑坐标的比例


		CSize vsize(wsize.cx * xLogPixPerInch/xLogPixPerInch0, wsize.cy * yLogPixPerInch/yLogPixPerInch0);
		pDC->SetViewportExt(vsize); //确定视口大小

		CView::OnPrepareDC(pDC, pInfo);
	}


	void CanalyzerView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
	{
		// TODO: Add your specialized code here and/or call the base class

		if(/*bPrint &&*/ pw.pdex!=NULL){
			CRect rect;
			this->GetClientRect(&rect);
			pw.pdex->DrawEx(rect,pDC);
			//bPrint=false;
		}

		CView::OnPrint(pDC, pInfo);
	}
