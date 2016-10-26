#pragma once

#include "sapitemA.hpp"
#include "DataOutA.hpp"
#include <vector>
// SAPara command target
///
/// \brief The SAPara class
///加液步骤
class SAPara : public ObjectF
{
public:
    ///
    /// \brief saplist
    ///加液步骤列表
	std::vector<sapitemA> saplist;


	SAPara(){};
	virtual ~SAPara(){};
	void operator=(const SAPara &src){
		//vmsvol=src.vmsvol;
		saplist.assign(src.saplist.begin(),src.saplist.end());
	};
	SAPara(const SAPara &src){ operator=(src); };
	

	virtual void Serialize(CArchive& ar){
		if (ar.IsStoring())
		{	// storing code
			//ar<<vmsvol
			ar<<saplist.size();

			for(size_t i=0;i<saplist.size();i++){
				saplist[i].Serialize(ar);
			}
		}
		else
		{	// loading code
			size_t nrow;
			//ar>>vmsvol
			ar>>nrow;

			saplist.assign(nrow,sapitemA());

			for(size_t i=0;i<nrow;i++){
				saplist[i].Serialize(ar);
			}
		}
	};

    ///
    /// \brief ConvertToStandrad
    /// 转换成标准加液步骤
    /// \param dol
    /// 溶液状态的记录
    /// \param sapStd
    /// 标准加液步骤
    /// \return
    ///
	bool ConvertToStandrad(const std::vector<DataOutA> &dol, SAPara &sapStd)
	{
		SAPara stdp;

		size_t j=0;
		for(size_t i=0;i<dol.size();i++){
			switch(saplist[j].addType){
			case 0:
				stdp.saplist.push_back(saplist[j]);
				j++;
				break;
			case 1:
				stdp.saplist.push_back(saplist[j]);
				stdp.saplist.back().addType=0;
				//if(saplist[j].isStepEnd(dol[i].
				break;
			case 2:
				stdp.saplist.push_back(saplist[j]);
				stdp.saplist.back().addType=0;
				stdp.saplist.back().volconc=dol[i].addVolume;
				j++;
				break;
			case 3:
				stdp.saplist.push_back(saplist[j]);
				stdp.saplist.back().addType=0;
				stdp.saplist.back().volconc=dol[i].addVolume;
				break;
			case 4:
				stdp.saplist.push_back(saplist[j]);
				j++;
				break;
			default:
				return false;
				//break;
			}
		}

		sapStd=stdp;

		return true;
		
	};

	void AppendData(const SAPara & pda)
	{		
		saplist.resize(saplist.size()+pda.saplist.size());
		std::copy_backward(pda.saplist.begin(),pda.saplist.end(),saplist.end());
	};

};


