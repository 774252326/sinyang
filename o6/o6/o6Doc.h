// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// o6Doc.h : interface of the Co6Doc class
//


#pragma once
#include <vector>
#include <fstream>


class Co6Doc : public CDocument
{
protected: // create from serialization only
	Co6Doc();
	DECLARE_DYNCREATE(Co6Doc)

	// Attributes
public:

	// Operations
public:
	template <typename T>
	bool readf2(char *fn, std::vector< std::vector<T> > &mx, const size_t nCol)
	{
		std::ifstream ifs(fn);

		if(ifs.bad()){
			return false;
		}

		std::vector<T> vx;
		while(!ifs.eof()){
			T tmp;
			ifs>>tmp;
			vx.push_back(tmp);
			if(vx.size()>=nCol){
				mx.push_back(vx);
				vx.clear();
			}
		}

		ifs.close();
		return true;
	};



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
	virtual ~Co6Doc();
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
};
