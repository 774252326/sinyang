// testdlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "testdlg.h"
#include "afxdialogex.h"
#include "pcct.h"

// testdlg dialog

IMPLEMENT_DYNAMIC(testdlg, CDialogEx)

	testdlg::testdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(testdlg::IDD, pParent)
{

}

testdlg::~testdlg()
{
}

void testdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(testdlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(3002, &testdlg::Onbtn1)
	ON_BN_CLICKED(3003, &testdlg::OnCopy)
END_MESSAGE_MAP()


// testdlg message handlers


int testdlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here




	CSize gap1(20,20);
	CSize gap2(20,20);
	CSize staticSize(150,22);

	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);
	CStatic *pStatic;
	CEdit *pEdit;
	CString str;

	CSize listsz=winrect.Size();
	listsz.cy/=3;

	//str.LoadStringW(IDS_STRING_SAP_TIPS);
	//stt.Create(
	//	str,
	//	WS_CHILD
	//	|WS_VISIBLE, 
	//	CRect(pt,CSize(winrect.Width(),staticSize.cy)),
	//	this,
	//	10000);

	//pt.y+=gap2.cy+staticSize.cy;



	//str.LoadStringW(IDS_STRING_VMS_VOLUME);
	//pStatic=new CStatic;
	//pStatic->Create(
	//	str,
	//	WS_CHILD
	//	|WS_VISIBLE, 
	//	CRect(pt,staticSize),
	//	this,
	//	IDS_STRING_VMS_VOLUME);

	//pt.x+=gap2.cx+staticSize.cx;

	//str.LoadStringW(IDS_EDIT_VMS_VOLUME);
	////str=L"0";
	//pEdit=new CEdit;
	//pEdit->CreateEx(
	//	WS_EX_CLIENTEDGE,
	//	L"Edit", 
	//	str,
	//	ES_LEFT
	//	|WS_CHILD
	//	|WS_VISIBLE,
	//	CRect(pt,CSize(winrect.Width()-gap2.cx-staticSize.cx,staticSize.cy)),
	//	this,
	//	IDS_EDIT_VMS_VOLUME);

	//pt.y+=staticSize.cy+gap2.cy;
	//pt.x-=gap2.cx+staticSize.cx;




	const DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		| WS_BORDER
		| WS_HSCROLL 
		| WS_VSCROLL 
		| LBS_NOINTEGRALHEIGHT;

	if(!lin.Create(dwStyle, CRect(pt,listsz), this, 3000) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	pt.y+=listsz.cy+gap2.cy;

	if(!lout.Create(dwStyle, CRect(pt,listsz), this, 3001) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	pt.y+=listsz.cy+gap2.cy;

	btn.Create(L"compute",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,CRect(pt,staticSize),this,3002);

	pt.x+=staticSize.cx+gap2.cx;

	btn2.Create(L"copy",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,CRect(pt,staticSize),this,3003);

	return 0;
}


int testdlg::GetSIFList(std::vector<sapitemF> & sfl)
{

	sfl.clear();


	int nItem=lin.GetItemCount();


	for(size_t i=0;i<nItem;i++){

		sapitemF sf;

		lin.GetItemSAPB(i,sf);

		sfl.push_back(sf);
	}


	return nItem;

	return 0;
}


int testdlg::ComputeDOA(const std::vector<sapitemF> & sfl, std::vector<DataOutA> & dol)
{
	dol.clear();


	DataOutA doa;
	for(size_t i=0;i<sfl.size();i++){

		pcct data;
		BOOL fg=data.readFile1(sfl[i].file);

		if(fg==FALSE || data.current.empty()){
			AfxMessageBox(L"read file error");
			return 1;
		}

		data.TomA();
		double a;
		if(data.potential.empty()){
			a=data.intg1();
		}
		else{
			a=data.intg2(1)*10;
		}


		switch(sfl[i].sia.addType){
		case 0:
			doa.VolOnce(sfl[i].sia);
			doa.Ar.assign(1,a);
			doa.UseIndex=0;
			doa.stepName=sfl[i].file;
			break;
		case 4:
			doa.VMSOnce(sfl[i].sia);
			doa.Ar0=a;
			doa.Ar.assign(1,a);
			doa.UseIndex=0;
			doa.stepName=sfl[i].file;
			break;
		default:
			break;
		}

		dol.push_back(doa);
	}





	return 0;
}


void testdlg::Onbtn1(void)
{

	std::vector<sapitemF> sfl;

	GetSIFList(sfl);

	std::vector<DataOutA> dol;

	ComputeDOA(sfl,dol);

	lout.DeleteAllItems();
	for(size_t i=0;i<dol.size();i++){
		lout.InsertItemDOA(i,dol[i]);
	}
}


void testdlg::OnCopy(void)
{

	

	CString seg=L"\t";
	CString endline=L"\r\n";

	CString source=L"";
	//L"file"+seg
	//+L"base"+seg
	//+L"additive"+seg
	//+L"total"+seg
	//+L"s"+seg
	//+L"a"+seg
	//+L"l"+seg
	//+L"q"+seg
	//+L"q0"+seg
	//+L"nq"+endline; 


	CHeaderCtrl* pHeaderCtrl = lout.GetHeaderCtrl();

	int nCount = pHeaderCtrl->GetItemCount();

	HDITEM hdi;
	TCHAR  lpBuffer[512];
	hdi.pszText = lpBuffer;
	hdi.mask = HDI_TEXT;
	hdi.cchTextMax = 512;


	for(int i=0;i<nCount;i++){
		pHeaderCtrl->GetItem(i,&hdi);
		source+=hdi.pszText;
		source+=(i<nCount-1)?seg:endline;

	}



	//put your text in source 

	POSITION pos = lout.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		TRACE(_T("No items were selected!\n"));
	}
	else
	{
		while (pos)
		{
			int nItem = lout.GetNextSelectedItem(pos);
			TRACE(_T("Item %d was selected!\n"), nItem);
			// you could do your own processing on nItem here				


			//source+=lout.GetItemText(nItem,0);
			//source+=seg;
			//source+=lout.GetItemText(nItem,1);
			//source+=seg;
			//source+=lout.GetItemText(nItem,2);
			//source+=seg;
			//source+=lout.GetItemText(nItem,3);
			//source+=seg;
			//source+=lout.GetItemText(nItem,4);
			//source+=seg;
			//source+=lout.GetItemText(nItem,5);
			//source+=seg;
			//source+=lout.GetItemText(nItem,6);
			//source+=seg;
			//source+=lout.GetItemText(nItem,7);
			//source+=seg;
			//source+=lout.GetItemText(nItem,8);
			//source+=seg;
			//source+=lout.GetItemText(nItem,9);

			//source+=endline;

			for(int i=0;i<nCount;i++){		
				source+=lout.GetItemText(nItem,i);
				source+=(i<nCount-1)?seg:endline;
			}

		}
	}



	if(OpenClipboard()) 
	{ 
		HGLOBAL clipbuffer; 
		//wchar_t * buffer; 
		char * buffer; 
		EmptyClipboard(); 
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1); 
		//buffer = (wchar_t*)GlobalLock(clipbuffer); 
		buffer = (char*)GlobalLock(clipbuffer); 
		//wcscpy(buffer, (source)); 
		wcstombs(buffer,source.GetBuffer(),source.GetLength()+1);

		GlobalUnlock(clipbuffer); 
		SetClipboardData(CF_TEXT,clipbuffer); 
		CloseClipboard(); 
		
		MessageBox(_T("Copy output"));
	}
	else	
		MessageBox(_T("Copy output fail"));
}
