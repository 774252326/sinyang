
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

#include "MainFrm.h"
#include "ComputeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CanalyzerDoc

IMPLEMENT_DYNCREATE(CanalyzerDoc, CDocument)

	BEGIN_MESSAGE_MAP(CanalyzerDoc, CDocument)
		ON_COMMAND(ID_ANALYSIS_COMPUTE, &CanalyzerDoc::OnAnalysisCompute)
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

		CSingleLock singleLock(&m_CritSection);
		singleLock.Lock();
		if (singleLock.IsLocked())  // Resource has been locked
		{
			da.Clear();
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}

		return TRUE;
	}




	// CanalyzerDoc serialization

	void CanalyzerDoc::Serialize(CArchive& ar)
	{
		CSingleLock singleLock(&m_CritSection);
		singleLock.Lock();
		if (singleLock.IsLocked())  // Resource has been locked
		{
			da.Serialize(ar);
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}

		if (ar.IsStoring())
		{
			// TODO: add storing code here
		}
		else
		{
			// TODO: add loading code here

			UINT flg=da.ComputeStateData();
			Show();

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


	void CanalyzerDoc::Show(void)
	{
		POSITION pos = GetFirstViewPosition();
		if(pos!=NULL){
			CMainFrame *mf=(CMainFrame*)(GetNextView(pos)->GetParentFrame());

			CSingleLock singleLock(&m_CritSection);
			singleLock.Lock();
			if (singleLock.IsLocked())  // Resource has been locked
			{
				mf->GetOutputWnd()->GetListCtrl()->dol.assign(da.dol.begin(),da.dol.end());
				// Now that we are finished, 
				// unlock the resource for others.
				singleLock.Unlock();
			}

			::PostMessage(mf->GetOutputWnd()->GetListCtrl()->GetSafeHwnd(),
				MESSAGE_SHOW_DOL,
				NULL,
				NULL);
		}

	}


	void CanalyzerDoc::OnAnalysisCompute()
	{
		// TODO: Add your command handler code here

		ComputeDlg cpd;
		cpd.DoModal();

	}