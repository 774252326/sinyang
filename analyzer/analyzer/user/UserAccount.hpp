#pragma once

#include "../ObjectF.hpp"


// UserAccount command target

///
/// \brief The UserAccount class
///用户账户属性

class UserAccount : public ObjectF
{
public:

	enum authority{
		admin,
		user,
		guest
	};

    ///
    /// \brief userName
    ///用户名
	CString userName;
    ///
    /// \brief passWord
    ///密码
	CString passWord;
    ///
    /// \brief remark
    ///备注
	CString remark;
    ///
    /// \brief au
    ///权限
	authority au;


public:
	UserAccount()
		: userName(_T("g"))
		, passWord(_T("g"))
		, remark(_T("g"))
		, au(admin)
	{
	};

	virtual ~UserAccount(){};

	virtual void Serialize(CArchive& ar)
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
				TRACE0("read type error");
				break;
			}

		}
	};

	void operator=(const UserAccount &src){
		userName=src.userName;
		passWord=src.passWord;
		remark=src.remark;
		au=src.au;
	};

	UserAccount(const UserAccount &src){ operator=(src); };


};


