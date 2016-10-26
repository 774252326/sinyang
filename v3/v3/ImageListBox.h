#pragma once


// ImageListBox

class ImageListBox : public CListBox
{
	DECLARE_DYNAMIC(ImageListBox)

public:
	ImageListBox();
	virtual ~ImageListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPopupAdd();
	afx_msg void OnPopupRemove();
	afx_msg void OnLbnDblclk();
};


