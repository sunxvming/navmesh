//====================================
// brief: Circle类，表示圆
// author:sunxvming@163.com
// date:  2019-11-15
//====================================

#include "Circle.h"

Circle::Circle( Point p1, Point p2, Point p3)
{
	double x1 = p1.x, y1 = p1.y;
	double x2 = p2.x, y2 = p2.y;
	double x3 = p3.x, y3 = p3.y;
	//求外接圆圆心  
	double t1 = x1*x1 + y1*y1;
	double t2 = x2*x2 + y2*y2;
	double t3 = x3*x3 + y3*y3;
	double temp = x1*y2 + x2*y3 + x3*y1 - x1*y3 - x2*y1 - x3*y2;
	double cx = (t2*y3 + t1*y2 + t3*y1 - t2*y1 - t3*y2 - t1*y3) / temp / 2;
	double cy = (t3*x2 + t2*x1 + t1*x3 - t1*x2 - t2*x3 - t3*x1) / temp / 2;
	center.x = cx, center.y = cy;
	r = sqrt((cx -x1)*(cx -x1)+ (cy - y1)*(cy - y1));
}

Point Circle::GetCenter()
{
	return center;
}

double Circle::GetR()
{
	return r;
}


Circle::~Circle()
{
}
