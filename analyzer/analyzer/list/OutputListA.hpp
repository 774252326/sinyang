#pragma once
#include "listctrla.hpp"
#include "../struct/DataOutA.hpp"
#include "../resource.h"
#include "../analyzerDoc.h"

#include "../messagemaphpp.h"

///
/// \brief The COutputListA class
///输出窗口的表格
/// 用于显示运行状态

class COutputListA :
	public ListCtrlA
{

public:
    ///
    /// \brief dol
    ///溶液状态
	std::vector<DataOutA> dol;
public:
	COutputListA(void){};
	virtual ~COutputListA(void){};	


    ///
    /// \brief ShowDOL
    ///在表格中显示溶液状态
	void ShowDOL()
	{
		size_t StepNo;
		size_t starti;
		StepNo=0;
		starti=0;

		CString strcycle;
		strcycle.LoadStringW(IDS_STRING_CYCLE);

		for(size_t i=starti;i<dol.size();i++){

			CString StepName=dol[i].GetStepName(i);

			for(size_t ari=0;ari<dol[i].Ar.size();ari++){

				CString str;
				str.Format(L"%d",StepNo);
				if(StepNo>=GetItemCount()){					
					InsertItem(StepNo,str);
				}
				else{
					SetItemText(StepNo,0,str);				
				}
				AdjustWidth(0,str);

				str.Format(L"%s(%s%d)",StepName,strcycle,ari+1);
				SetItemText(StepNo,1,str);
				AdjustWidth(1,str);

				if(ari==0){
					str.Format(L"%g",dol[i].addVolume);
					SetItemText(StepNo,2,str);
					AdjustWidth(2,str);
				}
				else{
					SetItemText(StepNo,2,L"");
				}

				str.Format(L"%g",dol[i].TotalVolume());
				SetItemText(StepNo,3,str);
				AdjustWidth(3,str);

				str.Format(L"%g",dol[i].Ar[ari]);
				SetItemText(StepNo,4,str);
				AdjustWidth(4,str);


				if(ari==dol[i].UseIndex){
					str.Format(L"%g",dol[i].Ar[ari]/dol[i].Ar0);
					SetItemText(StepNo,5,str);
					AdjustWidth(5,str);
					//ASSERT
					(str.LoadString(IDS_STRING_YES));
					//SetChoice(StepNo,6,0);
					SetItemText(StepNo,6,str);
				}
				else{
					SetItemText(StepNo,5,L"");
					//ASSERT				
					(str.LoadString(IDS_STRING_NO));
					//SetChoice(StepNo,6,1);
					SetItemText(StepNo,6,str);
				}
				AdjustWidth(6,str);

				StepNo++;
			}
		}

		EnsureVisible(StepNo-1, FALSE);

		while(GetItemCount()>StepNo){
			DeleteItem(StepNo);
		}

	};

    ///
    /// \brief GetDOLRow
    /// 计算要显示全部溶液状态所要的行数
    /// \return
    ///
	size_t GetDOLRow(void)
	{
		size_t s=0;
		for(size_t i=0;i<dol.size();i++){
			s+=dol[i].Ar.size();
		}
		return s;
	};

public:
    ///
    /// \brief BuildList
    ///设置表头
	void BuildList(void)
	{	
		std::vector<CString> strl;
		CString strTemp;
		for(int i=IDS_STRING111;i<=IDS_STRING117;i++){		
			(strTemp.LoadString(i));
			strl.push_back(strTemp);
		}

		this->SetHeader(strl);

		//cbstr[6].clear();
		//(strTemp.LoadString(IDS_STRING_YES));
		//cbstr[6].push_back(strTemp);
		//(strTemp.LoadString(IDS_STRING_NO));
		//cbstr[6].push_back(strTemp);

		for(int i=0;i<strl.size();i++){		
			AdjustWidth(i,strl[i],30);
		}
	};
    ///
    /// \brief ResetHeader
    ///设置表头
	void ResetHeader(void)
	{	
		BuildList();
	};

protected:

	afx_msg LRESULT OnMessageShowDol(WPARAM wParam, LPARAM lParam)
	{

		CanalyzerDoc *ow=(CanalyzerDoc*)(this->GetParentFrame()->GetActiveDocument());
		CSingleLock singleLock(&(ow->m_CritSection));
		if (singleLock.Lock())  // Resource has been locked
		{
			dol.assign(ow->da.dol.begin(),ow->da.dol.end());
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}
		ShowDOL();
		return 0;
	};


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (ListCtrlA::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		SetStyle();

		BuildList();

		return 0;
	};

	BEGIN_MESSAGE_MAP_HPP(COutputListA, ListCtrlA)
		ON_WM_CREATE()
		ON_MESSAGE(MESSAGE_SHOW_DOL, &COutputListA::OnMessageShowDol)
		END_MESSAGE_MAP_HPP()
};

