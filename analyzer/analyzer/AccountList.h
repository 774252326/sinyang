#pragma once
#include <vector>
#include "UserAccount.h"
// AccountList command target

class AccountList : public CObject
{
public:
	AccountList();
	virtual ~AccountList();
	std::vector<UserAccount> ual;
	virtual void Serialize(CArchive& ar);
	int CheckUserAccount(CString username, CString password);
	void operator=(const AccountList &src);
	AccountList(const AccountList &src){ operator=(src); };
};


