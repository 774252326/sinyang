
// analyzerDoc.h : interface of the CanalyzerDoc class
//


#pragma once

#include "struct\DocDataEx.hpp"


class CanalyzerDoc : public CDocument
{
protected: // create from serialization only
	CanalyzerDoc();
	DECLARE_DYNCREATE(CanalyzerDoc)

// Attributes
public:
	DocDataEx da;

	SAPara p3todo;
	bool bChangeSAP;
	CCriticalSection m_CritSection;
	CCriticalSection m_CritSectionSAP;

// Operations
public:
	void Show(void);
	public:
	UINT UpdateState(void);
	void SetSAPtodo(const SAPara & ptd);
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
	afx_msg void OnAnalysisCompute();
	afx_msg void OnAnalysisReport();
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	void ClearExpData(void);
};
