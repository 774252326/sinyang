#pragma once

#include "ParaDlg.h"

// ParaButton

class ParaButton : public CMFCButton
{
	DECLARE_DYNAMIC(ParaButton)

public:
	ParaButton();
	virtual ~ParaButton();

	ParaDlg *ppd;

protected:
	DECLARE_MESSAGE_MAP()
public:
	void Click(void);
	afx_msg void OnBnClicked();
};


