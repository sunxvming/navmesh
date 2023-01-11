//====================================
// brief: Triangle类，三角形的表示类
// author:sunxvming@163.com
// date:  2019-11-15
//====================================

#ifndef NAVMESH_TRIANGLE_H
#define NAVMESH_TRIANGLE_H

#include "Point.h"

namespace navmesh{
    class Polygon;
}

class Triangle
{
public:
	int p1;
	int p2;
	int p3;
	int edges[3];
	Point icenter;//重心
	Point lt;
	Point rb;
public:
	Triangle(int p1, int p2, int p3);
	void GenExtData(navmesh::Polygon* p);
	int Contain(navmesh::Polygon* p, Point pt);
	virtual ~Triangle();
};

#endif // NAVMESH_TRIANGLE_H