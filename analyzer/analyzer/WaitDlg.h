#pragma once


// WaitDlg dialog




//#include "filefunc.h"

enum ProcessState{
	stop,
	running,
	pause
};


class WaitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WaitDlg)

public:
	WaitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~WaitDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	ProcessState *pst;
	virtual void OnOK();
};
