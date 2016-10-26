#pragma once
#include "../ObjectF.hpp"
// Value command target

class Value : public ObjectF
{
public:
	int LogoId;
	double raw;
	double correction;
public:
	Value()	
		: LogoId(IDB_BITMAP13)
		, raw(0)
		, correction(1)
	{
	};
	virtual ~Value(){};
	virtual void Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{	// storing code
			ar<<LogoId
				<<raw
				<<correction;
		}
		else
		{	// loading code
			ar>>LogoId
				>>raw
				>>correction;
		}
	};

	void operator=(const Value &src)
	{
		LogoId=src.LogoId;
		raw=src.raw;
		correction=src.correction;
	};

	Value(const Value &src){ operator=(src);};

	double Output(void) const{return raw*correction;};

};
