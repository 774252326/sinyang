
// analyzerDoc.cpp : implementation of the CanalyzerDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "analyzer.h"
#endif

#include "analyzerDoc.h"

#include <propkey.h>


#include "AnalysisParametersPage.h"
#include "CVParametersPage.h"
//#include "SolutionAdditionParametersPage.h"
#include "SolutionAdditionParametersPageA.h"
//#include "pcct.h"
//#include "colormapT.h"
#include "analyzerView.h"
#include "analyzerViewR.h"
//#include "func.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CanalyzerDoc

IMPLEMENT_DYNCREATE(CanalyzerDoc, CDocument)

	BEGIN_MESSAGE_MAP(CanalyzerDoc, CDocument)
		ON_COMMAND(ID_ANALYSIS_METHODSETUP, &CanalyzerDoc::OnAnalysisMethodsetup)
		//		ON_COMMAND(ID_ANALYSIS_STARTANALYSIS, &CanalyzerDoc::OnAnalysisStartanalysis)
	END_MESSAGE_MAP()


	// CanalyzerDoc construction/destruction

	CanalyzerDoc::CanalyzerDoc()
		//: ppa(NULL)
		: resultStr(_T(""))
	{
		// TODO: add one-time construction code here

	}

	CanalyzerDoc::~CanalyzerDoc()
	{
	}

	BOOL CanalyzerDoc::OnNewDocument()
	{
		if (!CDocument::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		p1=ANPara();
		p2=CVPara();
		p3=SAPara();
		dol.clear();
		lp.clear();
		rp.clear();	
		resultStr.Empty();

		POSITION pos = GetFirstViewPosition();
		CanalyzerView* lv=((CanalyzerView*)GetNextView(pos));
		lv->SetSpin(0);
		CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());
		mf->GetOutputWnd()->clear();
		CanalyzerViewR* rv=((CanalyzerViewR*)GetNextView(pos));
		rv->SetSpin(0);

		return TRUE;
	}




	// CanalyzerDoc serialization

	void CanalyzerDoc::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
			// TODO: add storing code here
			p1.Serialize(ar);
			p2.Serialize(ar);
			p3.Serialize(ar);
			ar<<resultStr;
			ar<<dol.size();
			for(size_t i=0;i<dol.size();i++){
				dol[i].Serialize(ar);
			}
			ar<<lp.size();
			for(size_t i=0;i<lp.size();i++){
				lp[i].Serialize(ar);
			}
			ar<<rp.size();
			for(size_t i=0;i<rp.size();i++){
				rp[i].Serialize(ar);
			}
		}
		else
		{
			// TODO: add loading code here
			p1.Serialize(ar);
			p2.Serialize(ar);
			p3.Serialize(ar);
			ar>>resultStr;
			size_t n;
			ar>>n;
			dol.assign(n,DataOut());
			for(size_t i=0;i<dol.size();i++){
				dol[i].Serialize(ar);
			}
			ar>>n;
			lp.assign(n,PlotData());
			for(size_t i=0;i<lp.size();i++){
				lp[i].Serialize(ar);
			}
			ar>>n;
			rp.assign(n,PlotData());
			for(size_t i=0;i<rp.size();i++){
				rp[i].Serialize(ar);
			}
			///////////////////////////////////////////////

			POSITION pos = GetFirstViewPosition();
			CanalyzerView* lv=((CanalyzerView*)GetNextView(pos));
			lv->SetSpin(lp.size()-1);
			lv->updatePlotRange();

			CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());
			COutputList* ol=mf->GetOutputWnd()->GetListCtrl();
			mf->GetCaptionBar()->ShowMessage(resultStr);

			CanalyzerViewR* rv=((CanalyzerViewR*)GetNextView(pos));
			rv->SetSpin(rp.size()-1);
			rv->updatePlotRange();

			for(size_t i=0;i<dol.size();i++){
				ol->InsertListCtrl(i,dol[i]);
			}

			//////////////////////////////////////////////

			////CString str=this->GetPathName();
			//POSITION pos = GetFirstViewPosition();
			//CView* pFirstView = GetNextView(pos);
			//{
			//	CString str=L"C:\\Users\\r8anw2x\\Desktop\\ss\\analyzer\\analyzer\\data\\DTR\\3360 base 25ml + 8mlcali8-5.txt";
			//	pcct dt1;
			//	dt1.readFile(str);
			//	dt1.TomA();
			//	PlotData p1;			
			//	LineSpec ps1;
			//	ps1.colour=genColor( genColorvFromIndex<float>( 0 ) ) ;
			//	ps1.dotSize=-1;
			//	ps1.name=str;
			//	ps1.lineType=0;
			//	ps1.smoothLine=0;
			//	ps1.traceLast=false;
			//	p1.AddNew(dt1.potential,dt1.current,ps1,dt1.label[0],dt1.label[1]);
			//	((CanalyzerView*)pFirstView)->AddPlot(p1);
			//}


			//{
			//	CString str=L"C:\\Users\\r8anw2x\\Desktop\\ss\\analyzer\\analyzer\\data\\DTR\\3360 base 25ml + 2mlcali8-5.txt";
			//	pcct dt1;
			//	dt1.readFile(str);
			//	dt1.TomA();
			//	PlotData p1;			
			//	LineSpec ps1;
			//	ps1.colour=genColor( genColorvFromIndex<float>( 0 ) ) ;
			//	ps1.dotSize=-1;
			//	ps1.name=str;
			//	ps1.lineType=0;
			//	ps1.smoothLine=0;
			//	ps1.traceLast=false;
			//	p1.AddNew(dt1.potential,dt1.current,ps1,dt1.label[0],dt1.label[1]);
			//	((CanalyzerView*)pFirstView)->AddPlot(p1);
			//}

			//if(((CanalyzerView*)pFirstView)->updatePlotRange())
			//	;
			////((CanalyzerView*)pFirstView)->Invalidate(FALSE);

		}
	}

