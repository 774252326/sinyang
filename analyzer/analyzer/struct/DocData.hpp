#pragma once

#include "../struct\ANParaEx.hpp"
#include "../struct\SAPara.hpp"
#include "../struct\VPara.hpp"
#include "../struct1\RawData.hpp"

// DocData command target

class DocData : public CObject
{

public:
	ANParaEx p1;
	CVPara p2;
	SAPara p3;
	RawData raw;

public:
	DocData(){};
	virtual ~DocData(){};

	void operator=(const DocData &src)
	{
		p1=src.p1;
		p2=src.p2;
		p3=src.p3;
		raw=src.raw;	
	};

	DocData(const DocData &src){ operator=(src); };

	virtual void Serialize(CArchive& ar)
	{
		p1.Serialize(ar);
		p2.Serialize(ar);
		p3.Serialize(ar);
		raw.Serialize(ar);

		if (ar.IsStoring())
		{	// storing code
		}
		else
		{	// loading code
		}
	};







};


