#pragma once
#include "afxsplitterwndex.h"
class CSplitterWndExA :
	public CSplitterWndEx
{
public:
	COLORREF barColor;
public:
	CSplitterWndExA(void){};
	virtual ~CSplitterWndExA(void){};
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
	{
		// TODO: Add your specialized code here and/or call the base class

		if(pDC!=NULL){
			switch(nType){ 
			case splitBorder:
				TRACE("\nborder");
				break;
			case splitBox:
				TRACE("\nbox");
				break;
			case splitBar:  
				TRACE("\nbar");
				pDC->FillSolidRect(&rect,barColor);
				return;
				break;
			case splitIntersection:	
				TRACE("\ninter");
				break;
			default:	
				TRACE("\ndefault");
				break;
			} 
		}
		CSplitterWndEx::OnDrawSplitter(pDC, nType, rect);
	};

};

