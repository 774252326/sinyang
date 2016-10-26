
// analyzerDoc.cpp : implementation of the CanalyzerDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "analyzer.h"
#endif

#include "analyzerDoc.h"


#include "AnalysisParametersPage.h"
#include "CVParametersPage.h"
#include "SolutionAdditionParametersPageA.h"

#include "pcct.h"
#include "MainFrm.h"
#include "analyzerViewL.h"
#include "analyzerViewR.h"
#include "pdfout.h"
#include "func.h"
#include "PropertySheetA.h"


//void LoadFileList(const CString &m_filePath, std::vector<CString> &filelist)
//{
//
//	CString folderpath=m_filePath.Left(m_filePath.ReverseFind('\\'));
//
//	filelist.clear();
//	CStdioFile file;
//	BOOL readflag;
//	readflag=file.Open(m_filePath, CFile::modeRead);
//
//	if(readflag)
//	{	
//		CString strRead;
//		//TRACE("\n--Begin to read file");
//		while(file.ReadString(strRead)){
//			strRead=folderpath+"\\"+strRead;
//			filelist.push_back(strRead);
//		}
//		//TRACE("\n--End reading\n");
//		file.Close();
//	}
//}




#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CanalyzerDoc

IMPLEMENT_DYNCREATE(CanalyzerDoc, CDocument)

	BEGIN_MESSAGE_MAP(CanalyzerDoc, CDocument)
		ON_COMMAND(ID_ANALYSIS_METHODSETUP, &CanalyzerDoc::OnAnalysisMethodsetup)
		ON_COMMAND(ID_ANALYSIS_REPORT, &CanalyzerDoc::OnAnalysisReport)
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


			//raw.Serialize(ar);

		}
		else
		{
			// TODO: add loading code here

			//raw.Serialize(ar);
			//raw.LoadFromFileList(flistlist[p1.analysistype]);

			//size_t wo=15500;
			//
			//raw.xll.erase(raw.xll.end()-wo,raw.xll.end());
			//raw.yll.erase(raw.yll.end()-wo,raw.yll.end());
			//
			//while(raw.ll.back()<=wo){
			//	wo-=raw.ll.back();
			//	raw.ll.pop_back();
			//}
			//raw.ll.back()-=wo;
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

		//sheet.ModifyStyleEx(0, WS_EX_CONTEXTHELP); 
		//sheet.ModifyStyle(0, DS_CONTEXTHELP); 

		//sheet.dwFlags   |=   (~PSH_HASHELP); 
//sheet.m_psp.dwFlags   &=   (~PSP_HASHELP);
		//sheet.


		// 打开模态向导对话框   
		if(sheet.DoModal()==IDOK){
			p1=appage.para;
			p2=cppage.para;
			p3=sppage.para;

			POSITION pos = GetFirstViewPosition();
			CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
			CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());

			CanalyzerViewR* rv=((CanalyzerViewR*)GetNextView(pos));

			::SendMessage(mf->GetOutputWnd()->GetListCtrl()->GetSafeHwnd(),
				MESSAGE_UPDATE_DOL,
				NULL,
				NULL);

			str.LoadString(IDS_STRING_PARA_CHANGED);
			mf->GetCaptionBar()->ShowMessage(str);


		}
	}



	int CanalyzerDoc::pdfd(CString outfile, 
		bool b1,
		bool b2,
		bool b3,
		bool b4,
		bool b5,
		bool b6,
		bool b7
		)
	{

		const std::wstring searchpath = L"../data";

		const std::wstring temppdf = L"temp.pdf";

		pdflib::PDFlib p;

		std::wostringstream optlist;

		optlist.str(L"");

		p.set_parameter(L"errorpolicy", L"return");

		p.set_parameter(L"SearchPath", searchpath);

		if (p.begin_document(temppdf, optlist.str()) == -1) {
			//if (p.begin_document((LPCWSTR)outfile, L"") == -1) {
			std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
			return 2;
		}
		p.set_info(L"Creator", L"PDFlib starter sample");
		p.set_info(L"Title", L"starter_table");


		CString templogobmp=L"templogo.bmp";



		CBitmap bmp;
		bmp.LoadBitmap(IDB_BITMAP_SINYANG);
		CImage img;
		//img.LoadFromResource(NULL,IDB_BITMAP_SINYANG);
		img.Attach(HBITMAP(bmp));
		HRESULT hResult = img.Save((LPCTSTR)templogobmp);
		bmp.DeleteObject();
		//if (SUCCEEDED(hResult))


		POSITION pos = GetFirstViewPosition();
		CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
		CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());
		COutputList *ol=mf->GetOutputWnd()->GetListCtrl();

		CanalyzerViewR* rv=((CanalyzerViewR*)GetNextView(pos));


		int a;

		std::vector<CString> res;
		if(b2){
			bool flg=Compute(ol->dol, p1, res);
		}


		a=pdfout6(p,p1,res,p2,p3,b1,b2,b3,b4);

		if(b5){
			a=pdfout(p,ol->dol);
		}


		std::vector<PlotData> pdl;
		std::vector<CString> nl;

		if(b7){
			pdl.assign(lv->pdl.begin(),lv->pdl.end());
			CString str;
			str.LoadStringW(IDS_STRING_VOLTAMMOGRAM);
			nl.assign(lv->pdl.size(),str);
			if(b6){
				pdl.resize(rv->pdl.size()+pdl.size());
				std::copy_backward(rv->pdl.begin(),rv->pdl.end(),pdl.end());
				str.LoadStringW(IDS_STRING_TEST_CURVE);
				nl.resize(rv->pdl.size()+nl.size(),str);
			}
		}
		else{
			if(b6){
				pdl.assign(rv->pdl.begin(),rv->pdl.end());
				CString str;
				str.LoadStringW(IDS_STRING_TEST_CURVE);
				nl.assign(rv->pdl.size(),str);
			}
		}

		a=imgout2(p,lv->GetDC(),pdl,nl);

		p.end_document(optlist.str());

		AddPageNumber(temppdf,(LPCWSTR)outfile);

		CFile::Remove(temppdf.c_str());
		CFile::Remove(templogobmp);

		return 0;
	}

	void CanalyzerDoc::OnAnalysisReport()
	{
		// TODO: Add your command handler code here

		TCHAR szFilters[]= _T("PDF Files (*.pdf)|*.pdf||");

		CFileDialog se(FALSE,L"pdf",TimeString()+L".pdf",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilters);

		CString str;

		str.LoadStringW(IDS_STRING_ANALYSIS_PARA);
		se.AddCheckButton(IDS_STRING_ANALYSIS_PARA,str,TRUE);

		str.LoadStringW(IDS_STRING_CV_PARA);
		se.AddCheckButton(IDS_STRING_CV_PARA,str,TRUE);

		str.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
		se.AddCheckButton(IDS_STRING_ADDITION_SOLUTION_PARA,str,TRUE);

		str.LoadStringW(IDS_STRING_RESULT);
		se.AddCheckButton(IDS_STRING_RESULT,str,TRUE);

		str.LoadStringW(IDS_OUTPUT_WND);
		se.AddCheckButton(IDS_OUTPUT_WND,str,TRUE);

		str.LoadStringW(IDS_STRING_TEST_CURVE);
		se.AddCheckButton(IDS_STRING_TEST_CURVE,str,TRUE);

		str.LoadStringW(IDS_STRING_VOLTAMMOGRAM);
		se.AddCheckButton(IDS_STRING_VOLTAMMOGRAM,str,TRUE);


		if(se.DoModal()==IDOK){

			CString fp = se.GetPathName();

			BOOL chk1,chk2,chk3,chk4,chk5,chk6,chk7;
			se.GetCheckButtonState(IDS_STRING_ANALYSIS_PARA,chk1);
			se.GetCheckButtonState(IDS_STRING_CV_PARA,chk2);
			se.GetCheckButtonState(IDS_STRING_ADDITION_SOLUTION_PARA,chk3);
			se.GetCheckButtonState(IDS_STRING_RESULT,chk4);
			se.GetCheckButtonState(IDS_OUTPUT_WND,chk5);
			se.GetCheckButtonState(IDS_STRING_TEST_CURVE,chk6);
			se.GetCheckButtonState(IDS_STRING_VOLTAMMOGRAM,chk7);

			str.LoadString(IDS_STRING_REPORTING);
			str+=fp;
			POSITION pos = GetFirstViewPosition();
			CMainFrame *mf=(CMainFrame*)(GetNextView(pos)->GetParentFrame());
			mf->GetCaptionBar()->ShowMessage(str);

			//if(pdfd(fp,this)==0){
			if(pdfd(fp,chk1,chk2,chk3,chk4,chk5,chk6,chk7)==0){
				//AfxMessageBox(L"report "+fp+L" is saved");

				str.LoadStringW(IDS_STRING_REPORT);
				CString tmps;
				tmps.LoadStringW(IDS_STRING_IS_SAVED);

				mf->GetCaptionBar()->ShowMessage(str+L" "+se.GetFileName()+L" "+tmps);
				ShellExecute(NULL, L"open", fp, NULL, NULL, SW_SHOW);			
			}
			else{
				//AfxMessageBox(IDS_STRING_SAVE_ERROR);

				str.LoadString(IDS_STRING_SAVE_ERROR);
				mf->GetCaptionBar()->ShowMessage(str);
			}

		}



	}
