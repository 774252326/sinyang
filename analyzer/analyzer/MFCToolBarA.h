#pragma once
#include "afxtoolbar.h"
class CMFCToolBarA :
	public CMFCToolBar
{
	DECLARE_DYNAMIC(CMFCToolBarA)
public:
	CMFCToolBarA(void);
	virtual ~CMFCToolBarA(void);
protected:
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL DrawButton(
     CDC* pDC,
     CMFCToolBarButton* pButton,
     CMFCToolBarImages* pImages,
     BOOL bHighlighted,
     BOOL bDrawDisabledImages 
  );

virtual void DoPaint(
     CDC* pDC 
  );


};

