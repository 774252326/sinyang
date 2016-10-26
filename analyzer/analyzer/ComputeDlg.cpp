// ComputeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "ComputeDlg.h"
#include "afxdialogex.h"
#include "analyzerDoc.h"
#include "filefunc.h"


void ScaleBitmap(CBitmap *pSrc, CBitmap *pDst, int dstW, int dstH);
bool FitLine(std::vector<double> &x, std::vector<double> &y, LineSeg &ls, int nFront=0, int nBack=0);

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
	DDX_Control(pDX, IDC_MFCBUTTON1, m_btn1);
	DDX_Control(pDX, IDC_MFCBUTTON2, m_btn2);
}


BEGIN_MESSAGE_MAP(ComputeDlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_MFCBUTTON1, &ComputeDlg::OnBnClickedMfcbutton1)
END_MESSAGE_MAP()


// ComputeDlg message handlers


int ComputeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here



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





BOOL ComputeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here





	CSize gap1(20,20);
	CSize gap2(20,20);

	CRect winrect;
	this->GetClientRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);
	CSize btnSz(200,50);


	m_btn1.MoveWindow(CRect(pt,btnSz));
	m_btn1.EnableFullTextTooltip(FALSE);
	m_btn1.SetTooltip(L"sdjalsajdslfka;kkkkkkkkkkkkkkkkkksld;;;;;;;;;;;;;;;;;;;;;jffffffffffffffffffffffffffffffffffffffffffa;l\r\n\
					   sadjklllllllllllllllllllllllllllfdsaaaaaaaaaaafasdfdafdasdsaaaaaaaaaaaaaaaaaaaaaaaaaaaaalllllllllllllll\r\n\
					   123142421");


	//CMFCButton *pbtn;

	//pbtn=new CMFCButton();

	//pbtn->Create(L"",WS_CHILD|WS_VISIBLE|/*BS_PUSHBUTTON|BS_FLAT|BS_NOTIFY|*/BS_BITMAP,CRect(pt,btnSz),this,1278);
	//pbtn->EnableWindowsTheming(true);

	//pbtn->EnableFullTextTooltip(true);
	// pbtn->SetTooltip(_T("this is a button!"));

	CBitmap * pBmp = NULL;

	CBitmap * dst=NULL;

	pBmp = new CBitmap();
	pBmp->LoadBitmapW(IDB_BITMAP12);
	dst=new CBitmap();
	ScaleBitmap(pBmp,dst,btnSz.cx,btnSz.cy);

	BITMAP bm;
	dst->GetBitmap(&bm); 

	HBITMAP hb=(HBITMAP)(*dst);
	////pbtn->SetImage(hb);
	m_btn1.SetImage(hb);

	m_btn1.ShowWindow(SW_SHOW);

	//pbtn->SetImage(IDB_BITMAP12);
	//	//delete dst;
	//	delete pBmp;

	//	pbtn->SizeToContent();

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



	CFrameWndEx* pframe = (CFrameWndEx*) AfxGetMainWnd();
	//CMDIChildWnd* pchild = pframe->MDIGetActive();
	CanalyzerDoc* doc = (CanalyzerDoc*) pframe->GetActiveDocument();
	//doc->m_Color = m_Color;

	// Tell the view to paint with the new selected color.
	//CanalyzerView* view = (CanalyzerView*)(pframe->GetActiveView());


	RawData rd0;
	std::vector<DataOutA> dolast0;

	if(doc->FinalData(rd0,dolast0)){
		//if(RecordDTGetVL(m_list.vl,dolast0,doc->p1.evaluationratio,rd0)){
		if(GetVL(m_list.vl,dolast0,rd0,doc->p1)){

			m_list.Refresh();
			return TRUE;  // return TRUE unless you set the focus to a control
		}
	}

	return FALSE;
	// EXCEPTION: OCX Property Pages should return FALSE

}


