//====================================
// brief: Triangle类，三角形的表示类
// author:sunxvming@163.com
// date:  2019-11-15
//====================================

#include "Triangle.h"
#include "Polygon.h"

Triangle::Triangle(int p1, int p2, int p3) : p1(p1), p2(p2), p3(p3), edges{-1, -1, -1}, isPath(false)
{
}

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) > (b) ? (b) : (a))
void Triangle::GenExtData(navmesh::Polygon *p)
{
	Point pt1 = p->GetPoint(p1);
	Point pt2 = p->GetPoint(p2);
	Point pt3 = p->GetPoint(p3);

	icenter.x = (pt1.x + pt2.x + pt3.x) / 3;
	icenter.y = (pt1.y + pt2.y + pt3.y) / 3;

	double maxx = max(pt1.x, pt2.x);
	double maxy = max(pt1.y, pt2.y);
	double minx = min(pt1.x, pt2.x);
	double miny = min(pt1.y, pt2.y);

	if (pt3.x > maxx)
		maxx = pt3.x;
	if (pt3.y > maxy)
		maxy = pt3.y;
	if (pt3.x < minx)
		minx = pt3.x;
	if (pt3.y < miny)
		miny = pt3.y;

	lt.x = minx, lt.y = miny;
	rb.x = maxx, rb.y = maxy;
}

int Triangle::Contain(navmesh::Polygon *p, Point pt)
{
	double x = pt.x, y = pt.y;
	// 快速排查
	if (x < lt.x)
		return 0;
	if (x > rb.x)
		return 0;
	if (y < lt.y)
		return 0;
	if (y > rb.y)
		return 0;

	Point pt1 = p->GetPoint(p1);
	Point pt2 = p->GetPoint(p2);
	Point pt3 = p->GetPoint(p3);

	Point v0 = pt3 - pt1;
	Point v1 = pt2 - pt1;
	Point v2 = pt - pt1;

	// P = A +  u * (C – A) + v * (B - A)  满足 u + v <=1 u >= 0 v >= 0
	double dot00 = v0.Dot(v0);
	double dot01 = v0.Dot(v1);
	double dot02 = v0.Dot(v2);
	double dot11 = v1.Dot(v1);
	double dot12 = v1.Dot(v2);
	double inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);
	double u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
	if (u < 0 || u > 1) // if u out of range, return directly
	{
		return 0;
	}

	double v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
	if (v < 0 || v > 1) // if v out of range, return directly
	{
		return 0;
	}

	return u + v <= 1 ? 1 : 0;
}

Triangle::~Triangle()
{
}
