#pragma once

// DataOut command target

class DataOut : public CObject
{
public:
	DataOut();
	virtual ~DataOut();
	void operator=(const DataOut &src);

	DataOut(const DataOut &src){ operator=(src); };

	CString stepName;
	double addVol;
	double totalVol;
	double Q;
	double nQ;
	bool Use;
	virtual void Serialize(CArchive& ar);
};


