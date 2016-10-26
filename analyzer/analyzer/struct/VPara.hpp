#pragma once
#include "../ObjectF.hpp"

#include "../funT1\ComputeQT.h"

// CVPara command target

//CVParameters
///
/// \brief The CVPara class
///伏安参数
class CVPara : public ObjectF
{
public:
    ///
    /// \brief lowelimit
    ///最低电势
    double lowelimit;
    ///
    /// \brief highelimit
    ///最高电势
    double highelimit;
    ///
    /// \brief scanrate
    ///扫描速度
    double scanrate;
    ///
    /// \brief combochoice
    ///积分上限下拉框的选中项
    int combochoice;
    ///
    /// \brief noofcycles
    ///循环次数
    int noofcycles;
    ///
    /// \brief variationtolerance
    ///容限
    double variationtolerance;
    ///
    /// \brief rotationrate
    ///旋转速度
    double rotationrate;
    ///
    /// \brief endintegratione
    ///积分上限
    double endintegratione;
    ///
    /// \brief startintegratione
    ///积分下限
    double startintegratione;
    ///
    /// \brief samplefrequecy
    ///采样频率
    double samplefrequecy;
    ///
    /// \brief quiettime
    ///读数据线程启动到有数据读入之间的等待时间
    double quiettime;

    CVPara()
        : lowelimit(-0.23)
        , highelimit(1.57)
        , scanrate(0.1)
        , combochoice(0)
        , noofcycles(4)
        , variationtolerance(0)
        , rotationrate(2000)
        , endintegratione(1)
        , startintegratione(0)
        , samplefrequecy(100)
        , quiettime(1)
    {
    };

    virtual ~CVPara(){};

    void operator=(const CVPara &src){
        lowelimit=src.lowelimit;
        highelimit=src.highelimit;
        scanrate=src.scanrate;
        combochoice=src.combochoice;
        noofcycles=src.noofcycles;
        variationtolerance=src.variationtolerance;
        rotationrate=src.rotationrate;
        endintegratione=src.endintegratione;
        startintegratione=src.startintegratione;
        samplefrequecy=src.samplefrequecy;
        quiettime=src.quiettime;
    };

    CVPara(const CVPara &src){ operator=(src); };

    virtual void Serialize(CArchive& ar){
        if (ar.IsStoring())
        {	// storing code
            ar<<lowelimit
             <<highelimit
            <<scanrate
            <<combochoice
            <<noofcycles
            <<variationtolerance
            <<rotationrate
            <<endintegratione
            <<startintegratione;
        }
        else
        {	// loading code
            ar>>lowelimit
             >>highelimit
            >>scanrate
            >>combochoice
            >>noofcycles
            >>variationtolerance
            >>rotationrate
            >>endintegratione
            >>startintegratione;
        }
    };


    ///
    /// \brief EInterval
    /// 按照参数设定计算出相邻两数据点的电势差
    /// \return
    ///
    double EInterval() const
    {
        return scanrate/samplefrequecy;
    };

    ///
    /// \brief ComputeQList
    /// 计算电量
    /// \param x
    /// \param y
    /// \param q
    /// \return
    ///

    UINT ComputeQList(const std::vector<double> &x,
                      const std::vector<double> &y,
                      std::vector<double> &q) const
    {
        if(combochoice==0){
            double gap=0.9;
            return ::ComputeQList(x,y,q,endintegratione,scanrate,lowelimit*gap,highelimit*gap);
        }

        return ::ComputeQListA(x,y,q,endintegratione,startintegratione,scanrate);

    };

};


