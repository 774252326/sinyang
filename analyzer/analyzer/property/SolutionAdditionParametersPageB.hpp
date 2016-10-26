#pragma once
#include "../struct\SAPara.hpp"
#include "../list\ListCtrlSAP.hpp"
#include "../analyzerDoc.h"
#include "../Resource.h"
#include "afxdialogex.h"

#include "../messagemaphpp.h"

#include "SolutionAdditionParametersPageA.hpp"

// SolutionAdditionParametersPageB dialog
///
/// \brief The SolutionAdditionParametersPageB class
///加液参数属性页
/// 用于非模态属性表对话框
class SolutionAdditionParametersPageB : public SolutionAdditionParametersPageA
{

public:
    CanalyzerDoc *pDoc;
public:
    SolutionAdditionParametersPageB()
        : SolutionAdditionParametersPageA()
    {

    };
    virtual ~SolutionAdditionParametersPageB()
    {
        //delete [] m_psp.pszTitle;
    };

	void SetList()
	{
		SolutionAdditionParametersPageA::SetList();
    };

    void SetData(CanalyzerDoc *pD)
    {
        pDoc=pD;
        if(pDoc->bChangeSAP){
            para=pDoc->p3todo;
        }
        else{
            para.saplist.assign(pDoc->da.p3.saplist.begin()+pDoc->da.nextSAPIndex,pDoc->da.p3.saplist.end());
        }
    };

protected:	
    virtual BOOL OnApply()
    {
        // TODO: Add your specialized code here and/or call the base class

        pDoc->SetSAPtodo(para);

        return CPropertyPage::OnApply();
    };
    virtual void OnCancel()
    {
        // TODO: Add your specialized code here and/or call the base class
        CPropertyPage::OnCancel();
    };

    virtual void DoDataExchange(CDataExchange* pDX)
    {
        CPropertyPage::DoDataExchange(pDX);
    };    // DDX/DDV support



};
