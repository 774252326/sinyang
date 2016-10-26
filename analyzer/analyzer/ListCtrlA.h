#pragma once
//#include "edita.h"
//#include "ComboBoxA.h"
#include "ComboItem.h"
#include <vector>

#include "EditB.h"
#include "EditInt.h"
#include "EditNum.h"
#include "EditReal.h"

// ListCtrlA

class ListCtrlA : public CListCtrl
{
	DECLARE_DYNAMIC(ListCtrlA)

public:
	ListCtrlA();
	virtual ~ListCtrlA();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	int m_iSubItem;
	//EditA m_edtItemEdit;

	EditB m_itemEdit;
	EditReal m_itemEditReal;
	EditInt m_itemEditInt;
	EditNum m_itemEditNum;


	//ComboBoxA m_edtItemEdit;
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);

	enum eType{
		eStatic,
		eEdit,
		eEditReal,
		eEditInt,
		eEditNum,
		eCombo,
		eColor	
	};

	std::vector<eType> typelist;

	std::vector< std::vector<CString> > cbstr;
	std::vector<double> mind;
	std::vector<double> maxd;
	std::vector<int> mini;
	std::vector<int> maxi;

		CComboBox * ComboItem( int nItem,  int nSubItem);
		int GetChoice(int nItem, int nSubItem);

		afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
		virtual void DoDataExchange(CDataExchange* pDX);

		double aa;
		int bb;


		BOOL SetHeader(const std::vector<CString> & headerstrl);
};


