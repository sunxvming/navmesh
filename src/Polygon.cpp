
#include <assert.h>
#include <cstdio>
#include <algorithm>
#include "Polygon.h"
#include <iostream>

using navmesh::Polygon;

Polygon::Polygon(double pos[], int size)
{
	assert(size > 10);
	assert(size < MAXPOINT * 2);
	for (int i = 0; i < size; i += 2)
	{
		points.push_back(Point(pos[i], pos[i + 1]));
	}
	Delaunay();
	GenExtData();
}

void Polygon::CreateTriangle(Hash *eindexs, int p1, int p2, int p3)
{
	triangles.push_back(Triangle(p1, p2, p3));
	int triangle = triangles.size() - 1;
	triangles[triangle].edges[0] = CreateEdge(eindexs, triangle, p1, p2); // 点的顺序要保持方向，外层逆时针，内层顺时针
	triangles[triangle].edges[1] = CreateEdge(eindexs, triangle, p3, p2);
	triangles[triangle].edges[2] = CreateEdge(eindexs, triangle, p1, p3);
}

int Polygon::CreateEdge(Hash *eindexs, int triangle, int p1, int p2)
{
	int k = PIndex(p1, p2);
	if (eindexs->find(k) == eindexs->end())
	{
		int v = edges.size();
		edges.push_back(Edge(triangle, -1, p1, p2));
		eindexs->insert(make_pair(k, v));
		return v;
	}
	int v = (*eindexs)[k];
	int t2 = edges[v].triangles[1];
	assert(t2 < 0);
	edges[v].triangles[1] = triangle;
	return v;
}

// 叉积 ac×bc, a在bc那一侧
double mult(Point a, Point b, Point c)
{
	return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
}

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) > (b) ? (b) : (a))
// aa, bb为一条线段两端点 cc, dd为另一条线段的两端点 相交返回true, 不相交返回false
bool intersect(Point aa, Point bb, Point cc, Point dd)
{
	if (max(aa.x, bb.x) < min(cc.x, dd.x))
	{
		return false;
	}
	if (max(aa.y, bb.y) < min(cc.y, dd.y))
	{
		return false;
	}
	if (max(cc.x, dd.x) < min(aa.x, bb.x))
	{
		return false;
	}
	if (max(cc.y, dd.y) < min(aa.y, bb.y))
	{
		return false;
	}
	// cd在ab同侧
	if (mult(cc, bb, aa) * mult(dd, bb, aa) > 0)
	{
		return false;
	}
	// ab在cd同侧
	if (mult(aa, dd, cc) * mult(bb, dd, cc) > 0)
	{
		return false;
	}
	return true;
}

// 根据点乘公式计算first-center-second的角度（弧度制）
static inline double Angle(Point cen, Point first, Point second)
{
	double dx1, dx2, dy1, dy2;

	dx1 = first.x - cen.x;
	dy1 = first.y - cen.y;
	dx2 = second.x - cen.x;
	dy2 = second.y - cen.y;
	// 求（cen 到 first 的距离）*（cen 到 second 的距离）
	double c = (double)sqrt(dx1 * dx1 + dy1 * dy1) * (double)sqrt(dx2 * dx2 + dy2 * dy2);
	// 避免 cen与  first  或者 second  重叠的现象
	if (c == 0)
		return 0;

	return (double)acos((dx1 * dx2 + dy1 * dy2) / c);
}

static inline int lerp(int v, int min, int max)
{
	if (v < min)
		v = min;
	if (v > max)
		v = max;
	return v;
}

