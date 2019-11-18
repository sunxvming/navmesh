//====================================
// brief: Circle¿‡£¨±Ì æ‘≤
// author:sunxvming@163.com
// date:  2019-11-15
//====================================

#ifndef NAVMESH_CIRCLE_H
#define NAVMESH_CIRCLE_H

#include "Point.h"
#include <math.h>
class Circle
{
private:
	Point center;
	double r;
public:
	Circle(Point p1, Point p2, Point p3);
	Point GetCenter();
	double GetR();
	virtual ~Circle();
};

#endif // NAVMESH_CIRCLE_H
