
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

#include "PDFPrinter.hpp"

#include "analyzerView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CanalyzerDoc

IMPLEMENT_DYNCREATE(CanalyzerDoc, CDocument)

	BEGIN_MESSAGE_MAP(CanalyzerDoc, CDocument)
		ON_COMMAND(ID_ANALYSIS_COMPUTE, &CanalyzerDoc::OnAnalysisCompute)
		ON_COMMAND(ID_ANALYSIS_REPORT, &CanalyzerDoc::OnAnalysisReport)
	END_MESSAGE_MAP()


	// CanalyzerDoc construction/destruction

	CanalyzerDoc::CanalyzerDoc()
		: bChangeSAP(false)
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
		if (singleLock.Lock())  // Resource has been locked
		{
			da.Clear();
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}

		CSingleLock singleLockSAP(&m_CritSectionSAP);		
		if (singleLockSAP.Lock())  // Resource has been locked
		{
			p3todo.saplist.clear();
			bChangeSAP=false;
			// Now that we are finished, 
			// unlock the resource for others.
			singleLockSAP.Unlock();
		}



		return TRUE;
	}




	// CanalyzerDoc serialization

	void CanalyzerDoc::Serialize(CArchive& ar)
	{
		CSingleLock singleLock(&m_CritSection);
		if (singleLock.Lock())  // Resource has been locked
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

			UpdateState();
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

	UINT CanalyzerDoc::UpdateState(void)
	{
		UINT rs;
		CSingleLock singleLock(&m_CritSection);
		if (singleLock.Lock())  // Resource has been locked
		{
			rs=da.ComputeStateData();
			switch(da.runstate){
			case 0:
			case 5:
			case 7:
				{
					if(bChangeSAP){
						CSingleLock singleLockSAP(&m_CritSectionSAP);
						if (singleLockSAP.Lock())  // Resource has been locked
						{
							da.p3=da.p3done;
							da.p3.AppendData(p3todo);
							rs=da.ComputeStateData();
							bChangeSAP=false;
							// Now that we are finished, 
							// unlock the resource for others.
							singleLockSAP.Unlock();
						}
					}
				}
				break;
			default:
				break;
			}
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}

		return rs;
	}


	void CanalyzerDoc::SetSAPtodo(const SAPara & ptd)
	{
		CSingleLock singleLockSAP(&m_CritSectionSAP);
		if (singleLockSAP.Lock())  // Resource has been locked
		{


			POSITION pos = GetFirstViewPosition();
			if(pos!=NULL){
				CMainFrame *mf=(CMainFrame*)(GetNextView(pos)->GetParentFrame());
				if(mf->pst==pause){
					da.ChangeSAP(ptd);
					bChangeSAP=false;
					::PostMessage(mf->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);
				}
				else{
					p3todo=ptd;
					bChangeSAP=true;
				}
			}

			// Now that we are finished, 
			// unlock the resource for others.
			singleLockSAP.Unlock();
		}
	}

	void CanalyzerDoc::OnAnalysisReport()
	{
		// TODO: Add your command handler code here
		PDFPrinter pp;
		pp.ddex=da;

		POSITION pos = GetFirstViewPosition();
		if(pos!=NULL){
			CanalyzerView *lv=(CanalyzerView*)GetNextView(pos);
			pp.va.assign(lv->pdl.begin(),lv->pdl.end());
		}
		if(pos!=NULL){
			CanalyzerView *lv=(CanalyzerView*)GetNextView(pos);
			pp.tc.assign(lv->pdl.begin(),lv->pdl.end());
		}

		pp.work();

	}

	void CanalyzerDoc::ClearExpData(void)
	{
		CSingleLock singleLock(&m_CritSection);
		if (singleLock.Lock())  // Resource has been locked
		{
			da.raw.Clear();			
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}
	}
