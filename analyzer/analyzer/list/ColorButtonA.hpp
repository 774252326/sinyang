#pragma once
#include "atltypes.h"

#include "../messagemaphpp.h"

// ColorButtonA

class ColorButtonA : public CMFCColorButton 
{
	//DECLARE_DYNAMIC(ColorButtonA)
protected:
	CPoint coord;
	BOOL m_bVK_ESCAPE;
	short flag;
public:
	ColorButtonA(CPoint coord0)
		: flag(0)
	{
		coord=coord0;
		m_bVK_ESCAPE = FALSE;
		//m_bAutoSetFocus=FALSE;
	};
	virtual ~ColorButtonA(){};

protected:
	afx_msg void OnKillFocus(CWnd* pNewWnd)
	{
		TRACE("\non kill focus %d",flag);

		CMFCButton::OnKillFocus(pNewWnd);

		// TODO: Add your message handler code here

		//if(flag==3)
		//{
		//PostMessage(WM_CLOSE);
		//}
	};

	virtual void UpdateColor(COLORREF color)
	{
		// TODO: Add your specialized code here and/or call the base class

		TRACE("\nupdate color %d",flag);

		flag=1;

		CString str;	
		str.Format(L"%d",color);
		// Send Notification to parent of ListView ctrl	
		LV_DISPINFO lvDispinfo;
		lvDispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
		lvDispinfo.hdr.idFrom = GetDlgCtrlID();//that's us
		lvDispinfo.hdr.code = LVN_ENDLABELEDIT;
		lvDispinfo.item.mask = LVIF_TEXT | LVIF_PARAM;	
		lvDispinfo.item.iItem = coord.x;
		lvDispinfo.item.iSubItem = coord.y;
		lvDispinfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
		lvDispinfo.item.cchTextMax = str.GetLength();
		//lvDispinfo.item.lParam = GetItemData(GetCurSel());
		//if(nIndex!=CB_ERR)
		GetParent()->GetParent()->SendMessage(
			WM_NOTIFY,
			GetParent()->GetDlgCtrlID(),
			(LPARAM)&lvDispinfo);


		GetParent()->SetFocus();	


		//return CMFCColorButton::UpdateColor(color);
		CMFCColorButton::UpdateColor(color);
		PostMessage(WM_CLOSE);
	};

	afx_msg void OnNcDestroy()
	{

		TRACE("\nncdestroy %d",flag);

		CMFCColorButton::OnNcDestroy();

		// TODO: Add your message handler code here

		delete this;
	};

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CMFCColorButton::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here
		//flag=3;

		SetFocus();

		TRACE("\ncreate %d",flag);


		//EnableOtherButton(L"other");
		OnShowColorPopup();
		return 0;
	};

	virtual void OnShowColorPopup()
	{
		TRACE("\non show popup %d",flag);
		// TODO: Add your specialized code here and/or call the base class

		flag=2;

		return CMFCColorButton::OnShowColorPopup();
	};

	afx_msg void OnSetFocus(CWnd* pOldWnd)
	{
		TRACE("\non set focus %d",flag);

		CMFCColorButton::OnSetFocus(pOldWnd);

		// TODO: Add your message handler code here



		if(/*flag==3||*/flag==2/*||flag==1*/){
			PostMessage(WM_CLOSE);
		}
	};


	void OnDraw(CDC* pDC, const CRect& rect, UINT uiState)
	{
		ASSERT_VALID(pDC);

		if (m_pPalette == NULL)
		{
			RebuildPalette(NULL);
		}

		CPalette* pCurPalette = pDC->SelectPalette(m_pPalette, FALSE);
		pDC->RealizePalette();

		CSize sizeArrow = CMenuImages::Size();

		CRect rectColor = rect;

		COLORREF color = m_Color;
		if (color == (COLORREF) -1) // Automatic
		{
			//---------------------------
			// Draw automatic text label:
			//---------------------------
			color = m_ColorAutomatic;

		}

		//----------------
		// Draw color box:
		//----------------
		rectColor.DeflateRect(2, 2);
		pDC->Draw3dRect(rectColor, afxGlobalData.clrBtnHilite, afxGlobalData.clrBtnHilite);
		rectColor.DeflateRect(1, 1);
		pDC->Draw3dRect(rectColor, afxGlobalData.clrBtnDkShadow, afxGlobalData.clrBtnDkShadow);
		rectColor.DeflateRect(1, 1);

		if (color != (COLORREF)-1 &&(uiState & ODS_DISABLED) == 0)
		{
			if (afxGlobalData.m_nBitsPerPixel == 8) // 256 colors
			{
				ASSERT_VALID(m_pPalette);
				color =  PALETTEINDEX(m_pPalette->GetNearestPaletteIndex(color));
			}

			CBrush br(color);
			pDC->FillRect(rectColor, &br);
		}	

		if (pCurPalette != NULL)
		{
			pDC->SelectPalette(pCurPalette, FALSE);
		}
	};

	void OnDrawFocusRect(CDC* pDC, const CRect& rectClient)
	{
		CSize sizeArrow = CMenuImages::Size();

		CRect rectColor = rectClient;	

		CMFCButton::OnDrawFocusRect(pDC, rectColor);
	};

	//	DECLARE_MESSAGE_MAP()
	//
	//};
	//
	//BEGIN_MESSAGE_MAP(ColorButtonA, CMFCColorButton)
	//	ON_WM_KILLFOCUS()
	//	ON_WM_NCDESTROY()
	//	ON_WM_CREATE()
	//	ON_WM_SETFOCUS()
	//END_MESSAGE_MAP()


	BEGIN_MESSAGE_MAP_HPP(ColorButtonA, CMFCColorButton)
		ON_WM_KILLFOCUS()
		ON_WM_NCDESTROY()
		ON_WM_CREATE()
		ON_WM_SETFOCUS()
		END_MESSAGE_MAP_HPP()

};