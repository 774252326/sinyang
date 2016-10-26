#pragma once

#include "../struct\SAPara.hpp"
#include "../list\ListCtrlSAP.hpp"
#include "../messagemaphpp.h"

// SolutionAdditionParametersPageA dialog

class SolutionAdditionParametersPageA : public CPropertyPage
{
	//DECLARE_DYNAMIC(SolutionAdditionParametersPageA)
public:
	SAPara para;
	ListCtrlSAP m_SAPlist;
	CStatic stt;
	// Dialog Data
	enum { IDD = IDD_DIALOG_SAP };

public:
	SolutionAdditionParametersPageA()
		: CPropertyPage(SolutionAdditionParametersPageA::IDD)

	{
		CString title;
		title.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
		m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
		m_psp.pszTitle = new TCHAR[title.GetLength()+1];
		_tcscpy((wchar_t*)m_psp.pszTitle, title);

		m_psp.dwFlags &=~PSP_HASHELP;
	};
	virtual ~SolutionAdditionParametersPageA()
	{
		delete [] m_psp.pszTitle;
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CPropertyPage::DoDataExchange(pDX);
	};    // DDX/DDV support
	void SetList(void)
	{
		m_SAPlist.DeleteAllItems();
		for(size_t i=0;i<para.saplist.size();i++){
			m_SAPlist.InsertItemSAP(i,para.saplist[i]);
		}
	};
	void GetList(void)
	{
		int nItem=m_SAPlist.GetItemCount();
		para.saplist.resize(nItem);
		for(size_t i=0;i<nItem;i++){
			m_SAPlist.GetItemSAP(i,para.saplist[i]);
		}
	};
	virtual BOOL OnKillActive()
	{
		// TODO: Add your specialized code here and/or call the base class

		GetList();

		return CPropertyPage::OnKillActive();
	};
	virtual BOOL OnSetActive()
	{
		// TODO: Add your specialized code here and/or call the base class

		// 获得父窗口，即属性表CPropertySheet类   
		CPropertySheet* psheet = (CPropertySheet*) GetParent();   

		psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);

		//设置属性表只有“完成”按钮   
		//psheet->SetFinishText(_T("完成"));  

		return CPropertyPage::OnSetActive();
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
		CStatic *pStatic;
		CEdit *pEdit;
		CString str;

		str.LoadStringW(IDS_STRING_SAP_TIPS);
		stt.Create(
			str,
			WS_CHILD
			|WS_VISIBLE, 
			CRect(pt,CSize(winrect.Width(),staticSize.cy)),
			this,
			10000);

		pt.y+=gap2.cy+staticSize.cy;


		const DWORD dwStyle = WS_VISIBLE 
			| WS_CHILD 
			| WS_BORDER
			| WS_HSCROLL 
			| WS_VSCROLL 
			| LBS_NOINTEGRALHEIGHT;

		if(!m_SAPlist.Create(dwStyle, CRect(pt,CPoint(winrect.Size()+gap1)), this, IDS_LISTCTRL_SAP) ){
			TRACE0("Failed to create output windows\n");
			return -1;      // fail to create
		}

		SetList();

		return 0;
	};

//	DECLARE_MESSAGE_MAP()
//};
//BEGIN_MESSAGE_MAP(SolutionAdditionParametersPageA, CPropertyPage)
//	ON_WM_CREATE()
//END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP_HPP(SolutionAdditionParametersPageA, CPropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP_HPP()
};