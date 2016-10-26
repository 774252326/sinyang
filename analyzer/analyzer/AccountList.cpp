// AccountList.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "AccountList.h"


// AccountList

AccountList::AccountList()
{
}

AccountList::~AccountList()
{
}

void AccountList::operator=(const AccountList &src)
{
	ual.assign(src.ual.begin(),src.ual.end());
}

// AccountList member functions


void AccountList::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<ual.size();
		//for(size_t i=0;i<ual.size();i++){
		//	ual[i].Serialize(ar);
		//}
	}
	else
	{	// loading code
		size_t n;
		ar>>n;
		ual.assign(n,UserAccount());
	}

	for(size_t i=0;i<ual.size();i++){
		ual[i].Serialize(ar);
	}


}


int AccountList::CheckUserAccount(CString username, CString password)
{
	bool bHitUser=false;
	for(size_t i=0;i<ual.size();i++){
		if(username==ual[i].userName){
			if( password==ual[i].passWord ){
				return i;
			}
			bHitUser=true;
		}
	}
	
	return bHitUser?-1:-2;
}