void ComputeDlg::OnBnClickedMfcbutton1()
{
	// TODO: Add your control notification handler code here

	//double z;
	//if(RecordDT(m_list.vl,z)){
	//	CString str;
	//	str.Format(L"%g",z);
	//	((CEdit*)GetDlgItem(1289))->SetWindowTextW(str);
	//}

	std::vector<CString> namel;
	std::vector<CString> valuel;

	CFrameWndEx* pframe = (CFrameWndEx*) AfxGetMainWnd();
	//CMDIChildWnd* pchild = pframe->MDIGetActive();
	CanalyzerDoc* doc = (CanalyzerDoc*) pframe->GetActiveDocument();
	//doc->m_Color = m_Color;

	// Tell the view to paint with the new selected color.
	//CanalyzerView* view = (CanalyzerView*)(pframe->GetActiveView());

	if(GetResult(doc,m_list.vl,namel,valuel)){
		CString str=L"";
		for(size_t i=0;i<namel.size();i++){
			str+=namel[i]+L" "+valuel[i]+L"\n";
		}
			((CEdit*)GetDlgItem(1289))->SetWindowTextW(str);
	}
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
		//tmp.p1.evaluationratio=evaR;
		std::vector<Value> vl0;
		if(RecordDTGetVL(vl0,dolast,evaR,rd0)){
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




bool ComputeDlg::RecordLAT(const RawData & rd, double & ITc, double slopeThreshold, size_t index)
{
	std::vector<DWORD> sl;
	bool flg=CanalyzerDoc::GetStepList(sl,3);

	BYTE bb=(CanalyzerDoc::nby(sl[0],2)&PF_Q_NORMALIZED);

	return rd.InterpDerivativeX(index,slopeThreshold,ITc,bb);

}

bool ComputeDlg::AnalysisLATGetVL(std::vector<Value> & vl, 
	const std::vector<DataOutA> & dolast, 
	const RawData & rd, 
	size_t lineIndex,
	int nFront,
	int nBack)
{
	if(dolast.size()==3){
		double SPv=dolast[2].TotalVolume();
		double SPv0=SPv-dolast[1].TotalVolume();
		double ITQ=dolast[1].ArUse();

		std::vector<double> x;
		std::vector<double> y;
		rd.GetDatai(lineIndex,x,y);

		LineSeg ls;
		if(FitLine(x,y,ls,nFront,nBack)){

			vl.assign(5,Value());
			vl[0].raw=SPv;
			vl[1].raw=SPv0;
			vl[2].raw=ITQ;
			vl[3].raw=ls.GetB();
			vl[4].raw=ls.GetK();
			return true;
		}
	}

	return false;
}


bool ComputeDlg::AnalysisLAT(const std::vector<Value> & vl, double & SPc)
{
	if(vl.size()==5){
		SPc=vl[0].Output()*(vl[3].Output()-vl[2].Output())/vl[1].Output()/vl[4].Output();
		//Ac=-(ITQ-b)/k;
		//SPc=-(ITQ-ls.GetB())/ls.GetK()*SPv/SPv0;
		return true;
	}
	return false;
}

bool ComputeDlg::RecordRC(const std::vector<DataOutA> & dolast, double evaR, const RawData & rd, double & Lc, size_t index)
{
	double Q=evaR*dolast.back().Ar0;

	return rd.InterpX(index,Q,Lc);
}

bool ComputeDlg::AnalysisRCGetVL(std::vector<Value> & vl, 
	const std::vector<DataOutA> & dolast, 
	double Lc)
{
	if(dolast.size()==3){
		double SPv=dolast[2].TotalVolume();
		double SPv0=SPv-dolast[1].TotalVolume();
		vl.assign(3,Value());
		vl[0].raw=SPv;
		vl[1].raw=SPv0;
		vl[2].raw=Lc;
		return true;
	}
	return false;
}


bool ComputeDlg::AnalysisRCGetVL(std::vector<Value> & vl, 
	const std::vector<DataOutA> & dolast, 
	//double evaR, 
	CString fp, 
	const RawData & rd, 
	size_t lineIndex)
{
	CanalyzerDoc tmp;
	RawData rd0;
	std::vector<DataOutA> dolast0;

	if(tmp.FinalData(rd0,dolast0,fp)){

		tmp.p1.evaluationratio=dolast[2].ArUse()/dolast0.back().Ar0;

		double Lc;
		if(RecordRC(dolast0,tmp.p1.evaluationratio,rd0,Lc)){
			return AnalysisRCGetVL(vl,dolast,Lc);
		}
	}


	return false;
}

bool ComputeDlg::AnalysisRC(const std::vector<Value> & vl, double & SPc)
{
	if(vl.size()==3){
		SPc=vl[2].Output()*vl[0].Output()/vl[1].Output();
		return true;
	}
	return false;
}

bool ComputeDlg::RecordSAR(const std::vector<DataOutA> & dolast, double evaR, const RawData & rd, RawData &SAraw, LineSeg &lis)
{
	if(dolast.size()==2*rd.ll.size()){

		SAraw.ll.assign(1,0);
		for(size_t i=0;i<rd.ll.size();i++){
			double sconc;
			if(rd.InterpX(i,evaR,sconc)){
				double aconc=sconc*(dolast[2*i+1].AConc()/dolast[2*i+1].SConc());
				SAraw.AddFollow(aconc,sconc);
			}
		}

		SAraw.Sort(0);

		std::vector<double> Ac;
		std::vector<double> Sc;
		SAraw.GetDatai(0,Ac,Sc);

		if(FitLine(Ac,Sc,lis))
			return true;
	}

	return false;
}


bool ComputeDlg::RecordSARGetStd(const std::vector<DataOutA> & dolast, const RawData & rd, RawData &SnQstd)
{
	SnQstd.Clear();
	for(size_t i=0;i<rd.ll.size() && 2*i+1<dolast.size();i++){

		if(dolast[2*i+1].AConc()==0){
			std::vector<double> x;
			std::vector<double> y;
			rd.GetDatai(i,x,y);
			SnQstd.AddNew(x,y);
			return true;
		}			
	}


	return false;
}


bool ComputeDlg::AnalysisSARGetVL(std::vector<Value> & vl, 
	const std::vector<DataOutA> & dolast, 
	double evaR, 
	CString fp, 
	const RawData & rd, 
	size_t lineIndexSP,
	size_t lineIndexA)
{

	CanalyzerDoc tmp;
	RawData rd0;
	std::vector<DataOutA> dolast0;

	if(tmp.FinalData(rd0,dolast0,fp)){
		RawData SAraw;
		LineSeg ls0;
		if(RecordSAR(dolast0,evaR,rd0,SAraw,ls0)){
			if(dolast.size()==3){
				double Vv=dolast[2].VMSVolume;
				double SPvEnd=dolast[1].TotalVolume()-dolast[0].TotalVolume();

				std::vector<double> x;
				std::vector<double> y;

				rd.GetDatai(lineIndexA,x,y);
				LineSeg lis;
				if(FitLine(x,y,lis)){
					double SPv;
					if( rd.InterpX(lineIndexSP,evaR,SPv) ){
						vl.assign(7,Value());
						vl[0].raw=ls0.GetB();
						vl[1].raw=ls0.GetK();
						vl[2].raw=Vv;
						vl[3].raw=SPvEnd;
						vl[4].raw=lis.GetB();
						vl[5].raw=lis.GetK();
						vl[6].raw=SPv;

						return true;
					}
				}
			}
		}
	}

	return false;
}


bool ComputeDlg::AnalysisSAR(const std::vector<Value> & vl, 
	const RawData &SnQstd, 
	double & Sc, 
	double & Ac, 
	size_t nir, 
	size_t lineIndex)
{
	if(vl.size()==7){
		if(vl[6].Output()!=0){
			double a1=vl[2].Output()/vl[6].Output()+1;
			if(vl[3].Output()!=0){
				double a2=vl[2].Output()/vl[3].Output()+1;
				if(a2!=0){
					double b1=a1/a2;
					if(vl[5].Output()!=0){
						Ac=0;
						for(int i=0;i<nir;i++){
							Sc=vl[1].Output()*Ac+vl[0].Output()*b1;
							double tmp=SnQstd.InterpY(lineIndex,Sc);
							Ac=(vl[4].Output()-tmp)/vl[5].Output();							
						}
						Sc*=a2;
						Ac*=a2;
						return true;
					}
				}
			}
		}
	}


	return false;
}


bool ComputeDlg::RecordPAL(double evaR, const RawData & rd, double & Lc, size_t index)
{
	return rd.InterpX(index,evaR,Lc);
}


bool ComputeDlg::AnalysisPALGetVL(std::vector<Value> & vl, 
	const std::vector<DataOutA> & dolast, 
	CString fp)
{

	CanalyzerDoc tmp;
	RawData rd0;
	std::vector<DataOutA> dolast0;

	if(tmp.FinalData(rd0,dolast0,fp)){

		if(dolast.size()==3){
			double SPv=dolast[2].TotalVolume();
			double SPv0=SPv-dolast[1].TotalVolume();
			double Lml=dolast[1].Lml;
			double nQ=dolast[2].ArUse()/dolast[2].Ar0;
			double Lc;
			if(RecordPAL(nQ,rd0,Lc)){
				vl.assign(4,Value());
				vl[0].raw=SPv;
				vl[1].raw=SPv0;
				vl[2].raw=Lml;
				vl[3].raw=Lc;
				return true;
			}
		}
	}

	return false;
}


bool ComputeDlg::AnalysisPAL(const std::vector<Value> & vl, double & SPc)
{
	if(vl.size()==4){
		SPc=(vl[3].Output()*vl[0].Output()-vl[2].Output())/vl[1].Output();
		return true;
	}
	return false;
}

bool ComputeDlg::RecordLRT(const RawData & rd, LineSeg &lis, size_t index)
{
	std::vector<double> x;
	std::vector<double> y;
	rd.GetDatai(index,x,y);
	return FitLine(x,y,lis);
}

bool ComputeDlg::AnalysisLRTGetVL(std::vector<Value> & vl, 
	const std::vector<DataOutA> & dolast, 
	CString fp,
	const RawData & rd,
	int nIgnore,
	size_t lineIndex)
{

	CanalyzerDoc tmp;
	RawData rd0;
	std::vector<DataOutA> dolast0;

	if(tmp.FinalData(rd0,dolast0,fp)){

		LineSeg ls0;
		if(RecordLRT(rd0,ls0)){
			if(dolast.size()==4){
				double SPv0=dolast[2].TotalVolume()-dolast[1].TotalVolume();
				double Lml=dolast[3].Lml;
				double nQ=dolast[3].ArUse()/dolast[3].Ar0;
				double SPv=dolast[3].TotalVolume();

				std::vector<double> x;
				std::vector<double> y;

				rd.GetDatai(lineIndex,x,y);
				LineSeg lis;

				if(FitLine(x,y,lis,nIgnore)){
					vl.assign(6,Value());
					vl[0].raw=ls0.GetB();
					vl[1].raw=SPv0;
					vl[2].raw=Lml;
					vl[3].raw=nQ;
					vl[4].raw=SPv;
					vl[5].raw=lis.GetK();

					return true;
				}
			}
		}
	}


	return false;
}


bool ComputeDlg::AnalysisLRT(const std::vector<Value> & vl, double & SPc)
{
	if(vl.size()==6){
		double Lc=(vl[3].Output()-vl[0].Output())/vl[5].Output();
		SPc=(Lc*vl[4].Output()-vl[2].Output())/vl[1].Output();
		return true;
	}
	return false;
}



bool ComputeDlg::GetVL(std::vector<Value> & vl, 
	const std::vector<DataOutA> & dolast, 
	const RawData & rd,
	const ANPara & p1)
{
	switch(p1.analysistype){
	case 1:
		return RecordDTGetVL(vl,dolast,p1.evaluationratio,rd);
	case 2:
		{
			switch(p1.calibrationfactortype){
			case 0:
				return AnalysisDTGetVL(vl,dolast,p1.evaluationratio,p1.calibrationfactor,rd);
			case 1:
				return AnalysisDTGetVL(vl,dolast,p1.evaluationratio,p1.calibrationfilepath,rd);
			default:
				return false;
			}
		}
	case 4:
		return AnalysisLATGetVL(vl,dolast,rd);
	case 6:
		return AnalysisRCGetVL(vl,dolast,p1.calibrationfilepath,rd);
	case 8:
		return AnalysisSARGetVL(vl,dolast,p1.evaluationratio,p1.calibrationfilepath,rd);
	case 10:
		return AnalysisPALGetVL(vl,dolast,p1.calibrationfilepath);
	case 12:
		return AnalysisLRTGetVL(vl,dolast,p1.calibrationfilepath,rd);
	default:
		return false;
	}
	return false;
}


bool ComputeDlg::GetResult(CanalyzerDoc *pDoc, const std::vector<Value> &vl, std::vector<CString> &name, std::vector<CString> &value)
{
	CString str,strt;
	switch(pDoc->p1.analysistype){
	case 1:
		{
			double z;
			if(RecordDT(vl,z)){
				strt.LoadStringW(IDS_STRING_CALIBRATION_FACTOR);
				str=strt;
				name.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",z,strt);

				value.push_back(str);
				return true;
			}
			return false;
		}
	case 2:
		{
			double SPc;
			if(AnalysisDT(vl,SPc)){
				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				name.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",SPc,strt);

				value.push_back(str);

				return true;
			}
			return false;
		}
	case 3:
		{
			RawData rd0;
			std::vector<DataOutA> dolast0;
			if(pDoc->FinalData(rd0,dolast0)){
				double ITc;
				if(RecordLAT(rd0,ITc)){
					strt.LoadStringW(IDS_STRING_INTERCEPT_VALUE);
					str=strt;

					name.push_back(str);

					strt.LoadStringW(IDS_STRING_ML_PER_L);
					str.Format(L"%g%s",ITc,strt);

					value.push_back(str);

					return true;
				}
			}

			return false;
		}
	case 4:
		{
			double SPc;
			if(AnalysisLAT(vl,SPc)){

				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				name.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",SPc,strt);
				value.push_back(str);
				return true;
			}
			return false;
		}
	case 5:
		{
			RawData rd0;
			std::vector<DataOutA> dolast0;
			if(pDoc->FinalData(rd0,dolast0)){
				double Lc;
				if(RecordRC(dolast0,pDoc->p1.evaluationratio,rd0,Lc)){

					strt.LoadStringW(IDS_STRING_L);
					str=strt;
					str+=L" ";
					strt.LoadStringW(IDS_STRING_CONCERTRATION);
					str+=strt;
					name.push_back(str);

					strt.LoadStringW(IDS_STRING_ML_PER_L);
					str.Format(L"%g%s",Lc,strt);
					value.push_back(str);

					return true;
				}

			}
			return false;
		}

	case 6:
		{
			double SPc;
			if(AnalysisRC(vl,SPc)){
				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				name.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",SPc,strt);
				value.push_back(str);

				return true;
			}
			return false;
		}
	case 7:
		{
			RawData rd0;
			std::vector<DataOutA> dolast0;
			if(pDoc->FinalData(rd0,dolast0)){

				RawData r0;
				LineSeg l0;
				if(RecordSAR(dolast0,pDoc->p1.evaluationratio,rd0,r0,l0)){

					strt.LoadStringW(IDS_STRING_FITTING_LINE);
					str=strt;
					name.push_back(str);

					str.Format(L"S=%gA%+g",l0.GetK(),l0.GetB());
					value.push_back(str);

					return true;
				}
			}
			return false;
		}
	case 8:
		{
			RawData rd0;
			std::vector<DataOutA> dolast0;
			CanalyzerDoc tmpDoc;

			if(tmpDoc.FinalData(rd0,dolast0,pDoc->p1.calibrationfilepath)){
				RawData SnQstd;
				if(RecordSARGetStd(dolast0,rd0,SnQstd)){
					double Sc,Ac;
					if(AnalysisSAR(vl,SnQstd,Sc,Ac)){

						strt.LoadStringW(IDS_STRING_SAMPLE);
						str=strt;
						str+=L" ";
						strt.LoadStringW(IDS_STRING_S);
						str+=strt;
						str+=L" ";
						strt.LoadStringW(IDS_STRING_CONCERTRATION);
						str+=strt;
						name.push_back(str);

						strt.LoadStringW(IDS_STRING_ML_PER_L);
						str.Format(L"%g%s",Sc,strt);
						value.push_back(str);

						strt.LoadStringW(IDS_STRING_SAMPLE);
						str=strt;
						str+=L" ";
						strt.LoadStringW(IDS_STRING_A);
						str+=strt;
						str+=L" ";
						strt.LoadStringW(IDS_STRING_CONCERTRATION);
						str+=strt;
						name.push_back(str);

						strt.LoadStringW(IDS_STRING_ML_PER_L);
						str.Format(L"%g%s",Ac,strt);
						value.push_back(str);
						return true;
					}
				}
			}
			return false;
		}
	case 9:
		{
			RawData rd0;
			std::vector<DataOutA> dolast0;
			if(pDoc->FinalData(rd0,dolast0)){
				double Lc;
				if(RecordPAL(pDoc->p1.evaluationratio,rd0,Lc)){
					strt.LoadStringW(IDS_STRING_L);
					str=strt;
					str+=L" ";
					strt.LoadStringW(IDS_STRING_CONCERTRATION);
					str+=strt;
					name.push_back(str);

					strt.LoadStringW(IDS_STRING_ML_PER_L);
					str.Format(L"%g%s",Lc,strt);
					value.push_back(str);

					return true;
				}
			}
			return false;
		}

	case 10:
		{
			double SPc;
			if(AnalysisPAL(vl,SPc)){
				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				name.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",SPc,strt);
				value.push_back(str);

				return true;
			}
			return false;
		}
	case 11:
		{
			RawData rd0;
			std::vector<DataOutA> dolast0;
			if(pDoc->FinalData(rd0,dolast0)){
				LineSeg lis;
				if(RecordLRT(rd0,lis)){

					strt.LoadStringW(IDS_STRING_FITTING_LINE);
					str=strt;
					name.push_back(str);

					str.Format(L"nQ=%gL%+g",lis.GetK(),lis.GetB());
					value.push_back(str);

					return true;
				}

			}
			return false;
		}
	case 12:
		{
			double SPc;
			if(AnalysisLRT(vl,SPc)){
				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				name.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",SPc,strt);
				value.push_back(str);

				return true;
			}
			return false;
		}


	default:
		return false;
	}

}