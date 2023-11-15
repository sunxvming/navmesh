//====================================
// brief: 程序的main入口文件，调用glut库来画经过三角剖分后的多边形网格和寻路的路径
// author:sunxvming@163.com
// date:  2019-11-16
//====================================
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <GL/glut.h>
#include "Polygon.h"

int fromx = -1;
int fromy = -1;
int tox = -1;
int toy = -1;
int clicknum = 0;
double pos[] = {50, 50, 275, 100, 425, 60, 450, 160, 350, 230, 600, 325, 750, 200, 750, 500, 525, 460, 575, 550, 425, 550, 480, 450, 325, 425, 300, 300, 30, 400, 85, 275};
// double pos[] = {0, 126, 119, 118, 158, 0, 200, 122, 317, 120, 224, 201, 260, 317, 163, 245, 65, 316, 99, 196};
// double pos[] = {150, 100, 300, 100, 350, 150, 400, 100, 550, 100, 600, 150, 450, 450, 200, 500, 200, 350, 50, 250};
// double pos[] = {50, 150,150,100,200,100,250,200, 300, 150,250,250,200,250,150,200,150,150,50,250};
std::shared_ptr<navmesh::Polygon> polygon = make_shared<navmesh::Polygon>(pos, sizeof(pos) / sizeof(double));
vector<Point> ways;

static void DrawMap()
{
	// （0,0）is in the upper left corner

	glLineWidth(2);
	glBegin(GL_LINES);

	// draw grid lines
	vector<Line> gridelines = polygon->GetGrideLines();
	for (unsigned i = 0; i < gridelines.size(); i++)
	{
		Line grideline = gridelines[i];
		glColor3fv(grideline.color);
		glVertex2f((GLfloat)grideline.p1.x, (GLfloat)grideline.p1.y);
		glVertex2f((GLfloat)grideline.p2.x, (GLfloat)grideline.p2.y);
	}
	glEnd();

	// draw triangle than path has been found
	for (const auto &tri : polygon->triangles)
	{

		if (tri.isPath && ways.size() > 0)
		{
			glBegin(GL_TRIANGLES);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex2f((GLfloat)polygon->points[tri.p1].x, (GLfloat)polygon->points[tri.p1].y);
			glVertex2f((GLfloat)polygon->points[tri.p2].x, (GLfloat)polygon->points[tri.p2].y);
			glVertex2f((GLfloat)polygon->points[tri.p3].x, (GLfloat)polygon->points[tri.p3].y);
			glEnd();
		}
	}

	glBegin(GL_LINES);
	// draw polygon lines
	vector<Line> lines = polygon->GetLines();
	for (unsigned i = 0; i < lines.size(); i++)
	{
		Line line = lines[i];
		glColor3fv(line.color);
		glVertex2f((GLfloat)line.p1.x, (GLfloat)line.p1.y);
		glVertex2f((GLfloat)line.p2.x, (GLfloat)line.p2.y);
	}

	// draw path lines
	for (unsigned i = 1; i < ways.size(); i++)
	{
		glColor3f(0.0f, 0.0, 1.0);
		glVertex2f((GLfloat)ways[i - 1].x, (GLfloat)ways[i - 1].y);
		glVertex2f((GLfloat)ways[i].x, (GLfloat)ways[i].y);
	}
	glEnd();

	// draw triangle centers
	glPointSize(3);
	glBegin(GL_POINTS);
	auto centers = polygon->GetCenters();
	for (unsigned i = 0; i < centers.size(); i++)
	{
		glColor3f(0.5f, 0.5, 0.5);
		glVertex3f(centers[i].x, centers[i].y, 0.0);
	}
	glEnd();

	// draw start and end point
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0, 0.0);
	if (fromx != -1 && fromy != -1)
	{
		glVertex3f(fromx, fromy, 0.0);
	}
	if (tox != -1 && toy != -1)
	{
		glVertex3f(tox, toy, 0.0);
	}
	glEnd();
}

void myDisplay(void)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	DrawMap();
	glFlush();
}

// 鼠标点击的回调函数，目的为获取用户的鼠标点输入，并作为寻路的起点和终点来进行寻路
// param ：
//	button: GLUT_LEFT_BUTTON或GLUT_RIGHT_BUTTON分别表示左右按键。
//	state: 按键的状态，若为按下则为GLUT_DOWN
void myClick(int button, int state, int x, int y)
{
	if (state == 1) // 抬起是1
	{
		ways.clear();
		clicknum += 1;
		int t = clicknum % 3;
		if (t == 0)
		{
			fromx = fromy = tox = toy = -1;
		}
		else if (t == 1)
		{
			fromx = x;
			fromy = y;
		}
		else if (t == 2)
		{
			tox = x;
			toy = y;
			ways = polygon->FindPath(Point(fromx, fromy), Point(tox, toy));
		}
		myDisplay();
	}
}

int main(int argc, char *argv[])
{

	Point p1(0, 10);
	p1.inLeft(Point(1, 1));

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(30, 30); // 定义窗口位置
	glutInitWindowSize(1000, 800);	// 定义窗口大小
	glutCreateWindow("navmesh test");
	// gluOrtho2D(x_mix, x_max, y_mix, y_max)
	// 用于截取世界坐标系相应区域。在世界坐标系中以从(x_mix, x_max)到(y_mix, y_max)的直线为对角线的
	// 矩形部分截取图像，该截图可以被用于显示。
	gluOrtho2D(0, 1000, 800, 0);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myClick);
	glutMainLoop();
	return 0;
}