#pragma once



// dlg2 form view

class dlg2 : public CFormView
{
	DECLARE_DYNCREATE(dlg2)

protected:
	dlg2();           // protected constructor used by dynamic creation
	virtual ~dlg2();

public:
	enum { IDD = IDD_DIALOG2 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
};


