#pragma once

#include "ParaList.h"
#include "struct1\RawData.hpp"
#include "struct\DataOutA.hpp"
#include "struct\ANPara.hpp"


// ComputeDlg dialog

class ComputeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ComputeDlg)

public:
	ComputeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ComputeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_COMPUTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ParaList m_list;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	static bool SetParameter(const std::vector<DataOutA> & dol, const ANPara & p1, std::vector<RawData> & rdl, std::vector<Value> & vl);
	static bool RecordDT(const std::vector<Value> & vl, double & z);
	static bool AnalysisDT(const std::vector<Value> & vl, double & SPc);
	static bool RecordDTGetVL(std::vector<Value> &vl, const std::vector<DataOutA> & dolast, double evaR, const RawData & rd, size_t lineIndex=0);
	static bool AnalysisDTGetVL(std::vector<Value> & vl, const std::vector<DataOutA> & dolast, double evaR, double z, const RawData & rd, size_t lineIndex=0);

	static bool AnalysisDTGetVL(std::vector<Value> & vl, const std::vector<DataOutA> & dolast, double evaR, CString fp, const RawData & rd, size_t lineIndex=0);


	virtual BOOL OnInitDialog();

	CBitmap ddd;
};