int Polygon::FindDT(Grid *grid, int p1, int p2)
{
	double x1 = points[p1].x, y1 = points[p1].y;
	double x2 = points[p2].x, y2 = points[p2].y;
	double x = (x1 + x2) / 2, y = (y1 + y2) / 2;

	double minx = grid->minx, miny = grid->miny;
	double gride_len = grid->gride_len;
	int gx = (int)((x - minx) / gride_len);
	int gy = (int)((y - miny) / gride_len);
	int xnum = grid->xnum, ynum = grid->ynum;
	int d = 0;
	int p3 = -1;
	double angle3 = -1;
	Point point1 = GetPoint(p1), point2 = GetPoint(p2);
	while (1)
	{
		for (int i = -d; i <= d; i++)
		{
			for (int j = -d; j <= d; j++)
			{
				int pos = lerp(gx + i, 0, xnum - 1) + lerp(gy + j, 0, ynum - 1) * xnum;
				if (pos >= 0 && pos < (int)grid->cells.size())
				{
					Cell c = grid->cells[pos];
					for (unsigned k = 0; k < c.points.size(); k++)
					{
						int p = c.points[k];
						Point point = GetPoint(p);
						if (p1 != p && p2 != p && (point2 - point1).inLeft(point - point1)) // p 要在p1p2的左侧
						{
							bool flag = JudgeIsVisible(p1, p, grid) && JudgeIsVisible(p2, p, grid);

							if (flag)
							{
								double angle = Angle(point, point1, point2);
								if (p3 == -1 || angle > angle3)
								{
									angle3 = angle;
									p3 = p;
								}
							}
						}
					}
				}
			}
		}
		// 判断是否应该结束当前趟
		if (p3 != -1)
		{
			Circle c(point1, point2, GetPoint(p3));
			Point cc = c.GetCenter(); // 外接圆圆心
			double radius = c.GetR();
			double l = cc.x - radius, r = cc.x + radius, t = cc.y - radius, b = cc.y + radius;
			int lx = lerp((int)((l - minx) / gride_len), 0, xnum - 1);
			int rx = lerp((int)((r - minx) / gride_len), 0, xnum - 1);
			int ty = lerp((int)((t - miny) / gride_len), 0, ynum - 1);
			int by = lerp((int)((b - miny) / gride_len), 0, ynum - 1);
			if ((gx - d) <= lx && (gx + d) >= rx && (gy - d) <= ty && (gy + d) >= by)
				break;
		}
		d++;
	}
	assert(p3 != -1);
	return p3;
}

Point Polygon::GetPoint(int p)
{
	return points[p];
}

bool Polygon::IsIntersect(Grid *grid, int edgepos, int pa1, int p1)
{
	Point pa = GetPoint(pa1);
	Point p = GetPoint(p1);

	Cell c = grid->cells[edgepos];
	for (unsigned m = 0; m < c.edges.size(); m++)
	{
		int eposId = c.edges[m];
		int next_eposId = (eposId + 1) % points.size();

		bool flag = intersect(GetPoint(next_eposId), GetPoint(eposId), pa, p);
		if ((eposId == pa1) || (eposId == p1) || (next_eposId == pa1) || (next_eposId == p1))
		{
			flag = false;
		}
		if (flag)
			return true;
	}
	return false;
}

// false代表pa 与p不可见，true代表可见
bool Polygon::JudgeIsVisible(int pindex1, int pindex2, Grid *grid)
{
	Point p1 = GetPoint(pindex1);
	Point p2 = GetPoint(pindex2);

	// 检查最大最小值合法性
	Point p0 = points[0];
	double minx = grid->minx;
	double miny = grid->miny;

	int gride = grid->gride_len;
	int xnum = grid->xnum;
	int ynum = grid->ynum;

	if (p1.x > p2.x)
	{
		Point demo = p2;
		p2 = p1;
		p1 = demo;
	}
	int xn1 = (int)((p1.x - minx) / gride);
	int yn1 = (int)((p1.y - miny) / gride);
	int xn2 = (int)((p2.x - minx) / gride);
	int yn2 = (int)((p2.y - miny) / gride);

	if (xn1 == xn2)
	{
		if (yn1 > yn2)
		{
			yn1 = yn1 ^ yn2;
			yn2 = yn1 ^ yn2;
			yn1 = yn1 ^ yn2;
		}
		for (int j = yn1; j <= yn2; j++)
		{
			if (j > ynum)
				break;
			int edgepos = (xn1 >= xnum ? xnum - 1 : xn1) + (j >= ynum ? ynum - 1 : j) * xnum;
			if (IsIntersect(grid, edgepos, pindex1, pindex2))
				return false;
		}
	}
	else
	{
		double x = p1.x;
		double y = p1.y;
		for (int i = xn1; i <= xn2; i++)
		{
			double x3 = (i + 1) * gride + minx;
			if (x3 > p2.x)
				x3 = p2.x;
			// 两点求得方程 (y-y2)/(y1-y2)=(x-x2)/(x1-x2)，把x3代入计算y3
			double y3 = (p2.x - x3) * (p2.y - p1.y) / (p1.x - p2.x) + p2.y;

			int cur_x = (int)((x - minx) / gride);
			int cur_y = (int)((y - miny) / gride);
			int next_y = (int)((y3 - miny) / gride);
			if (cur_y > next_y)
			{
				cur_y = cur_y ^ next_y;
				next_y = cur_y ^ next_y;
				cur_y = cur_y ^ next_y;
			}
			for (int j = cur_y; j <= next_y; j++)
			{
				if (j > ynum)
					break;
				int edgepos = (cur_x >= xnum ? xnum - 1 : cur_x) + (j >= ynum ? ynum - 1 : j) * xnum;
				if (IsIntersect(grid, edgepos, pindex1, pindex2))
					return false;
			}
			x = x3;
			y = y3;
		}
	}
	return true;
}

