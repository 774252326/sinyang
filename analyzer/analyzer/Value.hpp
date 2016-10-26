#pragma once

// Value command target

class Value : public CObject
{
public:
	int LogoId;
	double raw;
	double correction;
public:
	Value()	
		: LogoId(0)
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
