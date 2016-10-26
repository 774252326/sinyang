#pragma once

//#include "typedefine.h"
//#include "VPara.h"
#include "../struct\VPara.hpp"
#include "../Resource.h"
//#include "CVParametersPage.h"
#include "afxdialogex.h"
#include <cfloat>

// CVParametersPage dialog

class CVParametersPage : public CPropertyPage
{
	//DECLARE_DYNAMIC(CVParametersPage)
public:
	CVPara para;
	DWORD dwStyle;
	// Dialog Data
	enum { IDD = IDD_DIALOG_CVP };
protected:
	CStatic StaticCVP[7];
	CEdit EditCVP[7];
	CStatic StaticIDS_STRING_START_INTEGRATION_E;
	CComboBox ComboIDS_EDIT_START_INTEGRATION_E;
public:
	CVParametersPage()
		: CPropertyPage(CVParametersPage::IDD)
		, dwStyle(WS_CHILD|WS_VISIBLE)
	{
		CString title;
		title.LoadStringW(IDS_STRING_CV_PARA);
		m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE | PSH_HASHELP ; 	
		m_psp.pszTitle = new TCHAR[title.GetLength()+1];
		_tcscpy((wchar_t*)m_psp.pszTitle, title);
		m_psp.dwFlags &=~PSP_HASHELP;
	};
	virtual ~CVParametersPage()
	{
		delete [] m_psp.pszTitle;
	};



protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		DDX_Text(pDX, IDS_EDIT_LOW_E_LIMIT, para.lowelimit);

		DDX_Text(pDX, IDS_EDIT_HIGH_E_LIMIT, para.highelimit);
		DDV_MinMaxDouble(pDX,para.highelimit,para.lowelimit,DBL_MAX);

		DDX_Text(pDX, IDS_EDIT_SCAN_RATE, para.scanrate);
		DDV_MinMaxDouble(pDX,para.scanrate,0,DBL_MAX);
		DDX_Text(pDX, IDS_EDIT_ROTATION_RATE, para.rotationrate);
		DDV_MinMaxDouble(pDX,para.rotationrate,0,DBL_MAX);


		DDX_Text(pDX, IDS_EDIT_VARIATION_TOLERANCE, para.variationtolerance);
		DDV_MinMaxDouble(pDX,para.rotationrate,0,DBL_MAX);

		DDX_Text(pDX, IDS_EDIT_NO_OF_CYCLES, para.noofcycles);
		DDV_MinMaxInt(pDX,para.noofcycles,1,50);

		DDX_Text(pDX, IDS_EDIT_END_INTEGRATION_E, para.endintegratione);
		DDV_MinMaxDouble(pDX, para.endintegratione, para.lowelimit+para.EInterval(), para.highelimit-para.EInterval());

		DDX_CBIndex(pDX, IDS_EDIT_START_INTEGRATION_E, para.combochoice);

		if(para.combochoice!=0){
			//strtmp.Format(L"%g",para.startintegratione);
			DDX_Text(pDX, IDS_EDIT_START_INTEGRATION_E, para.startintegratione);
			DDV_MinMaxDouble(pDX, para.startintegratione, para.lowelimit+para.EInterval(), para.endintegratione-para.EInterval());
		} 

		CPropertyPage::DoDataExchange(pDX);
	};    // DDX/DDV support

	virtual BOOL OnSetActive()
	{
		// TODO: Add your specialized code here and/or call the base class

		// 获得父窗口，即属性表CPropertySheet类   
		CPropertySheet* psheet = (CPropertySheet*) GetParent();   

		psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);

		return CPropertyPage::OnSetActive();
	};

	virtual BOOL OnKillActive()
	{
		// TODO: Add your specialized code here and/or call the base class

		if(UpdateData()==FALSE){
			return FALSE;
		}

		return CPropertyPage::OnKillActive();
	};

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		CSize gap1(20,20);
		CSize gap2(20,20);
		CSize staticSize(150,22);

		CRect winrect;
		this->GetWindowRect(&winrect);

		winrect.DeflateRect(gap1);

		CPoint pt(gap1);

		CSize editSize((winrect.Width()-3*gap2.cx)/2-staticSize.cx,staticSize.cy);

		CString str;

		int i=0;
		for(;i<7;){

			str.LoadStringW(IDS_STRING_LOW_E_LIMIT+i);
			//pStatic=new CStatic;
			StaticCVP[i].Create(
				str,
				dwStyle,
				CRect(pt,staticSize),
				this,
				IDS_STRING_LOW_E_LIMIT+i);

			pt.x+=gap2.cx+staticSize.cx;

			str.LoadStringW(IDS_EDIT_LOW_E_LIMIT+i);
			//str=L"0";
			//pEdit=new CEdit;
			EditCVP[i].CreateEx(
				WS_EX_CLIENTEDGE,
				L"Edit", 
				str,
				ES_LEFT
				|dwStyle,
				CRect(pt,editSize),
				this,
				IDS_EDIT_LOW_E_LIMIT+i);
			if(i%2==0){
				pt.x+=gap2.cx+editSize.cx;
			}
			else{
				pt.y+=staticSize.cy+gap2.cy;
				pt.x=gap1.cx;
			}

			i++;
		}



		str.LoadStringW(IDS_STRING_START_INTEGRATION_E);
		//pStatic=new CStatic;
		StaticIDS_STRING_START_INTEGRATION_E.Create(
			str,
			dwStyle, 
			CRect(pt,staticSize),
			this,
			IDS_STRING_START_INTEGRATION_E);

		pt.x+=gap2.cx+staticSize.cx;




		//pCombo=new CComboBox;
		ComboIDS_EDIT_START_INTEGRATION_E.Create(
			CBS_DROPDOWN
			//CBS_DROPDOWNLIST
			//|WS_TILED
			|dwStyle, 
			CRect(pt,editSize),
			this,
			IDS_EDIT_START_INTEGRATION_E);


		str.LoadStringW(IDS_STRING_AUTO);
		ComboIDS_EDIT_START_INTEGRATION_E.AddString(str);

		return 0;
	};


	DECLARE_MESSAGE_MAP()


};

BEGIN_MESSAGE_MAP(CVParametersPage, CPropertyPage)	
	ON_WM_CREATE()
END_MESSAGE_MAP()