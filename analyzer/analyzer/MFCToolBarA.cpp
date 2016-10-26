#include "StdAfx.h"
#include "MFCToolBarA.h"

IMPLEMENT_DYNAMIC(CMFCToolBarA, CMFCToolBar)

CMFCToolBarA::CMFCToolBarA(void)
{
}


CMFCToolBarA::~CMFCToolBarA(void)
{
}

BEGIN_MESSAGE_MAP(CMFCToolBarA, CMFCToolBar)

END_MESSAGE_MAP()

BOOL CMFCToolBarA::DrawButton(
     CDC* pDC,
     CMFCToolBarButton* pButton,
     CMFCToolBarImages* pImages,
     BOOL bHighlighted,
     BOOL bDrawDisabledImages 
  )
{

	//COLORREF oc=pDC->GetTextColor();
	//pDC->SetTextColor( RGB(255,255,0) );

	return CMFCToolBar::DrawButton(pDC, pButton, pImages, bHighlighted, bDrawDisabledImages );
}


void CMFCToolBarA::DoPaint(
     CDC* pDC 
  )
{
	//pDC->SetTextColor( RGB(255,255,0) );

	CMFCToolBar::DoPaint(pDC);
}

