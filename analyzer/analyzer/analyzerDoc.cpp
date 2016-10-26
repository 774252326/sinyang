
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
#include "analyzerViewL.h"
#include "analyzerViewR.h"
//#include "func.h"
#include "MainFrm.h"

#include "pdfout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CanalyzerDoc

IMPLEMENT_DYNCREATE(CanalyzerDoc, CDocument)

	BEGIN_MESSAGE_MAP(CanalyzerDoc, CDocument)
		ON_COMMAND(ID_ANALYSIS_METHODSETUP, &CanalyzerDoc::OnAnalysisMethodsetup)
		//		ON_COMMAND(ID_ANALYSIS_STARTANALYSIS, &CanalyzerDoc::OnAnalysisStartanalysis)
		ON_COMMAND(ID_ANALYSIS_REPORT, &CanalyzerDoc::OnAnalysisReport)
	END_MESSAGE_MAP()


	// CanalyzerDoc construction/destruction

	CanalyzerDoc::CanalyzerDoc()
		//: ppa(NULL)
		: resultStr(_T(""))
		, bUpdateView(true)
	{
		// TODO: add one-time construction code here

	}


	void CanalyzerDoc::operator=(const CanalyzerDoc &src)
	{
		p1=src.p1;
		p2=src.p2;
		p3=src.p3;
		dol.assign(src.dol.begin(),src.dol.end());
		lp.assign(src.lp.begin(),src.lp.end());
		rp.assign(src.rp.begin(),src.rp.end());
		resultStr=src.resultStr;
		bUpdateView=false;
	}

	CanalyzerDoc::CanalyzerDoc(bool flg)
	{
		bUpdateView=flg;
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
		CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
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
			dol.assign(n,DataOutA());
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

			if(bUpdateView){
				POSITION pos = GetFirstViewPosition();
				CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
				lv->selectIdx=0;
				lv->SetSpin(lp.size()-1);
				lv->updatePlotRange();

				CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());
				COutputList* ol=mf->GetOutputWnd()->GetListCtrl();
				ol->DeleteAllItems();
				mf->GetCaptionBar()->ShowMessage(resultStr);

				CanalyzerViewR* rv=((CanalyzerViewR*)GetNextView(pos));
				rv->selectIdx=0;
				rv->SetSpin(rp.size()-1);
				rv->updatePlotRange();

				size_t c=0;
				for(size_t i=0;i<dol.size();i++){
					for(size_t j=0;j<dol[i].Ar.size();j++){
						ol->InsertListCtrl(c,dol[i],i,j);
						c++;
					}
				}
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

			if(!dol.empty()){
				switch(p1.analysistype){
				case 1:
					{
						double Sv,z;
						bool flg=Compute1(dol, p1, Sv, z);
						resultStr.Format(L"Sv=%g,z=%g",Sv,z);
					}
					break;
				default:
					break;
				}


				POSITION pos = GetFirstViewPosition();
				CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
				//lv->selectIdx=0;
				//lv->SetSpin(lp.size()-1);
				//lv->updatePlotRange();

				CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());
				//COutputList* ol=mf->GetOutputWnd()->GetListCtrl();
				//ol->DeleteAllItems();
				mf->GetCaptionBar()->ShowMessage(resultStr);


			}

		}
	}




	PlotData * CanalyzerDoc::GetPD(int lr, int index)
	{
		switch(lr){
		case 0:
			if(lp.size()>index){
				return &lp[index];
			}			
			return NULL;
		case 1:
			if(rp.size()>index){
				return &rp[index];
			}			
			return NULL;
		default:
			return NULL;
		}
	}


	int CanalyzerDoc::GetNPD(int lr)
	{
		switch(lr){
		case 0:
			return lp.size();
		case 1:	
			return rp.size();
		default:
			return -1;
		}
	}

	int CanalyzerDoc::AddPD(const PlotData & pda, int lr)
	{
		switch(lr){
		case 0:
			lp.push_back(pda);
			return lp.size();
		case 1:	
			rp.push_back(pda);
			return rp.size();
		default:
			return -1;
		}
		//return 0;
	}


	void CanalyzerDoc::OnAnalysisReport()
	{
		// TODO: Add your command handler code here

		CString fp=this->GetPathName();
		fp=fp.Left(fp.ReverseFind('\\')+1);
		fp+=TimeString()+L".pdf";

		if(pdfd(fp,this)==0){
			//AfxMessageBox(L"report "+fp+L" is saved");

			ShellExecute(NULL, L"open", fp, NULL, NULL, SW_SHOW);
		}
		else{
			AfxMessageBox(IDS_STRING_SAVE_ERROR);
		}


	}


	bool CanalyzerDoc::SaveImage(const PlotData & pd, CSize sz, CString filepath)
	{

		POSITION pos = GetFirstViewPosition();
		CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
		CDC* pdc=lv->GetDC();

		CDC dcMem;   //用于缓冲作图的内存DC
		dcMem.CreateCompatibleDC(pdc);               //依附窗口DC创建兼容内存DC		

		CBitmap bmp;           //内存中承载临时图象的位图
		bmp.CreateCompatibleBitmap(pdc,sz.cx,sz.cy);//创建兼容位图

		dcMem.SelectObject(&bmp);  	//将位图选择进内存DC

		double xmin,xmax,ymin,ymax,pct=0.02;
		UpdateRange(pd.xll,xmin,xmax,pct,true);
		UpdateRange(pd.yll,ymin,ymax,pct,true);

		CRect plotrect(0,0,sz.cx,sz.cy);
		DrawData(plotrect,&dcMem,pd,xmin,xmax,ymin,ymax,lv->bkcr);

		CImage img;
		img.Attach(HBITMAP(bmp));
		HRESULT hResult = img.Save((LPCWSTR)filepath);

		dcMem.DeleteDC(); //删除DC
		bmp.DeleteObject(); //删除位图
		if (SUCCEEDED(hResult))
			return true;


		return false;
	}
