#pragma once
#include "afxext.h"
class CSplitterWndA :
	public CSplitterWnd
{

	DECLARE_DYNAMIC(CSplitterWndA)

public:
	CSplitterWndA(void);
	/*virtual*/ ~CSplitterWndA(void);

protected:

	DECLARE_MESSAGE_MAP()

};

