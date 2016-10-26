#pragma once


// EditC

class EditC : public CEdit
{
	DECLARE_DYNAMIC(EditC)

public:
	EditC();
	virtual ~EditC();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnKillfocus();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


