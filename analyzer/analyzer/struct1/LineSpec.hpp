#pragma once

// linespec command target
///
/// \brief The LineSpec class
///曲线的画图属性
///
class LineSpec : public CObject
{
public:
    ///
    /// \brief colour
    ///颜色
	COLORREF colour;
    ///
    /// \brief name
    ///曲线名
	CString name;
    ///
    /// \brief dotSize
    ///数据点大小
	int dotSize;
    ///
    /// \brief lineType
    ///线型
	int lineType;//0~5
    ///
    /// \brief smoothLine
    ///连线方法
	int smoothLine;//0~2
    ///
    /// \brief traceLast
    ///凸出显示最后一点
	bool traceLast;
	//protected:
    ///
    /// \brief lineWidth
    ///线宽
	int lineWidth;
    ///
    /// \brief lastWidth
    ///最后一点的点大小
	int lastWidth;

public:
	LineSpec()
		: name(_T(""))
		, dotSize(0)
		, lineType(0)
		, smoothLine(0)
		, traceLast(false)
		, colour(0)
		, lineWidth(1)
		, lastWidth(4)
	{};
	virtual ~LineSpec(){};

	void operator=(const LineSpec &src)
	{
		colour=src.colour;
		name=src.name;
		dotSize=src.dotSize;
		lineType=src.lineType;
		smoothLine=src.smoothLine;
		traceLast=src.traceLast;
		lineWidth=src.lineWidth;
		lastWidth=src.lastWidth;
	};

	LineSpec(const LineSpec &src){ operator=(src); };

	virtual void Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{	// storing code
			ar<<colour
				<<name
				<<dotSize
				<<lineType
				<<smoothLine
				<<traceLast
				<<lineWidth
				<< lastWidth;
		}
		else
		{	// loading code
			ar>>colour
				>>name
				>>dotSize
				>>lineType
				>>smoothLine
				>>traceLast
				>>lineWidth
				>> lastWidth;
		}
	};

};


