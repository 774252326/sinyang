
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
//#include "d:\project1\analyzer\analyzer\struct\sapara.hpp"

#include "PropertySheetA1ML.h"

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
	//sapitemA &outitem,
	size_t &currentSAPIndex,
	size_t &nextSAPIndex,
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

//protected:
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
	afx_msg void OnBnClickedButton1();
	void ChangeSAP(void);
	SAPara p3done;
	afx_msg void OnControlsChangesap();

	PropertySheetA1ML *psheetml;

	int pdfd(CString filepath,
		bool b1,
		bool b2,
		bool b3,
		bool b4,
		bool b5,
		bool b6,
		bool b7);
	afx_msg void OnAnalysisReport();
};
