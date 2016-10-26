#pragma once


// ImageListView view

class ImageListView : public CListView
{
	DECLARE_DYNCREATE(ImageListView)

protected:
	ImageListView();           // protected constructor used by dynamic creation
	virtual ~ImageListView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


