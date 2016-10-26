
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


void LoadFileList(const CString &m_filePath, std::vector<CString> &filelist)
{

	CString folderpath=m_filePath.Left(m_filePath.ReverseFind('\\'));

	filelist.clear();
	CStdioFile file;
	BOOL readflag;
	readflag=file.Open(m_filePath, CFile::modeRead);

	if(readflag)
	{	
		CString strRead;
		//TRACE("\n--Begin to read file");
		while(file.ReadString(strRead)){
			strRead=folderpath+"\\"+strRead;
			filelist.push_back(strRead);
		}
		//TRACE("\n--End reading\n");
		file.Close();
	}
}





#include <propkey.h>

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
		//ar<<ll.size();		
		//size_t si=0;
		//size_t ei=0;
		//for(size_t i=0;i<ll.size();i++){
		//	ar<<ll[i];
		//	ei=si+ll[i];
		//	for(size_t j=si;j<ei;j++){
		//		ar<<xll[j]
		//		<<yll[j];
		//	}
		//	si=ei;
		//}

	//raw.Serialize(ar);

	}
	else
	{
		// TODO: add loading code here

		//size_t l;

		//ar>>l;
		//ll.assign(l,0);

		//size_t si=0;
		//size_t ei=0;
		//for(size_t i=0;i<ll.size();i++){
		//	ar>>ll[i];

		//	xll.resize(si+ll[i],0);
		//	yll.resize(si+ll[i],0);

		//	ei=si+ll[i];
		//	for(size_t j=si;j<ei;j++){
		//		ar>>xll[j]
		//		>>yll[j];
		//	}
		//	si=ei;
		//}

	//	raw.Clear();
	//
	//CString folderp=L"C:\\Users\\r8anw2x\\Desktop\\data\\d\\";
	//CString DTRflist=folderp+L"dtr.txt";

	//std::vector<CString> filelist;
	//LoadFileList(DTRflist,filelist);
	//pcct data;

	//while(!filelist.empty()){
	///////load data from file////////////
	//data.clear();
	//data.readFile(filelist.front());
	//data.TomA();

	//raw.xll.resize(raw.xll.size()+data.potential.size());
	//std::copy_backward(data.potential.begin(),data.potential.end(),raw.xll.end());

	//raw.yll.resize(raw.yll.size()+data.current.size());
	//std::copy_backward(data.current.begin(),data.current.end(),raw.yll.end());

	//raw.ll.push_back(data.potential.size());

	//filelist.erase(filelist.begin());

	//}



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

			//if(!dol.empty()){			

			//	str=Compute(dol,p1);
				POSITION pos = GetFirstViewPosition();
				CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
				CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());

				//CanalyzerViewR* rv=((CanalyzerViewR*)(mf->m_wndSplitter.GetPane(0,1)));

				CanalyzerViewR* rv=((CanalyzerViewR*)GetNextView(pos));

			//	mf->GetCaptionBar()->ShowMessage(str);

				::SendMessage(mf->GetOutputWnd()->GetListCtrl()->GetSafeHwnd(),
					MESSAGE_UPDATE_DOL,
					(WPARAM)lv,
					(LPARAM)rv);

			//}

		}
	}