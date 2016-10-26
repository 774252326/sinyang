#pragma once


enum authority{
	admin,
	user,
	guest
};

// UserAccount command target

class UserAccount : public CObject
{

public:
	UserAccount();
	virtual ~UserAccount();
	virtual void Serialize(CArchive& ar);
	CString userName;
	CString passWord;
	CString remark;
	authority au;
	void operator=(const UserAccount &src);
	UserAccount(const UserAccount &src){ operator=(src); };


};


