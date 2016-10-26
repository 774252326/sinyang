// ParaButton.cpp : implementation file
//

#include "stdafx.h"
#include "z27.h"
#include "ParaButton.h"


// ParaButton

IMPLEMENT_DYNAMIC(ParaButton, CMFCButton)

ParaButton::ParaButton()
: ppd(NULL)
{

}

ParaButton::~ParaButton()
{
}


BEGIN_MESSAGE_MAP(ParaButton, CMFCButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &ParaButton::OnBnClicked)
END_MESSAGE_MAP()



// ParaButton message handlers




void ParaButton::Click(void)
{
	
	if(ppd==NULL){
		ppd=new ParaDlg();
		ppd->Create(ParaDlg::IDD);
		ppd->ShowWindow(SW_SHOW);
	}
	else{		
		ppd->DestroyWindow();
		delete ppd;
		ppd=NULL;
	}

}


void ParaButton::OnBnClicked()
{
	// TODO: Add your control notification handler code here
	Click();
}
