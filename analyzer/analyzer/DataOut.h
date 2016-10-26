#pragma once

// DataOut command target

class DataOut : public CObject
{
public:
	DataOut();
	virtual ~DataOut();

	DataOut(const DataOut &src);
	void operator=(const DataOut &src);

	CString stepName;
	double addVol;
	double totalVol;
	double Q;
	double nQ;
	bool Use;
	virtual void Serialize(CArchive& ar);
};


