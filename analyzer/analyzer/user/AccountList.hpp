#pragma once
#include <vector>
#include "UserAccount.hpp"
// AccountList command target

///
/// \brief The AccountList class
///用户帐户列表
///
class AccountList : public ObjectF
{
public:
	std::vector<UserAccount> ual;

	AccountList(){};

	virtual ~AccountList(){};

	virtual void Serialize(CArchive& ar){
		if (ar.IsStoring())
		{	// storing code
			ar<<ual.size();
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
	};

    ///
    /// \brief CheckUserAccount
    /// 检查用户名密码是否配对
    /// \param username
    /// 用户名
    /// \param password
    /// 密码
    /// \return
    /// 若用户名密码正确配对
    /// 返回此用户名在列表中的序号
    /// 若用户名正确密码错误
    /// 返回-1
    /// 若用户名错误
    /// 返回-2
    ///
	int CheckUserAccount(CString username, CString password){
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
	};

	void operator=(const AccountList &src){
		ual.assign(src.ual.begin(),src.ual.end());
	};

	AccountList(const AccountList &src){ operator=(src); };

    ///
    /// \brief IsAdmin
    /// 检查第i个用户是不是管理员
    /// \param i
    /// \return
    ///
	bool IsAdmin(int i){
		if(i>=0 && ((size_t)i)<ual.size() && ual[i].au==UserAccount::admin)
			return true;
		return false;
	};

};


