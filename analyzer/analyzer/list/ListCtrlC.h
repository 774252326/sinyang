#pragma once
#include "listctrlb.h"
#include "EditC.hpp"


class ListCtrlC :
	public ListCtrlB
{
	DECLARE_DYNAMIC(ListCtrlC)

public:
	enum eType{
		eStatic,
		eEdit,
		eEditReal,
		eEditInt,
		eCombo,
		eColor	
	};

public:
	std::vector<eType> typelist;
	std::vector< std::vector<CString> > cbstr;
	std::vector<double> mind;
	std::vector<double> maxd;
	std::vector<int> mini;
	std::vector<int> maxi;

	EditC<int> m_itemEditInt;
	EditC<double> m_itemEditReal;

public:
	ListCtrlC(void);
	virtual ~ListCtrlC(void);
	CComboBox * ComboItem(CPoint coord);
	CMFCColorButton * ColorItem(CPoint coord);

	CRect GetBoundRect(int nItem, int nSubItem)
	{
		//basic code start
		CRect rect;

		// Make sure that the item is visible
		if( EnsureVisible(nItem, TRUE) )
		{
			GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
			// Now scroll if we need to expose the column
			CRect rcClient;
			GetClientRect(rcClient);
			int offset = 0;
			if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
			{
				CSize size;
				size.cx = offset + rect.left;
				size.cy = 0;
				Scroll(size);
				rect.left -= size.cx;
			}
			rect.left += offset;	
			rect.right = rect.left + GetColumnWidth(nSubItem);
			if(rect.right > rcClient.right) 
				rect.right = rcClient.right;
			//basic code end

			rect.bottom += 30 * rect.Height();//dropdown area
		}
		return rect;

	};

	int GetChoice(int nItem, int nSubItem)
	{
		CString strTemp, strTemp2;
		strTemp=GetItemText(nItem,nSubItem);
		for(size_t i=0;i<cbstr[nSubItem].size();i++){
			if(cbstr[nSubItem][i]==strTemp){
				return i;
			}
		}
		return -1;
	};

	BOOL SetChoice(int nItem, int nSubItem, int choiceIndex)
	{
		if( nItem<0 || nSubItem<0 
			|| nSubItem>=cbstr.size() 
			|| choiceIndex<0 
			|| choiceIndex>=cbstr[nSubItem].size() ){
				return FALSE;
		}
		return SetItemText(nItem,nSubItem,cbstr[nSubItem][choiceIndex]);
	};

	BOOL SetHeader(const std::vector<CString> & headerstrl)
	{
		BOOL flg=ListCtrlB::SetHeader(headerstrl);
		if(flg==TRUE){
			size_t totaln=headerstrl.size();

			typelist.resize(totaln,eStatic);
			std::vector<CString> strl;
			cbstr.resize(totaln,strl);
			mind.resize(totaln,0);
			mini.resize(totaln,0);
			maxd.resize(totaln,-1);
			maxi.resize(totaln,-1);
		}
		return flg;
	};

protected:
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

	virtual void DoDataExchange(CDataExchange* pDX)
	{
		// TODO: Add your specialized code here and/or call the base class
		if(m_itemEditInt.GetSafeHwnd())
		{
			m_itemEditInt.DoDataExchange(pDX);
		}
		if(m_itemEditReal.GetSafeHwnd())
		{
			m_itemEditReal.DoDataExchange(pDX);
		}

		ListCtrlB::DoDataExchange(pDX);
	};

	DECLARE_MESSAGE_MAP()
};

