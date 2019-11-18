//====================================
// brief: Polygon类，代表寻路多边形的类，主要实现多边形的三角剖分和寻路
// author:sunxvming@163.com
// date:  2019-11-15
//====================================

#ifndef NAVMESH_POLYGON_H
#define NAVMESH_POLYGON_H

#include<vector>
#include<unordered_map>
#include"Point.h"
#include"Triangle.h"
#include"Edge.h"
#include"Circle.h"

using namespace std;

typedef unordered_map<int, int> Hash;
#define  MAXPOINT 100000 
#define PIndex(p1, p2) (p1>p2?(p2*MAXPOINT+p1):(p1*MAXPOINT+p2))

//====================================
//格子类，其中包含构成多边形的顶点链表和边链表	
//====================================
class Cell {
public:
	vector<int> points;
	vector<int> edges;
};

//====================================
// 多边形对应的网格结构类，由纵横的线分割的Cell（格子）组成
//====================================
class Grid {
public:
	vector<Cell> cells;   // 包含的所有格子
	int gride;            // 格子长度
	double minx;
	double miny;
	double maxx;
	double maxy;
	int xnum;
	int ynum;
};

//====================================
// 线段类，在屏幕上画网格时要用到
//====================================
class Line {
public:
	Point p1;
	Point p2;
	float color[3];
};

//====================================
// 多边形类，三角剖分和寻路逻辑的主要实现在此处
//====================================
class Polygon
{
public:
	vector<Point>     points;
	vector<Triangle>  triangles;
	vector<Edge>      edges;
	Grid              grid;
private:
	inline void CreateTriangle(Hash* eindex, int p1, int p2, int p3);
	inline int CreateEdge(Hash* eindexs, int triangle, int p1, int p2);
	inline int FindDT(Grid* grid, int p1, int p2);
	// Polygon的三角剖分
	void Delaunay();
	// 生成额外的数据
	void GenExtData();
public:
	Point GetPoint(int p);
	bool IsIntersect(Grid* grid, int edgepos, int pa1, int p1);
	bool JudgeIsVisible(int pa1, int p1, Grid* grid);
	Polygon(double* pos, int size);
	vector<Line> GetLines();
	vector<Line> GetGrideLines();
	vector<Point> Polygon::GetCenters();
	int IsFrist(int p) { return p == 4; }
	inline int Polygon::FindTriangle(Point p);
	vector<Point> Polygon::FindPath(Point from, Point to);
	virtual ~Polygon();
};

#endif // NAVMESH_POLYGON_H