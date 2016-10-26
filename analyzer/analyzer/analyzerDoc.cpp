
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

#include "PropertySheetA.h"
#include "AnalysisParametersPage.h"
#include "CVParametersPage.h"
#include "SolutionAdditionParametersPageA.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CanalyzerDoc

IMPLEMENT_DYNCREATE(CanalyzerDoc, CDocument)

BEGIN_MESSAGE_MAP(CanalyzerDoc, CDocument)
	ON_COMMAND(ID_ANALYSIS_METHODSETUP, &CanalyzerDoc::OnAnalysisMethodsetup)
END_MESSAGE_MAP()


// CanalyzerDoc construction/destruction

CanalyzerDoc::CanalyzerDoc()
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
	raw=RawData();


	return TRUE;
}


	void CanalyzerDoc::operator=(const CanalyzerDoc &src)
	{
		p1=src.p1;
		p2=src.p2;
		p3=src.p3;
		raw=src.raw;
	}


// CanalyzerDoc serialization

void CanalyzerDoc::Serialize(CArchive& ar)
{
		p1.Serialize(ar);
		p2.Serialize(ar);
		p3.Serialize(ar);
		raw.Serialize(ar);

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
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
		//CPropertySheet sheet(str);
			PropertySheetA1 sheet(str);

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

			//POSITION pos = GetFirstViewPosition();
			//CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
			//CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());

			//CanalyzerViewR* rv=((CanalyzerViewR*)GetNextView(pos));

			//::SendMessage(mf->GetOutputWnd()->GetListCtrl()->GetSafeHwnd(),
			//	MESSAGE_UPDATE_DOL,
			//	NULL,
			//	NULL);

			//str.LoadString(IDS_STRING_PARA_CHANGED);
			//mf->GetCaptionBar()->ShowMessage(str);


		}
	}