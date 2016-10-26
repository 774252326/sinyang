// ComputeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "ComputeDlg.h"
#include "afxdialogex.h"
#include "analyzerDoc.h"
#include "filefunc.h"


void ScaleBitmap(CBitmap *pSrc, CBitmap *pDst, int dstW, int dstH);


// ComputeDlg dialog

IMPLEMENT_DYNAMIC(ComputeDlg, CDialogEx)

	ComputeDlg::ComputeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ComputeDlg::IDD, pParent)
{

}

ComputeDlg::~ComputeDlg()
{
}

void ComputeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ComputeDlg, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// ComputeDlg message handlers


int ComputeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CSize gap1(20,20);
	CSize gap2(20,20);

	CRect winrect;
	this->GetClientRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);
	CSize btnSz(100,50);

	CMFCButton *pbtn;

	pbtn=new CMFCButton();

	pbtn->Create(L"",WS_CHILD|WS_VISIBLE|/*BS_PUSHBUTTON|BS_FLAT|BS_NOTIFY|*/BS_BITMAP,CRect(pt,btnSz),this,1278);
	pbtn->EnableWindowsTheming(true);

	pbtn->EnableFullTextTooltip(true);
	 pbtn->SetTooltip(_T("this is a button!"));

	 
	CBitmap * pBmp = NULL;

	CBitmap * dst=NULL;

		pBmp = new CBitmap();
		pBmp->LoadBitmapW(IDB_BITMAP12);
		dst=new CBitmap();
		ScaleBitmap(pBmp,&ddd,btnSz.cx,btnSz.cy);
		//HBITMAP hb=(HBITMAP)(*pBmp);
		////pbtn->SetImage(hb);
		

	pbtn->SetImage(IDB_BITMAP12);
		//delete dst;
		delete pBmp;

		pbtn->SizeToContent();

	pt.x+=gap2.cx+btnSz.cx;

	CEdit *pEdit;

	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		L"",
		ES_LEFT
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,btnSz),
		this,
		1289);

	pt.x=gap1.cx;
	pt.y+=gap2.cy+btnSz.cy;

	const DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		| WS_BORDER
		| WS_HSCROLL 
		| WS_VSCROLL 
		| LBS_NOINTEGRALHEIGHT;

	if(!m_list.Create(dwStyle, CRect(pt,winrect.BottomRight()), this, 1256) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	

	return 0;
}


bool ComputeDlg::SetParameter(const std::vector<DataOutA> & dol, const ANPara & p1, std::vector<RawData> & rdl, std::vector<Value> & vl)
{
	//std::vector<RawData> rdl;
	//std::vector<CString> xlabellist;
	//std::vector<CString> ylabellist;
	//std::vector<size_t> dolastidx;
	//UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	//std::vector<DataOutA> dolast(dolastidx.size());
	//for(size_t i=0;i<dolast.size();i++){
	//	dolast[i]=dol[dolastidx[i]];
	//}

	//switch(p1.analysistype){
	//case 1:
	//	{
	//		if(RecordDTGetVL(vl,dolast,p1.evaluationratio,rdl.back())){
	//			return true;
	//		}
	//		return false;
	//	}
	//case 2:


	//default:
	//	return false;
	//}


	return false;
}



bool ComputeDlg::RecordDTGetVL(
	std::vector<Value> & vl, 
	const std::vector<DataOutA> & dolast, 
	double evaR, 
	const RawData & rd,
	size_t lineIndex)
{
	if(dolast.size()==2){
		double Sc=dolast[1].Sml/dolast[1].additiveVolume;
		double Vv=dolast[1].VMSVolume;
		double Sv;
		if(rd.InterpX(lineIndex,evaR,Sv)){
			vl.assign(3,Value());
			vl[0].raw=Sc;
			vl[1].raw=Vv;
			vl[2].raw=Sv;
			return true;
		}
	}
	return false;
}

bool ComputeDlg::RecordDT(
	const std::vector<Value> & vl, 
	double & z)
{
	if(vl.size()==3){
		z=vl[0].Output()/(1+vl[1].Output()/vl[2].Output());
		return true;
	}
	return false;
}



bool ComputeDlg::AnalysisDTGetVL(
	std::vector<Value> & vl, 
	const std::vector<DataOutA> & dolast, 
	double evaR,
	double z, 
	const RawData & rd, 
	size_t lineIndex)
{
	if(dolast.size()==2){
		double Vv=dolast[1].VMSVolume;
		double SPv;
		if(rd.InterpX(lineIndex,evaR,SPv)){
			vl.assign(3,Value());
			vl[0].raw=z;
			vl[1].raw=Vv;
			vl[2].raw=SPv;
			return true;
		}
	}
	return false;
}

bool ComputeDlg::AnalysisDTGetVL(
	std::vector<Value> & vl, 
	const std::vector<DataOutA> & dolast, 
	double evaR,
	CString fp, 
	const RawData & rd, 
	size_t lineIndex)
{
	CanalyzerDoc tmp;
	RawData rd0;
	std::vector<DataOutA> dolast0;

	if(tmp.FinalData(rd0,dolast0,fp)){
		tmp.p1.evaluationratio=evaR;
		std::vector<Value> vl0;
		if(RecordDTGetVL(vl0,dolast,tmp.p1.evaluationratio,rd0)){
			double z;
			if(RecordDT(vl0,z)){
				return AnalysisDTGetVL(vl,dolast,evaR,z,rd);
			}
		}
	}
	return false;
}


bool ComputeDlg::AnalysisDT(const std::vector<Value> & vl, double & SPc)
{
	if(vl.size()==3){
		//SPc=z*(1+Vv/SPv);
		SPc=vl[0].Output()*(1+vl[1].Output()/vl[2].Output());
		return true;
	}
	return false;
}

BOOL ComputeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here


	CFrameWndEx* pframe = (CFrameWndEx*) AfxGetMainWnd();
   //CMDIChildWnd* pchild = pframe->MDIGetActive();
   CanalyzerDoc* doc = (CanalyzerDoc*) pframe->GetActiveDocument();
   //doc->m_Color = m_Color;

   // Tell the view to paint with the new selected color.
   //CanalyzerView* view = (CanalyzerView*)(pframe->GetActiveView());

   	RawData rd0;
	std::vector<DataOutA> dolast0;

	if(doc->FinalData(rd0,dolast0)){
		if(RecordDTGetVL(m_list.vl,dolast0,doc->p1.evaluationratio,rd0)){
			m_list.Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
		}
	}

	return FALSE;
	// EXCEPTION: OCX Property Pages should return FALSE
		
}
