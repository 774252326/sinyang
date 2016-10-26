#pragma once

// RawData command target

#include <vector>

class RawData : public CObject
{
public:
	RawData();
	virtual ~RawData();
	virtual void Serialize(CArchive& ar);

	void operator=(const RawData &src);
	RawData(const RawData &src){ operator=(src); };

	std::vector<double> xll;
	std::vector<double> yll;
	std::vector<DWORD> ll;

	void GetDatai(size_t index, std::vector<double> & x, std::vector<double> & y) const;
	void Clear(void);

	bool CheckData(void);

	bool AddNew(const std::vector<double> &x, const std::vector<double> &y);
	bool AddFollow(const std::vector<double> &x, const std::vector<double> &y);
	bool AppendData(const RawData & pda);
	
};


