
// analyzerDoc.h : interface of the CanalyzerDoc class
//
#pragma once
#include "struct\ANPara.hpp"
#include "struct\SAPara.hpp"
#include "struct\VPara.hpp"
#include "struct1\RawData.hpp"
#include "struct\DataOutA.hpp"
#include <vector>
#include "afxmt.h"

class CanalyzerDoc : public CDocument
{
protected: // create from serialization only
	//CanalyzerDoc();
	DECLARE_DYNCREATE(CanalyzerDoc)

// Attributes
public:
	ANPara p1;
	CVPara p2;
	SAPara p3;
	RawData raw;

	std::vector<DataOutA> dol;

// Operations
public:
	CanalyzerDoc();
	void operator=(const CanalyzerDoc &src);
	CanalyzerDoc(const CanalyzerDoc &src){ operator=(src); };


	UINT ComputeStateData(
	sapitemA &outitem,
	BYTE &outstep,
	double &VtoAdd
	);

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
	afx_msg void OnAnalysisMethodsetup();
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS


public:
	void Show(void);
	CCriticalSection m_CritSection;
};
