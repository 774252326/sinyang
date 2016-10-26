
// analyzerDoc.cpp : implementation of the CanalyzerDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "analyzer.h"
#endif

#include "pdfout.h"

#include "analyzerDoc.h"
#include "analyzerViewL.h"
#include "analyzerViewR.h"

#include <propkey.h>

#include "PropertySheetA.h"
#include "AnalysisParametersPage.h"
#include "CVParametersPage.h"
#include "SolutionAdditionParametersPageA.h"
#include "SolutionAdditionParametersPageB.h"
#include "calfunc.h"

#include "MainFrm.h"
//#include "plot\LegendDlg.hpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CanalyzerDoc

IMPLEMENT_DYNCREATE(CanalyzerDoc, CDocument)

	BEGIN_MESSAGE_MAP(CanalyzerDoc, CDocument)
		//ON_COMMAND(ID_ANALYSIS_METHODSETUP, &CanalyzerDoc::OnAnalysisMethodsetup)
		ON_BN_CLICKED(IDC_BUTTON1, &CanalyzerDoc::OnBnClickedButton1)
		ON_COMMAND(ID_CONTROLS_CHANGESAP, &CanalyzerDoc::OnControlsChangesap)
		ON_COMMAND(ID_ANALYSIS_REPORT, &CanalyzerDoc::OnAnalysisReport)
		ON_COMMAND(ID_CONTROLS_2, &CanalyzerDoc::OnControls2)
	END_MESSAGE_MAP()


	// CanalyzerDoc construction/destruction

	CanalyzerDoc::CanalyzerDoc()
		: psheetml(NULL)
		//, runstate(0)
		, bChangeSAP(false)
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
		//singleLock.Lock();

		//if (singleLock.IsLocked())  // Resource has been locked
		if(singleLock.Lock())
		{
			//...use the shared resource...
			p1=ANPara();
			p2=CVPara();
			p3=SAPara();
			raw=RawData();
			dol.clear();
			p3done=SAPara();
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();

			return TRUE;

		}

		return FALSE;



	}


	void CanalyzerDoc::operator=(const CanalyzerDoc &src)
	{
		CSingleLock singleLock(&m_CritSection);
		//singleLock.Lock();

		//if (singleLock.IsLocked())  // Resource has been locked
		if(singleLock.Lock())
		{
			//...use the shared resource...
			p1=src.p1;
			p2=src.p2;
			p3=src.p3;
			raw=src.raw;
			dol.clear();
			p3done.saplist.clear();
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}
	}


	// CanalyzerDoc serialization

	void CanalyzerDoc::Serialize(CArchive& ar)
	{

		CSingleLock singleLock(&m_CritSection);
		//singleLock.Lock();

		//if (singleLock.IsLocked())  // Resource has been locked
		if(singleLock.Lock())
		{
			//...use the shared resource...


			p1.Serialize(ar);
			p2.Serialize(ar);
			p3.Serialize(ar);
			raw.Serialize(ar);
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
			////sapitemA outitem;
			//BYTE outstep;
			//double a1;
			//size_t nextidx;
			//size_t nowidx;
			UINT flg=ComputeStateData();
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


	void CanalyzerDoc::OnAnalysisMethodsetup()
	{
		// TODO: Add your command handler code here

		POSITION pos = GetFirstViewPosition();
		CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
		CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());

		if(mf->pst==stop){


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
		else{

			//CSingleLock singleLock(&(mf->m_CritSection));
			//if(singleLock.Lock())			{

			//	singleLock.Unlock();
			//	SolutionAdditionParametersPageB *sppage;
			//	if(psheetml==NULL){
			//		psheetml=new PropertySheetA1ML(IDS_STRING_ANALYSIS_SETUP);
			//		sppage=new SolutionAdditionParametersPageB();	
			//		sppage->para1.saplist.assign(p3.saplist.begin()+nextSAPIndex,p3.saplist.end());
			//		//sppage->para1=p3todo;
			//		sppage->para0=p3done;
			//		sppage->pDoc=this;

			//		psheetml->AddPage(sppage);			

			//	}
			//	else{
			//		sppage=(SolutionAdditionParametersPageB*)(psheetml->GetPage(0));
			//		sppage->para1.saplist.assign(p3.saplist.begin()+nextSAPIndex,p3.saplist.end());
			//		//sppage->para1=p3todo;
			//		sppage->para0=p3done;
			//		sppage->pDoc=this;
			//	}

			//	if(psheetml->GetSafeHwnd()){
			//		sppage->SetList();			
			//		psheetml->ShowWindow(SW_SHOW);
			//		psheetml->CenterWindow();			
			//	}
			//	else{
			//		psheetml->Create();
			//	}

			//	::SetWindowPos(psheetml->GetSafeHwnd(),
			//		//HWND_TOPMOST,
			//		HWND_TOP,
			//		0,0,0,0,
			//		SWP_NOMOVE|SWP_NOSIZE); 

			//}
		}







	}



	UINT CanalyzerDoc::ComputeStateData()
	{

		CSingleLock singleLock(&m_CritSection);
		//singleLock.Lock();

		//if (singleLock.IsLocked())  // Resource has been locked
		if(singleLock.Lock())
		{

			dol.clear();
			SAPara p3d;

			runstate=::ComputeStateData(p1.analysistype,p2,p3,raw,dol,p3d,currentSAPIndex,nextSAPIndex,outstep,VtoAdd);

			p3done=p3d;

			//p3todo.saplist.assign(p3.saplist.begin()+nextSAPIndex,p3.saplist.end());

			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();

			return runstate;
		}

		return 200;
	}

	void CanalyzerDoc::Show(void)
	{
		//if(!dol.empty()){
		POSITION pos = GetFirstViewPosition();
		if(pos!=NULL){
			CMainFrame *mf=(CMainFrame*)(GetNextView(pos)->GetParentFrame());
			//CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
			//CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());
			::SendMessage(mf->GetOutputWnd()->GetListCtrl()->GetSafeHwnd(),
				MESSAGE_SHOW_DOL,
				NULL,
				(LPARAM)this);
		}
		//}
	}

	void CanalyzerDoc::OnBnClickedButton1()
	{
		// TODO: Add your control notification handler code here

		//::AfxMessageBox(L"sdafa");
	}


	UINT CanalyzerDoc::ChangeSAP()
	{


		//SAPara p3new=p3done;
		//p3new.AppendData(p3todo);

		//POSITION pos = GetFirstViewPosition();
		//CMainFrame *mf=(CMainFrame*)(GetNextView(pos)->GetParentFrame());

		//CSingleLock singleLock(&m_CritSection);
		//	while(singleLock.Lock())
		//	{
		//		if(mf->pst!=pause){
		//			singleLock.Unlock();
		//		}
		//		else{
		//			p3=p3new;
		//			singleLock.Unlock();
		//			break;
		//		}
		//	}
		if(bChangeSAP){

			CSingleLock singleLock(&m_CritSection);
			if(singleLock.Lock())
			{

				p3=p3done;
				p3.AppendData(p3todo);

				dol.clear();
				SAPara p3d;

				runstate=::ComputeStateData(p1.analysistype,p2,p3,raw,dol,p3d,currentSAPIndex,nextSAPIndex,outstep,VtoAdd);

				p3done=p3d;

				this->bChangeSAP=false;

				// Now that we are finished, 
				// unlock the resource for others.
				singleLock.Unlock();


			}


		}


		return runstate;


	}


	void CanalyzerDoc::OnControlsChangesap()
	{
		// TODO: Add your command handler code here

		//std::vector<DataOutA> doltmp;
		//SAPara p3d;

		//BYTE outstep;
		//double VtoAdd;
		//size_t nextSAPIndex;
		//size_t currentSAPIndex;

		//UINT res=::ComputeStateData(p1.analysistype,p2,p3,raw,doltmp,p3d,currentSAPIndex,nextSAPIndex,outstep,VtoAdd);

		//if(psheetml==NULL){
		//	psheetml=new PropertySheetA1ML();
		//	SolutionAdditionParametersPageA *sppage=new SolutionAdditionParametersPageA();	
		//	sppage->para.saplist.assign(p3.saplist.begin()+nextSAPIndex,p3.saplist.end());
		//	psheetml->AddPage(sppage);			
		//	
		//}
		//else{
		//SolutionAdditionParametersPageA *sppage=(SolutionAdditionParametersPageA*)(psheetml->GetPage(0));

		//
		//}

		//if(psheetml->GetSafeHwnd()){			
		//	psheetml->ShowWindow(SW_SHOW);
		//	psheetml->CenterWindow();			
		//}
		//else{
		//	psheetml->Create();
		//}

		//::SetWindowPos(psheetml->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE); 

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
		COutputListA *ol=mf->GetOutputWnd()->GetListCtrl();

		CanalyzerViewR* rv=((CanalyzerViewR*)GetNextView(pos));


		int a;

		std::vector<CString> res;
		if(b2){
			bool flg=Compute(ol->dol, p1, res);
		}


		a=pdfout6(p,p1,res,p2,p3,b1,b2,b3,b4,templogobmp.GetBuffer());

		if(b5){
			a=pdfout(p,ol->dol,templogobmp.GetBuffer());
		}


		std::vector<PlotDataEx> pdl;
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

		for(size_t i=0;i<pdl.size();i++){
			pdl[i].ResetRange();
			pdl[i].pd.ps.SetStandradCr();
		}

		a=imgout2(p,lv->GetDC(),pdl,nl,CSize(1000,690),templogobmp.GetBuffer());

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

			BOOL chk1=TRUE,chk2=TRUE,chk3=TRUE,chk4=TRUE,chk5=TRUE,chk6=TRUE,chk7=TRUE;
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
			//mf->GetCaptionBar()->ShowMessage(str);

			//if(pdfd(fp,this)==0){
			if(pdfd(fp,chk1,chk2,chk3,chk4,chk5,chk6,chk7)==0){
				//AfxMessageBox(L"report "+fp+L" is saved");

				str.LoadStringW(IDS_STRING_REPORT);
				CString tmps;
				tmps.LoadStringW(IDS_STRING_IS_SAVED);

				//mf->GetCaptionBar()->ShowMessage(str+L" "+se.GetFileName()+L" "+tmps);
				ShellExecute(NULL, L"open", fp, NULL, NULL, SW_SHOW);			
			}
			else{
				//AfxMessageBox(IDS_STRING_SAVE_ERROR);

				str.LoadString(IDS_STRING_SAVE_ERROR);
				//mf->GetCaptionBar()->ShowMessage(str);
			}

		}



	}


	void CanalyzerDoc::OnControls2()
	{
		// TODO: Add your command handler code here
		//POSITION pos = GetFirstViewPosition();
		//CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));

		//CopyWndToClipboard(lv);
	}


	UINT CanalyzerDoc::UpdateALL(void)
	{

		POSITION pos = GetFirstViewPosition();
		CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
		//CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());
		//COutputListA *ol=mf->GetOutputWnd()->GetListCtrl();

		CanalyzerViewR* rv=((CanalyzerViewR*)GetNextView(pos));

		switch(runstate){
		case 0:
			{

				UINT rstmp=ChangeSAP();
				if(rstmp==0){
					::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
					::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
					Show();
					return 0;
				}
				else{
					return UpdateALL();
				}

			}
		case 1:
			{
				::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
				::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
				Show();
				return 1;
			}
		case 2:
			{
				::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
				::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
				Show();
				return 2;
			}
		case 3:
			{
				//AfxMessageBox(IDS_STRING_STEP_ERROR);
				//OnAnalysisAbortanalysis();
				return 3;
			}
		case 4:
			{
				//AfxMessageBox(IDS_STRING_STEP_ERROR);
				//OnAnalysisAbortanalysis();
				::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
				::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
				Show();
				return 4;
			}
		case 5:
			{
				UINT rstmp=ChangeSAP();
				if(rstmp==5){
					::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
					::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
					Show();
					return 5;
				}
				else{
					return UpdateALL();
				}
			}
		case 6:
			{
				::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
				::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
				Show();
				return 6;
			}
		case 7:
			{
				UINT rstmp=ChangeSAP();
				if(rstmp==7){
					::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
					::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
					Show();
					return 7;
				}
				else{
					return UpdateALL();
				}
			}
		default:
			return 100;
		}

	}