void Polygon::Delaunay()
{
	// 检查最大最小值合法性
	Point p0 = points[0];
	double minx = p0.x, miny = p0.y, maxx = p0.x, maxy = p0.y;
	for (auto const &it : points)
	{
		double x = it.x;
		double y = it.y;
		if (x < minx)
			minx = x;
		if (x > maxx)
			maxx = x;
		if (y < miny)
			miny = y;
		if (y > maxy)
			maxy = y;
	}

	double dx = maxx - minx, dy = maxy - miny;
	int gride_len = (int)sqrt(dx * dy / (points.size()));
	int xnum = (int)ceil(dx / gride_len);
	int ynum = (int)ceil(dy / gride_len);

	grid.gride_len = gride_len;
	grid.minx = minx;
	grid.maxx = maxx;
	grid.miny = miny;
	grid.maxy = maxy;
	grid.xnum = xnum;
	grid.ynum = ynum;

	// 把点和边都放到格子里
	vector<Cell> cells(xnum * ynum);
	for (auto it = points.cbegin(); it != points.cend(); it++)
	{
		// point放到Grid里
		double x = it->x;
		double y = it->y;
		int xn = (int)((x - minx) / gride_len);
		int yn = (int)((y - miny) / gride_len);
		int pos = (xn >= xnum ? xnum - 1 : xn) + (yn >= ynum ? ynum - 1 : yn) * xnum;
		int point = it - points.cbegin();
		cells[pos].points.push_back(point);

		// edges放到Grid里
		Point p = GetPoint(point);
		Point p1 = GetPoint((point + 1) % points.size());
		if (p.x > p1.x)
		{
			Point demo = p;
			p = p1;
			p1 = demo;
		}
		int xn1 = (int)((p.x - minx) / gride_len);
		int xn2 = (int)((p1.x - minx) / gride_len);
		int yn1 = (int)((p.y - miny) / gride_len);
		int yn2 = (int)((p1.y - miny) / gride_len);

		if (xn1 == xn2)
		{
			if (yn1 > yn2)
			{
				yn1 = yn1 ^ yn2;
				yn2 = yn1 ^ yn2;
				yn1 = yn1 ^ yn2;
			}
			for (int j = yn1; j <= yn2; j++)
			{
				if (j > ynum)
					break;
				int edgepos = (xn1 >= xnum ? xnum - 1 : xn1) + (j >= ynum ? ynum - 1 : j) * xnum;
				cells[edgepos].edges.push_back(point);
			}
		}
		else
		{

			// 将x值偏小的 y值保存起来
			double y = p.y;
			double x = p.x;
			for (int i = xn1; i <= xn2; i++)
			{
				double x3 = (i + 1) * gride_len + minx;
				if (x3 > p1.x)
					x3 = p1.x;
				// 两点求得方程 (y-y2)/(y1-y2)=(x-x2)/(x1-x2)，把x3代入计算y3, 方程是边的方程
				// 知道直线方程后，分别把格子两端的x值代入方程，就可以知道直线穿过的y方向的格子
				double y3 = (p.x - x3) * (p.y - p1.y) / (p1.x - p.x) + p.y;

				int cur_x = (int)((x - minx) / gride_len);
				int cur_y = (int)((y - miny) / gride_len);	 // 格子左侧y点坐标
				int next_y = (int)((y3 - miny) / gride_len); // 格子右侧y点坐标
				if (cur_y > next_y)
				{
					cur_y = cur_y ^ next_y;
					next_y = cur_y ^ next_y;
					cur_y = cur_y ^ next_y;
				}
				for (int j = cur_y; j <= next_y; j++) // 记录x前进一格，线段在y方向上跨越的格子数
				{
					if (j > ynum)
						break;
					int edgepos = (cur_x >= xnum ? xnum - 1 : cur_x) + (j >= ynum ? ynum - 1 : j) * xnum;
					cells[edgepos].edges.push_back(point);
				}
				x = x3;
				y = y3;
			}
		}
	}

	grid.cells = cells;

	Hash eindexs;	// k->v (Pindex->edges的index)，存放着已经处理过的所有三角形的边
	Hash restrains; // 存放着所有的约束边，即内外边界的边
	for (unsigned i = 0; i < points.size() - 1; i++)
	{
		restrains.insert(make_pair((int)PIndex(i, i + 1), 1));
	}
	restrains.insert(make_pair((int)PIndex(0, points.size() - 1), 1));

	vector<int> es; // 待处理的边的栈
	int p1 = 0, p2 = 1;
	int e = PIndex(p1, p2);
	while (1)
	{
		int p3 = FindDT(&grid, p1, p2);
		if (restrains.find((int)PIndex(p1, p3)) == restrains.end())
		{
			vector<int>::iterator it = std::find(es.begin(), es.end(), PIndex(p1, p3));
			if (it == es.end())
			{
				es.push_back(PIndex(p1, p3));
			}
			else
			{
				es.erase(it);
			}
		}
		if (restrains.find((int)PIndex(p2, p3)) == restrains.end())
		{
			vector<int>::iterator it = std::find(es.begin(), es.end(), PIndex(p2, p3));
			if (it == es.end())
			{
				es.push_back(PIndex(p2, p3));
			}
			else
			{
				es.erase(it);
			}
		}
		CreateTriangle(&eindexs, p1, p2, p3);
		if (es.empty())
			break;
		// 得到新的两个点，循环计算。如果es为空。则说明已经遍历完，三角形网格化停止
		e = es.back();
		es.pop_back();
		int *points = edges[eindexs[e]].points;
		p1 = points[0], p2 = points[1];
	}
}

