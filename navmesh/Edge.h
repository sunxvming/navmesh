//====================================
// brief: Edge类，表示经过剖分后的线段，一条线段可能包含一个或两个三角，包含两个点
// author:sunxvming@163.com
// date:  2019-11-15
//====================================

#ifndef NAVMESH_EDGE_H
#define NAVMESH_EDGE_H

namespace navmesh{
    class Polygon;
}

class Edge
{
public:
	int triangles[2];
	int points[2];
	Edge(int t1, int t2, int p1, int p2);
	int IsRestrain(navmesh::Polygon* p);
	virtual ~Edge();
};

#endif // NAVMESH_EDGE_H
