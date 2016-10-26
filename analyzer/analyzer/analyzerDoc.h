
// analyzerDoc.h : interface of the CanalyzerDoc class
//
#include "ANPara.h"
#include "DataOut.h"
#include "PlotData.h"
#include "SAPara.h"
#include "VPara.h"
#include <vector>

#pragma once


class CanalyzerDoc : public CDocument
{
protected: // create from serialization only
	CanalyzerDoc();
	DECLARE_DYNCREATE(CanalyzerDoc)

// Attributes
public:

	void operator=(const CanalyzerDoc &src);
	CanalyzerDoc(const CanalyzerDoc &src){ operator=(src); };
	CanalyzerDoc(bool flg);

	ANPara p1;
	CVPara p2;
	SAPara p3;
	std::vector<DataOut> dol;
	std::vector<PlotData> lp;
	std::vector<PlotData> rp;
	CString resultStr;
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CanalyzerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnAnalysisMethodsetup();
//	afx_msg void OnAnalysisStartanalysis();
//	CWinThread *ppa;

	bool bUpdateView;
	PlotData * GetPD(int lr, int index);
	int GetNPD(int lr);
	int AddPD(const PlotData & pda, int lr);
	afx_msg void OnAnalysisReport();
	bool SaveImage(const PlotData & pd, CSize sz, CString filepath);
};