#define distance(p1, p2) ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y))

void Polygon::GenExtData()
{
	for (unsigned i = 0; i < triangles.size(); i++)
	{
		Triangle t = triangles[i];
		t.GenExtData(this);
		triangles[i] = t;
	}

	for (unsigned i = 0; i < edges.size(); i++)
	{
		Edge e = edges[i];
		int e0 = e.triangles[0];
		int e1 = e.triangles[1];
		if (e0 >= 0 && e1 >= 0) // 一条边两个三角形的
		{
			Triangle t0 = triangles[e0];
			int p0 = e.points[0], p1 = e.points[1];

			// 使其在第一个三角中，让p1位于p0的右侧（点的位置固定起来，p0是左点，p1是右点），使寻路时拐点算法的两边方向都是一致的
			Point pp0 = GetPoint(p0) - t0.icenter;
			Point pp1 = GetPoint(p1) - t0.icenter;
			if (pp0.inLeft(pp1))
			{
				e.points[0] = p1;
				e.points[1] = p0;
				edges[i] = e;
			}
		}
	}
}

vector<Line> Polygon::GetLines()
{
	vector<Line> lines(edges.size());
	for (unsigned i = 0; i < edges.size(); i++)
	{
		Line line;
		line.p1 = GetPoint(edges[i].points[0]);
		line.p2 = GetPoint(edges[i].points[1]);
		if (edges[i].IsRestrain(this))
		{
			line.color[0] = 1.0;
			line.color[1] = 0.0;
			line.color[2] = 0.0;
		}
		else
		{
			line.color[0] = 0.0;
			line.color[1] = 1.0;
			line.color[2] = 0.0;
		}
		lines[i] = line;
	}
	return lines;
}

