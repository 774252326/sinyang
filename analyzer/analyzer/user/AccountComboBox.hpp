#pragma once

#include "AccountList.hpp"
#include "../Resource.h"

#include "../messagemaphpp.h"

// AccountComboBox

class AccountComboBox : public CComboBoxEx
{
	//DECLARE_DYNAMIC(AccountComboBox)
public:
	CImageList m_ImageList;
	AccountList al;
public:
	AccountComboBox(){};
	virtual ~AccountComboBox(){};

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	};

//	DECLARE_MESSAGE_MAP()
//};
//
//BEGIN_MESSAGE_MAP(AccountComboBox, CComboBoxEx)
//	ON_WM_CREATE()
//END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP_HPP(AccountComboBox, CComboBoxEx)
	ON_WM_CREATE()
END_MESSAGE_MAP_HPP()
};

