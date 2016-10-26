#pragma once
#include "../ObjectF.hpp"
#include "../resource.h"

// sapitemA command target
///
/// \brief The sapitemA class
///加液参数
class sapitemA : public ObjectF
{
public:
    ///
    /// \brief Sconc
    ///抑制剂浓度
	double Sconc;
    ///
    /// \brief Aconc
    ///加速剂浓度
	double Aconc;
    ///
    /// \brief Lconc
    ///整平剂浓度
	double Lconc;

    ///
    /// \brief addType
    ///加液类型
    ///
    /// 0-按体积加一次
    /// 1-按体积递加
    /// 2-按浓度加一次
    /// 3-按浓度递加
    /// 4-加电镀基液
	int addType;
    ///
    /// \brief volconc
    ///体积或浓度数值
	double volconc;
    ///
    /// \brief endRatio
    ///结束点
	double endRatio;
	//bool bSample;


	sapitemA()
		: Sconc(0)
		, Aconc(0)
		, Lconc(0)
		, addType(0)
		, volconc(0)
		, endRatio(0)
		//, bSample(true)
	{
	};
	virtual ~sapitemA(){};

	void operator=(const sapitemA &src){
		Sconc=src.Sconc;
		Aconc=src.Aconc;
		Lconc=src.Lconc;
		addType=src.addType;
		volconc=src.volconc;
		endRatio=src.endRatio;
		//bSample=src.bSample;
	};
	sapitemA(const sapitemA &src){ operator=(src); };


	virtual void Serialize(CArchive& ar){
		if (ar.IsStoring())
		{	// storing code
			ar<<Sconc
				<<Aconc
				<<Lconc
				<<addType
				<<volconc
				<<endRatio;
		}
		else
		{	// loading code
			ar>>Sconc
				>>Aconc
				>>Lconc
				>>addType
				>>volconc
				>>endRatio;
		}
	};

    ///
    /// \brief isUnknownComposition
    /// 判断溶液成分是否不明
    /// \return
    ///
	bool isUnknownComposition(void){
		if( Sconc<=0 && Aconc<=0 && Lconc<=0 )
			return true;
		return false;
	};
    ///
    /// \brief isStepEnd
    /// 判断此加液步骤是否结束
    /// \param currentRatio
    /// 当前比率
    /// \param bLess
    /// 若真
    /// 当前比率小于结束点时结束
    /// 若假
    /// 当前比率大于结束点时结束
    /// \return
    ///
	bool isStepEnd(double currentRatio,bool bLess=true){
		if(addType==1||addType==3){
			if( currentRatio==endRatio || ((currentRatio<endRatio)^bLess) ){
				return false;
			}
		}
		return true;
	};
    ///
    /// \brief isMixedComposition
    /// 判断是否混合溶液
    /// \return
    ///
	bool isMixedComposition(void){
		if( (Sconc>0 && Aconc>0)
			||(Sconc>0 && Lconc>0)
			||(Lconc>0 && Aconc>0)
			){
				return true;
		}
		return false;
	};

    ///
    /// \brief isSingleComposition
    /// 判断是否单成分
    /// \return
    ///
	bool isSingleComposition(void){
		if(isMixedComposition()||isUnknownComposition())
			return false;
		return true;
	};

	bool SetEndRatio(double er){
		if(addType==1||addType==3){
			endRatio=er;
			return true;
		}
		return false;
	};

    ///
    /// \brief StandardStep
    /// 将当前加液步骤标准化
    /// 即转为按体积加一次的格式
    /// \param volume
    /// 此次加液体积
    /// \return
    ///
	sapitemA StandardStep(double volume){
		sapitemA sia(*this);
		if(sia.addType==1||sia.addType==3){
			sia.volconc=volume;
		}
		if(sia.addType!=4){
			sia.addType=0;
		}
		return sia;
	};

    ///
    /// \brief SolutionDescription
    /// 返回待加溶液名称
    /// \return
    ///
	CString SolutionDescription(){
		CString str=L"";
		if(addType==4){
			str.LoadStringW(IDS_STRING_VIRGIN_MAKEUP);
		}
		else{
			if(isUnknownComposition()){
				str.LoadStringW(IDS_STRING_SAMPLE);
			}
			else{
				CString strt=L"";
				if(Sconc>0){
					strt.LoadStringW(IDS_STRING_S);
					str+=strt;
				}
				
								if(Aconc>0){
					strt.LoadStringW(IDS_STRING_A);
					str+=strt;
				}
				
								if(Lconc>0){
					strt.LoadStringW(IDS_STRING_L);
					str+=strt;
				}
				
				if(isMixedComposition()){
					strt.LoadStringW(IDS_STRING_MIX);
					str+=strt;
				}
				
			}
		}
		return str;
	};

};