vector<Line> Polygon::GetGrideLines()
{
	vector<Line> lines(grid.ynum + grid.xnum + 2);
	// 纵向的线
	for (int i = 0; i <= grid.xnum; i++)
	{
		Line line;
		Point pa1, pa2;
		double x = i * grid.gride_len + grid.minx;
		pa1.x = x;
		pa1.y = grid.miny;

		pa2.x = x;
		pa2.y = grid.miny + grid.ynum * grid.gride_len;
		line.p1 = pa1;
		line.p2 = pa2;

		line.color[0] = 0.2f;
		line.color[1] = 0.3f;
		line.color[2] = 0.3f;

		lines[i] = line;
	}
	// 横向的线
	for (int j = 0; j <= grid.ynum; j++)
	{
		Line line;
		Point pa1, pa2;
		double y = j * grid.gride_len + grid.miny;
		pa1.x = grid.minx;
		pa1.y = y;
		pa2.x = grid.minx + grid.xnum * grid.gride_len;
		pa2.y = y;
		line.p1 = pa1;
		line.p2 = pa2;

		line.color[0] = 0.2f;
		line.color[1] = 0.3f;
		line.color[2] = 0.3f;

		lines[j + grid.xnum + 1] = line;
	}
	return lines;
}

vector<Point> Polygon::GetCenters()
{
	vector<Point> centers(triangles.size());
	for (unsigned i = 0; i < triangles.size(); i++)
	{
		centers[i] = triangles[i].icenter;
	}
	return centers;
}

