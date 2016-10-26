
// v2Doc.h : interface of the Cv2Doc class
//


#pragma once

#include <vector>

class Cv2Doc : public CDocument
{
protected: // create from serialization only
	Cv2Doc();
	DECLARE_DYNCREATE(Cv2Doc)

// Attributes
public:

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
	virtual ~Cv2Doc();
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
	std::vector<CRect> rl;

	CPoint & SelectPoint(int lri);
private:
	int idx;
public:
//	afx_msg void OnEditOpen1();
	CString imgfp0;
	CString imgfp1;
//	afx_msg void OnEditAdd();
	int AddRect(const CRect & rect);
	int DelRect(int di);
	int SetSelectIndex(int i);
	afx_msg void OnEditHetu();
};