#ifdef SHARED_HANDLERS

	// Support for thumbnails
	void CanalyzerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
	{
		// Modify this code to draw the document's data
		dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

		CString strText = _T("TODO: implement thumbnail drawing here");
		LOGFONT lf;

		CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
		pDefaultGUIFont->GetLogFont(&lf);
		lf.lfHeight = 36;

		CFont fontDraw;
		fontDraw.CreateFontIndirect(&lf);

		CFont* pOldFont = dc.SelectObject(&fontDraw);
		dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
		dc.SelectObject(pOldFont);
	}

	// Support for Search Handlers
	void CanalyzerDoc::InitializeSearchContent()
	{
		CString strSearchContent;
		// Set search contents from document's data. 
		// The content parts should be separated by ";"

		// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
		SetSearchContent(strSearchContent);
	}

	void CanalyzerDoc::SetSearchContent(const CString& value)
	{
		if (value.IsEmpty())
		{
			RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
		}
		else
		{
			CMFCFilterChunkValueImpl *pChunk = NULL;
			ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
			if (pChunk != NULL)
			{
				pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
				SetChunkValue(pChunk);
			}
		}
	}

#endif // SHARED_HANDLERS

	// CanalyzerDoc diagnostics

#ifdef _DEBUG
	void CanalyzerDoc::AssertValid() const
	{
		CDocument::AssertValid();
	}

	void CanalyzerDoc::Dump(CDumpContext& dc) const
	{
		CDocument::Dump(dc);
	}
#endif //_DEBUG


	// CanalyzerDoc commands


	void CanalyzerDoc::OnAnalysisMethodsetup()
	{
		// TODO: Add your command handler code here

		// 创建属性表对象   
		CString str;
		str.LoadStringW(IDS_STRING_ANALYSIS_SETUP);
		CPropertySheet sheet(str);
		//abc sheet(777);
		// 设置属性对话框为向导对话框   
		//sheet.SetWizardMode();   
		//sheet.SetWindowPos(&CWnd::wndTopMost,10,10,800,600,SWP_SHOWWINDOW);

		AnalysisParametersPage appage;
		appage.para=p1;
		sheet.AddPage(&appage);

		CVParametersPage cppage;
		cppage.para=p2;
		sheet.AddPage(&cppage);

		SolutionAdditionParametersPageA sppage;
		sppage.para=p3;
		sheet.AddPage(&sppage);

		// 打开模态向导对话框   
		if(sheet.DoModal()==IDOK){
			p1=appage.para;
			p2=cppage.para;
			p3=sppage.para;
		}
	}


