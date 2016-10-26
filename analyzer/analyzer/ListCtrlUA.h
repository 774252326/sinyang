#pragma once
#include "listctrla.h"
#include "UserAccount.h"

class ListCtrlUA :
	public ListCtrlA
{
public:
	ListCtrlUA(void);
	ListCtrlUA(bool flg);
	virtual ~ListCtrlUA(void);
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	bool bEditable;
//	afx_msg void OnPopupInsert();
//	afx_msg void OnPopupRemove();
//	afx_msg void OnPopupRemoveall();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

public:
	//int useIndex;
	int InsertItemUA(int i, const UserAccount & ua, bool bUse);
	bool GetItemUA(int i, UserAccount & ua, bool & bUse);
//	void GetList(void);
	void ShowPW(void);
	afx_msg void OnPopupDeleteuser();
	afx_msg void OnPopupAdduser();
	void SetEditable(bool b);
};