int Polygon::FindTriangle(Point p)
{
	for (unsigned i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].Contain(this, p))
			return i;
	}
	return -1;
}
// 找路径 from是起点，to是终点
vector<Point> Polygon::FindPath(Point from, Point to)
{
	std::cout << "find path==========" << std::endl;
	int tfrom = FindTriangle(from);
	int tto = FindTriangle(to);

	vector<int> ts; // 所有的三角形
	vector<int> es; // 所有的经过的边，用于拐点算法
	vector<Point> ways;
	if (tfrom < 0)
		return ways;
	if (tto < 0)
		return ways;
	ways.push_back(from);
	if (tfrom == tto)
	{
		ways.push_back(to);
		return ways;
	}
	ts.push_back(tfrom); // 栈，存放三角形
	// A*  F = G() + H()  变形的A*或者称为贪心算法的网格遍历
	// G() = 0   H() = 当前点到终点的距离
	int start = tfrom;
	Hash visited;
	visited.insert(make_pair(tfrom, 1));

	while (!ts.empty())
	{
		start = ts.back();
		double weight = -1;
		int next = -1;
		int e;
		for (int i = 0; i < 3; i++)
		{
			int eindex = triangles[start].edges[i];
			Edge edge = edges[eindex];
			// 找到非约束边的，非原三角形的另外其他的三角形
			int nextt = edge.triangles[0] == start ? edge.triangles[1] : edge.triangles[0];
			if (nextt >= 0) // 非约束边的，存在相邻三角形的
			{
				if (nextt == tto)
				{
					next = tto;
					e = eindex;
					break;
				}
				if (visited.find(nextt) == visited.end())
				{
					Point p = triangles[nextt].icenter;
					double d = distance(p, to);
					if (next < 0 || weight > d)
						next = nextt, weight = d, e = eindex;
				}
			}
		}
		if (next == tto)
		{
			ts.push_back(next);
			es.push_back(e);
			break;
		}
		else if (next >= 0)
		{
			visited.insert(make_pair(next, 1));
			ts.push_back(next);
			es.push_back(e);
		}
		else
		{ // 没有路可走，再进行回溯然后继续查找，栈就具有这种回溯的功能
			ts.pop_back();
			es.pop_back();
		}
	}

	if (ts.empty())
		return ways;

	for (int i = 0; i < triangles.size(); i++)
	{
		triangles[i].isPath = false;
	}
	for (int i : ts)
	{
		triangles[i].isPath = true;
	}

	// ===== 拐点算法 =====
	// main variables:
	// e  ep_l  ep_r   p_l  p_r   line_l  line_r  t  t_l  t_r
	// ne nep_l nep_r  np_l np_r
	int t = 0; // 第几个穿出边
	int size = es.size();
	while (t < size) // 每一次循环就是要找到一个拐点，然后下一次再从这个拐点找下一个拐点
	{
		Point p = ways.back();
		Edge e = edges[es[t]];
		int ep_r, ep_l;
		// ts[t]表示当前要处理的三角
		if (ts[t] == e.triangles[0]) // 判断是否是第0个三角，第0个三角p1是在p0左侧的
		{
			ep_r = e.points[1], ep_l = e.points[0];
		}
		else
		{ // 是第一个三角，p1在p0的右侧，需要换下顺序
			ep_r = e.points[0], ep_l = e.points[1];
		}

		Point p_r = GetPoint(ep_r), p_l = GetPoint(ep_l);
		Point line_r = p_r - p, line_l = p_l - p;
		int i = t + 1;
		int t_r = i, t_l = i;
		// 每一次循环处理一个穿出边，直到找到一个拐点
		std::cout << "size = " << size << std::endl;
		for (; i < size; i++)
		{
			std::cout << "i = " << i << std::endl;
			Edge ne = edges[es[i]];
			int nep_r, nep_l;
			if (ts[i] == ne.triangles[0])
			{
				nep_r = ne.points[1], nep_l = ne.points[0];
			}
			else
			{
				nep_r = ne.points[0], nep_l = ne.points[1];
			}
			Point np_r = GetPoint(nep_r), np_l = GetPoint(nep_l); // np1是右点，np2是左点
			// 处理右线
			if (line_r.inRight(np_l - p) || line_r.inSame(np_l - p)) // 左点在*右线*的右侧，直接确定左点为拐点
			{
				std::cout << "左点在*右线*的右侧，直接确定左点为拐点" << std::endl;
				p = GetPoint(ep_r);
				ways.push_back(p);
				t_r++;
				t = nep_r == ep_r ? (i++, t_r + 1) : t_r;
				break;
			}
			else if (line_r.inLeft(np_r - p) || line_r.inSame(np_r - p)) // 右点在*右线*的左侧，更新右线
			{
				std::cout << "右点在*右线*的左侧，更新右线" << std::endl;
				line_r = np_r - p;
				ep_r = nep_r;
				t_r = i;
			}
			// 处理左线
			if (line_l.inLeft(np_r - p) || line_l.inSame(np_r - p)) // 右点在*左线*的左侧，直接确定拐点
			{
				std::cout << "右点在*左线*的左侧，直接确定右点为拐点" << std::endl;
				p = GetPoint(ep_l);
				ways.push_back(p);
				t_l++;
				t = nep_l == ep_l ? (i++, t_l + 1) : t_l;
				break;
			}
			else if (line_l.inRight(np_l - p) || line_l.inSame(np_l - p)) // 左点在*左线*的右侧，更新左线
			{
				std::cout << "左点在*左线*的右侧，更新左线" << std::endl;
				line_l = np_l - p;
				ep_l = nep_l;
				t_l = i;
			}
		}
		if (i >= size - 1) // 左线和右线一直更新更新到了最后
		{
			std::cout << "go in last deal" << std::endl;
			if (line_r.inRight(to - p) || line_r.inSame(to - p)) // 终点在line1（右线）右侧
			{
				std::cout << "go 1111111111" << std::endl;
				ways.push_back(GetPoint(ep_r)); // 左点为拐点
				t = t_r;
			}
			else if (line_l.inLeft(to - p) || line_l.inSame(to - p)) // 终点在line2（左线）左侧
			{
				std::cout << "go 222222222222" << std::endl;

				ways.push_back(GetPoint(ep_l));
				t = t_l;
			}
			if (t >= size - 1) // //终点在中间
			{
				ways.push_back(to);
			}
			t++;
		}
	}

	return ways;
}
