#pragma once
#include <vector>
#include "UserAccount.hpp"
// AccountList command target

class AccountList : public CObject
{
public:
	std::vector<UserAccount> ual;

	AccountList(){};

	virtual ~AccountList(){};

	virtual void Serialize(CArchive& ar){
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
	};

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

	bool IsAdmin(int i){
		if(i>=0 && i<ual.size() && ual[i].au==UserAccount::admin)
			return true;
		return false;
	};

};


