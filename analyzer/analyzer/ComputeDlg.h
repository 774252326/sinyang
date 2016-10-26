#pragma once

#include "ParaList.h"
#include "struct1\RawData.hpp"
#include "struct\DataOutA.hpp"
#include "struct\ANPara.hpp"
#include "struct1\LineSeg.hpp"
#include "analyzerDoc.h"

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
	



	virtual BOOL OnInitDialog();

	CBitmap ddd;
	CMFCButton m_btn1;
	CMFCButton m_btn2;
	afx_msg void OnBnClickedMfcbutton1();


	static bool RecordDTGetVL(std::vector<Value> &vl, 
		const std::vector<DataOutA> & dolast, 
		double evaR, 
		const RawData & rd, 
		size_t lineIndex=0);
		static bool RecordDT(const std::vector<Value> & vl, double & z);

	static bool AnalysisDTGetVL(std::vector<Value> & vl, 
		const std::vector<DataOutA> & dolast, 
		double evaR, 
		double z, 
		const RawData & rd,
		size_t lineIndex=0);
	static bool AnalysisDTGetVL(std::vector<Value> & vl, 
		const std::vector<DataOutA> & dolast, 
		double evaR, 
		CString fp, 
		const RawData & rd, 
		size_t lineIndex=0);	

static bool AnalysisDT(const std::vector<Value> & vl, double & SPc);

static bool RecordLAT(const RawData & rd, double & ITc, double slopeThreshold=-.05, size_t index=0);


	static bool AnalysisLATGetVL(std::vector<Value> & vl, 
		const std::vector<DataOutA> & dolast, 
		const RawData & rd, 
		size_t lineIndex=0,
		int nFront=1,
		int nBack=0);

	static bool AnalysisLAT(const std::vector<Value> & vl, double & SPc);


	static bool RecordRC(const std::vector<DataOutA> & dolast, double evaR, const RawData & rd, double & Lc, size_t index=0);


	static bool AnalysisRCGetVL(std::vector<Value> & vl, 
		const std::vector<DataOutA> & dolast, 
		double Lc);

	static bool AnalysisRCGetVL(std::vector<Value> & vl, 
		const std::vector<DataOutA> & dolast, 
		//double evaR, 
		CString fp, 
		const RawData & rd, 
		size_t lineIndex=0);


	static bool AnalysisRC(const std::vector<Value> & vl, double & SPc);


	static bool RecordSAR(const std::vector<DataOutA> & dolast, 
		double evaR, 
		const RawData & rd, 
		RawData &SAraw, 
		LineSeg &lis);

	static bool RecordSARGetStd(const std::vector<DataOutA> & dolast, const RawData & rd, RawData &SnQstd);

	static bool AnalysisSARGetVL(std::vector<Value> & vl, 
		const std::vector<DataOutA> & dolast, 
		double evaR, 
		CString fp, 
		const RawData & rd, 
		size_t lineIndexSP=0,
		size_t lineIndexA=1);

	static bool AnalysisSAR(const std::vector<Value> & vl,
		const RawData &SnQstd, 
		double & Sc, double & Ac, size_t nir=3, size_t lineIndex=0);


	static bool RecordPAL(double evaR, const RawData & rd, double & Lc, size_t index=0);

	static bool AnalysisPALGetVL(std::vector<Value> & vl, 
		const std::vector<DataOutA> & dolast, 
		//double evaR, 
		CString fp);

	static bool AnalysisPAL(const std::vector<Value> & vl, double & SPc);


	static bool RecordLRT(const RawData & rd, LineSeg &lis, size_t index=0);

	static bool AnalysisLRTGetVL(std::vector<Value> & vl, 
		const std::vector<DataOutA> & dolast, 
		CString fp,
		const RawData & rd,
		int nIgnore=3,
		size_t lineIndex=0);

	static bool AnalysisLRT(const std::vector<Value> & vl, double & SPc);


	static bool GetVL(std::vector<Value> & vl, 
		const std::vector<DataOutA> & dolast, 
		const RawData & rd,
		const ANPara & p1);

	static bool GetResult(CanalyzerDoc *pDoc, 
		const std::vector<Value> &vl, 
		std::vector<CString> &name, 
		std::vector<CString> &value);
};
