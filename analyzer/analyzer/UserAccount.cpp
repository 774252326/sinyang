// UserAccount.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "UserAccount.h"


// UserAccount

UserAccount::UserAccount()
	: userName(_T(""))
	, passWord(_T(""))
	, remark(_T(""))
	, au(guest)
{
}

UserAccount::~UserAccount()
{
}


// UserAccount member functions


void UserAccount::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<userName
			<<passWord
			<<remark;
			//<<au;
		switch(au){
		case admin:
			ar<<(BYTE)0;
			break;
		case user:
			ar<<(BYTE)1;
			break;
		case guest:
			ar<<(BYTE)2;
			break;
		default:
			ar<<(BYTE)3;
			break;
		}

	}
	else
	{	// loading code
		ar>>userName
			>>passWord
			>>remark;
		BYTE aut;
		ar>>aut;
		switch(aut){
		case 0:
			au=admin;
			break;
		case 1:
			au=user;
			break;
		case 2:
			au=guest;
			break;
		default:
			break;
		}

	}
}


void UserAccount::operator=(const UserAccount &src)
{
	userName=src.userName;
	passWord=src.passWord;
	remark=src.remark;
	au=src.au;
}