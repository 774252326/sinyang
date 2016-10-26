#pragma once


// CColorButton

class CColorButton : public CButton
{
	DECLARE_DYNAMIC(CColorButton)

public:
	CColorButton();
	virtual ~CColorButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	COLORREF color;
	afx_msg void OnBnClicked();
};


