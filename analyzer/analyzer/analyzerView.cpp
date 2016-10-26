
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

#include "struct1\pcct.hpp"

#include "PropertySheetA.h"
#include "PlotSettingPageA.h"

#include "MainFrm.h"
#include "filefunc.h"
#include "calfunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SPIN_ID 1

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
		ON_COMMAND(ID_CONTROLS_1, &CanalyzerView::OnControls1)
		ON_WM_MOVE()
		ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerView::OnOptionsPlotsettings)
		ON_COMMAND(ID_VIEW_FITWINDOW, &CanalyzerView::OnViewFitwindow)
		ON_MESSAGE(MESSAGE_CHANGE_APPLOOK, &CanalyzerView::OnMessageChangeApplook)
		ON_MESSAGE(MESSAGE_UPDATE_VIEW, &CanalyzerView::OnMessageUpdateView)
		ON_NOTIFY(UDN_DELTAPOS, SPIN_ID, &CanalyzerView::OnDeltaposSpin)

		//ON_MESSAGE(MESSAGE_UPDATE_RAW, &CanalyzerView::OnMessageUpdateRaw)
	END_MESSAGE_MAP()

	// CanalyzerView construction/destruction

	CanalyzerView::CanalyzerView()
		: spBtnSize(CSize(23*2,23))
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

	void CanalyzerView::OnDraw(CDC* /*pDC*/)
	{
		CanalyzerDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: add draw code for native data here
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
		return DoPreparePrinting(pInfo);
	}

	void CanalyzerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add extra initialization before printing
	}

	void CanalyzerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
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


	int CanalyzerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		LONG style0=GetWindowLongW(this->GetSafeHwnd(), GWL_EXSTYLE);
		SetWindowLongW(GetSafeHwnd(), GWL_EXSTYLE, style0|WS_CLIPCHILDREN);

		//pw.pdex=NULL;
		if(pw.Create(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE /*| WS_DISABLED*/, CRect(), this, 1234)==FALSE)
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

		//CRect rc(0,0,cx,cy);
		//rc.BottomRight()-=spBtnSize;
		//pw.MoveWindow(&rc);
		m_spBtn.MoveWindow(CRect(CPoint(cx,cy)-spBtnSize,spBtnSize));
		pw.SetWindowPos(&CWnd::wndBottom, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE );
		//m_spBtn.SetWindowPos (&CWnd::wndTopMost, cx-spBtnSize.cx, cy-spBtnSize.cy, cx, cy, SWP_NOMOVE | SWP_SHOWWINDOW   );
		//m_spBtn.SetWindowPos (&CWnd::wndTopMost, 0, 0, spBtnSize.cx, spBtnSize.cy,  SWP_SHOWWINDOW   );
	}


	void CanalyzerView::OnControls1()
	{
		// TODO: Add your command handler code here


		TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");

		// Create an Open dialog; the default file name extension is ".my".

		CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT , szFilters);

		// Display the file dialog. When user clicks OK, fileDlg.DoModal() 

		// returns IDOK.

		if(fileDlg.DoModal() == IDOK)
		{   

			//POSITION pos=fileDlg.GetStartPosition();
			////PlotData pd;
			////pd.ps=pw.pdex->pd.ps;



			//while(pos!=NULL){
			//	CString fp=fileDlg.GetNextPathName(pos);

			//	pcct a;
			//	a.readFile(fp);
			//	a.TomA();
			//	a.SetTimeIntv();

			//	LineSpec ls;
			//	ls.colour=RGB(255,0,0);
			//	ls.name=L"line";

			//	pdl.push_back(PlotDataEx(*pw.GetPlotSpec()));
			//	pdl.back().pd.AddNew(a.time,a.current,ls,a.label[0],a.label[1]);

			//}

			////pw.pdex->lgc.legendDpMode=LEGEND_DP_SHOW
			////|LEGEND_DP_FIT_RECT|LEGEND_DP_AUTO_RECT
			////|LEGEND_DP_ALIGN|LEGEND_DP_TOP
			////;

			////pw.pdex->pd=pd;
			//////pw.lgs.bkColor=pw.pd.ps.bkgndC;
			////pw.ResetRange();
			////pw.SetLegend();

			////this->Invalidate(FALSE);
			//::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,NULL,NULL);
			//////////////////////////////////////////////////////////////////////////


			std::vector<CString> filelist;
			LoadFileList(fileDlg.GetPathName(),filelist);

			CanalyzerDoc* pDoc = GetDocument();

			while(!filelist.empty()){
				pcct a;
				a.readFile(filelist.front());
				a.TomA();
				a.SetTimeIntv();

				pDoc->raw.AddNew(a.potential,a.current);

				filelist.erase(filelist.begin());
			}
			
			CMainFrame *mf=(CMainFrame*)(GetParentFrame());

			::SendMessage(mf->GetOutputWnd()->GetSafeHwnd(),MESSAGE_UPDATE_DOL,NULL,NULL);
			
		}


		//CMainFrame *mf=(CMainFrame*)(GetParentFrame());
		//::SendMessage(mf->GetOutputWnd()->GetSafeHwnd(),MESSAGE_UPDATE_DOL,NULL,NULL);

	}


	void CanalyzerView::OnMove(int x, int y)
	{
		CView::OnMove(x, y);

		// TODO: Add your message handler code here

		//LPARAM pt=MAKELPARAM((short)(x),(short)(y));
		//::PostMessage(pw.GetSafeHwnd(),WM_MOVE,NULL,pt);

	}


	void CanalyzerView::OnOptionsPlotsettings()
	{
		// TODO: Add your command handler code here


		if(pdl.empty()){



			PropertySheetA1 sheet(IDS_STRING_POLT_SETTINGS);



			PlotSettingPage fig1setting;

			//fig1setting.lgc=pw.pdex->lgc;
			//fig1setting.lgs=pw.pdex->lgs;
			fig1setting.fs=pw.blankPS;
			sheet.AddPage(&fig1setting);

			// 打开模态向导对话框   
			if(sheet.DoModal()==IDOK){

				pw.blankPS=fig1setting.fs;
				pw.Invalidate(FALSE);

			}


			return;

		}


		// 创建属性表对象   
		CString str;


		PropertySheetA1 sheet(IDS_STRING_POLT_SETTINGS);

		str.LoadStringW(IDS_STRING_FIGURE1);
		str=L"";

		PlotSettingPage fig1setting(str,pw.pdex->pd.ps,pw.pdex->pd.ls);

		fig1setting.lgc=pw.pdex->lgc;
		fig1setting.lgs=pw.pdex->lgs;

		sheet.AddPage(&fig1setting);

		// 打开模态向导对话框   
		if(sheet.DoModal()==IDOK){

			pw.pdex->pd.ps=fig1setting.fs;
			pw.pdex->pd.ls.assign(fig1setting.ps.begin(),fig1setting.ps.end());
			pw.pdex->lgc=fig1setting.lgc;
			pw.pdex->lgs=fig1setting.lgs;

			pw.SetLegend();
			pw.Invalidate(FALSE);

		}

	}

	afx_msg LRESULT CanalyzerView::OnMessageChangeApplook(WPARAM wParam, LPARAM lParam)
	{


		COLORREF oc=(COLORREF)wParam;

		//pw.blankPS.winbkC=oc;
		//pw.blankPS.SetCr(pw.blankPS.GetCrType());
		pw.blankPS.RefreshWinCr(oc);

		for(size_t i=0;i<pdl.size();i++){
			//pdl[i].pd.ps.winbkC=oc;
			//pdl[i].pd.ps.SetCr(pdl[i].pd.ps.GetCrType());
			pdl[i].pd.ps.RefreshWinCr(oc);
		}

		pw.Invalidate(FALSE);

		return 0;
	}


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

	afx_msg LRESULT CanalyzerView::OnMessageUpdateView(WPARAM wParam, LPARAM lParam)
	{
		bool blast=wParam;

		int selecti=UpdateSpinButton(pdl.size(),blast);

		//if(selecti<0){
		//	//xmin=ymin=-1;
		//	//xmax=ymax=1;
		//	//PlotSpec ps=pw.pdex->pd.ps;
		//	//pw.pdex=PlotDataEx(ps);
		//	pw.pdex=NULL;
		//	//pw.selectPIdx=0;
		//	
		//	//pw.Invalidate(FALSE);	
		//	//return 1;
		//}
		pw.pdex=(selecti<0)? NULL : &pdl[selecti];
		//pw.pdex=&pdl[selecti];
		pw.selectPIdx=0;
		pw.bMouseCursor=false;

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


	afx_msg LRESULT CanalyzerView::OnMessageUpdateRaw(WPARAM wParam, LPARAM lParam)
	{
		CanalyzerDoc* pDoc = GetDocument();

		CMainFrame *mf=(CMainFrame*)(GetParentFrame());
		//COutputListA* ol=;

		pdl.clear();
		UINT flg=RawData2PlotDataList(pDoc->raw, mf->GetOutputWnd()->dol, pw.GetPlotSpec()->winbkC, pdl);

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,(WPARAM)true,NULL);

		return 0;
	}