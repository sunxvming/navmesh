//====================================
// brief: Edge类，表示经过剖分后的线段，一条线段可能包含一个或两个三角，包含两个点
// author:sunxvming@163.com
// date:  2019-11-15
//====================================

#include "Edge.h"
#include "Polygon.h"


Edge::Edge(int t1, int t2, int p1, int p2)
{
	triangles[0] = t1;
	triangles[1] = t2;
	points[0] = p1;
	points[1] = p2;
}

int Edge::IsRestrain(Polygon* p)
{
	int p1 = points[0];
	int p2 = points[1];
	int dp = p2 - p1;
	if (dp == 1 || dp == -1) return 1;
	if (p1 && p2) return 0;
	return p1 + p2 == p->points.size() - 1;
}

Edge::~Edge()
{
}
