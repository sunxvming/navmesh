//====================================
// brief: Polygon类，代表寻路多边形的类，主要实现多边形的三角剖分和寻路
// author:sunxvming@163.com
// date:  2019-11-15
//====================================

#ifndef NAVMESH_POLYGON_H
#define NAVMESH_POLYGON_H

#include <vector>
#include <unordered_map>
#include "Point.h"
#include "Triangle.h"
#include "Edge.h"
#include "Circle.h"

using namespace std;

typedef unordered_map<int, int> Hash;
#define MAXPOINT 100000
#define PIndex(p1, p2) (p1 > p2 ? (p2 * MAXPOINT + p1) : (p1 * MAXPOINT + p2))

class Cell
{
public:
	vector<int> points;
	vector<int> edges;
};

class Grid
{
public:
	vector<Cell> cells;
	int gride_len; // grid length
	double minx;
	double miny;
	double maxx;
	double maxy;
	int xnum;
	int ynum;
};

class Line
{
public:
	Point p1;
	Point p2;
	float color[3];
};

namespace navmesh
{

	class Polygon
	{
	public:
		vector<Point> points;
		vector<Triangle> triangles;
		vector<Edge> edges; // 三角剖分后的所有的三角形的边
		Grid grid;

	private:
		inline void CreateTriangle(Hash *eindex, int p1, int p2, int p3);
		inline int CreateEdge(Hash *eindexs, int triangle, int p1, int p2);
		inline int FindDT(Grid *grid, int p1, int p2);
		// Polygon的三角剖分
		void Delaunay();
		// 生成额外的数据
		void GenExtData();

	public:
		Point GetPoint(int p);
		bool IsIntersect(Grid *grid, int edgepos, int pa1, int p1);
		bool JudgeIsVisible(int pa1, int p1, Grid *grid);
		Polygon(double pos[], int size);
		vector<Line> GetLines();
		vector<Line> GetGrideLines();
		vector<Point> GetCenters();
		int IsFrist(int p) { return p == 4; }
		int FindTriangle(Point p);
		vector<Point> FindPath(Point from, Point to);
		Polygon() = default;
	};

}
#endif // NAVMESH_POLYGON_H