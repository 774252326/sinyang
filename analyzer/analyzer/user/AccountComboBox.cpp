// AccountComboBox.cpp : implementation file
//

#include "stdafx.h"
//#include "analyzer.h"
#include "AccountComboBox.h"
#include "../Resource.h"

// AccountComboBox

IMPLEMENT_DYNAMIC(AccountComboBox, CComboBoxEx)

AccountComboBox::AccountComboBox()
{

}

AccountComboBox::~AccountComboBox()
{
}


BEGIN_MESSAGE_MAP(AccountComboBox, CComboBoxEx)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, &AccountComboBox::OnCbnEditchange)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &AccountComboBox::OnCbnSelchange)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, &AccountComboBox::OnCbnCloseup)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, &AccountComboBox::OnCbnKillfocus)
	ON_NOTIFY_REFLECT(CBEN_ENDEDIT, &AccountComboBox::OnCbenEndedit)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, &AccountComboBox::OnCbnEditupdate)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, &AccountComboBox::OnCbnSelendcancel)
END_MESSAGE_MAP()



// AccountComboBox message handlers




int AccountComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBoxEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	m_ImageList.Create(16, 16, ILC_COLOR, 3, 3);

	HICON hIcon1;
	hIcon1 = AfxGetApp()->LoadIcon(IDI_ICON2);
	m_ImageList.Add(hIcon1);

	hIcon1 = AfxGetApp()->LoadIcon(IDI_ICON3);
	m_ImageList.Add(hIcon1);

	hIcon1 = AfxGetApp()->LoadIcon(IDI_ICON4);
	m_ImageList.Add(hIcon1);

	//关联图像列表和扩展组合框

	//m_pw.SetImageList(&m_ImageList);
	SetImageList(&m_ImageList);


	COMBOBOXEXITEM     cbi;//扩展组合框单元
	cbi.mask = CBEIF_IMAGE| CBEIF_INDENT | CBEIF_OVERLAY |CBEIF_SELECTEDIMAGE | CBEIF_TEXT;


	for(size_t i=0;i<al.ual.size();i++){
		cbi.iItem = i;
		cbi.iImage = al.ual[i].au;
		cbi.iSelectedImage = al.ual[i].au;
		cbi.pszText = al.ual[i].userName.GetBuffer();
		cbi.iOverlay = al.ual[i].au+1;
		cbi.iIndent=0;
		InsertItem(&cbi);

		//m_usr.SetItemHeight(i,20);
	}

	CComboBox *cb=GetComboBoxCtrl();
	cb->SetCurSel(0);
	cb->SetMinVisibleItems(3);
	SetWindowTextW(al.ual[0].userName);

	return 0;
}


void AccountComboBox::OnCbnEditchange()
{
	// TODO: Add your control notification handler code here

	//		CComboBox *cb=GetComboBoxCtrl();
	//int ci=cb->GetCurSel();
	//if(ci==CB_ERR)
	//	cb->SetCurSel(0);
}


void AccountComboBox::OnCbnSelchange()
{
	// TODO: Add your control notification handler code here

	//CComboBox *cb=GetComboBoxCtrl();
	//int ci=cb->GetCurSel();
	//if(ci==CB_ERR)
	//	cb->SetCurSel(0);

}


void AccountComboBox::OnCbnCloseup()
{
	// TODO: Add your control notification handler code here

	//CComboBox *cb=GetComboBoxCtrl();
	//int ci=cb->GetCurSel();
	//if(ci==CB_ERR)
	//	cb->SetCurSel(0);
}


void AccountComboBox::OnCbnKillfocus()
{
	 //TODO: Add your control notification handler code here
	//	CComboBox *cb=GetComboBoxCtrl();
	//int ci=cb->GetCurSel();
	//if(ci==CB_ERR)
	//	cb->SetCurSel(0);
}


void AccountComboBox::OnCbenEndedit(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	//		CComboBox *cb=GetComboBoxCtrl();
	//int ci=cb->GetCurSel();
	//if(ci==CB_ERR)
	//	cb->SetCurSel(0);



	*pResult = 0;
}


void AccountComboBox::OnCbnEditupdate()
{
	// TODO: Add your control notification handler code here

	//			CComboBox *cb=GetComboBoxCtrl();
	//int ci=cb->GetCurSel();
	//if(ci==CB_ERR)
	//	cb->SetCurSel(0);
}


void AccountComboBox::OnCbnSelendcancel()
{
	// TODO: Add your control notification handler code here
}
