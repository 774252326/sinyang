#pragma once

class Point2D
{
public:
	double x[2];
	Point2D(){};
	Point2D(double x0, double y0){x[0]=x0;x[1]=y0;};
	~Point2D(){};
public:
	void operator = (const Point2D &src)
	{
		x[0]=src.x[0];
		x[1]=src.x[1];
	};
	friend Point2D operator -(const Point2D &src1, const Point2D &src2)
	{
		Point2D dif;
		dif.x[0]=src1.x[0]-src2.x[0];
		dif.x[1]=src1.x[1]-src2.x[1];
		return dif;
	};
	friend Point2D operator +(const Point2D &src1, const Point2D &src2)
	{
		Point2D dif;
		dif.x[0]=src1.x[0]+src2.x[0];
		dif.x[1]=src1.x[1]+src2.x[1];
		return dif;
	};
	double operator*(const Point2D &src)
	{
		return x[0]*src.x[0]+x[1]*src.x[1];
	};
	double autoco()
	{
		return operator*(*this);
	};
	static double Dist2(Point2D p1, Point2D p2)
	{
		Point2D df=p1-p2;
		return df.autoco();
	};
	static bool OutCenter(Point2D p1, Point2D p2, Point2D p3, Point2D &pc, double &r2)
	{
		Point2D df1=p1-p2;
		Point2D df2=p1-p3;
		
		double detx=df1.x[0]*df2.x[1]-df1.x[1]*df2.x[0];
		if(detx==0)
			return false;

		Point2D b;
		b.x[0]=p1.autoco()-p2.autoco();
		b.x[1]=p1.autoco()-p3.autoco();

		pc.x[0]=b.x[0]*df2.x[1]-b.x[1]*df1.x[1];
		pc.x[1]=-b.x[0]*df2.x[0]+b.x[1]*df1.x[0];

		pc.x[0]/=2*detx;
		pc.x[1]/=2*detx;

		r2=Dist2(pc,p1);
		return true;
	};
	static bool min1(Point2D p1, Point2D p2)
	{
		return p1.x[1]<p2.x[1];
	};

	static bool min0(Point2D p1, Point2D p2)
	{
		return p1.x[0]<p2.x[0];
	};

	static double rsl(double x1, double x2, double y1, double y2, double x)
	{
		double dx=x2-x1;
		if(dx==0)
			return (y1+y2)/2;
		else
			return (x*(y2-y1)+x2*y1-x1*y2)/dx;
	};

	static Point2D Interp(Point2D p1, Point2D p2, double v1, double v2, double vi)
	{
		Point2D pi;
		pi.x[0]=rsl(v1,v2,p1.x[0],p2.x[0],vi);
		pi.x[1]=rsl(v1,v2,p1.x[1],p2.x[1],vi);
		return pi;
	};
		

	
};
