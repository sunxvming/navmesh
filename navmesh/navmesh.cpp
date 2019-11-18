//====================================
// brief: 程序的main入口文件，调用glut库来画经过三角剖分后的多边形网格和寻路的路径
// author:sunxvming@163.com
// date:  2019-11-16
//====================================

#include <stdio.h>
#include <glut.h>
#include "Polygon.h"


int fromx = -1;
int fromy = -1;
int tox = -1;
int toy  = -1;
int clicknum = 0;


// 根据输入的点创建多边形，根据鼠标点击的点作为起点输终点来进行寻路，并绘制网格和路径图像
static void DrawLines() {
	double pos[] = { 50,50,275,100,425,60,450,160,350,230, 600, 325, 750,200,750,500, 525,460,575,550,425,550,480,450,325,425,300,300,30,400,85,275 };
	//double pos[] = { 0, 126 ,119,118,158,0 ,200,122, 317,120 ,224,201, 260, 317, 163, 245, 65, 316, 99, 196 };
	//double pos[] = { 150, 100, 300, 100, 350, 150, 400, 100, 550, 100, 600, 150, 450, 450, 200, 500, 200, 350, 50, 250 };
	//double pos[] = {50, 150,150,100,200,100,250,200, 300, 150,250,250,200,250,150,200,150,150,50,250};
	vector<Point> ways;
	Polygon* p = new Polygon(pos, sizeof(pos)/sizeof(double));
	//前一个Point是起点，后一个Point是终点。
	//（0，0）在坐上角
	if (fromx > 0 && fromy > 0 && tox > 0 && toy > 0)
	{
		ways = p->FindPath(Point(fromx, fromy), Point(tox, toy));
	}

	glEnable(GL_LINE_STIPPLE);
	glLineWidth(2);//设置线段宽度
	glBegin(GL_LINES);

	//辅助的格子线
	vector<Line> gridelines = p->GetGrideLines();
	for (unsigned i = 0; i < gridelines.size(); i++)
	{
		Line grideline = gridelines[i];
		glColor3fv(grideline.color);
		glVertex2f((GLfloat)grideline.p1.x, (GLfloat)grideline.p1.y);
		glVertex2f((GLfloat)grideline.p2.x, (GLfloat)grideline.p2.y);
	}

	vector<Line> lines = p->GetLines();
	for (unsigned i = 0; i < lines.size(); i++)
	{
		Line line = lines[i];
		glColor3fv(line.color);
		glVertex2f((GLfloat)line.p1.x, (GLfloat)line.p1.y);
		glVertex2f((GLfloat)line.p2.x, (GLfloat)line.p2.y);
	}

	

	for (unsigned i = 1; i < ways.size(); i++)
	{
		glColor3f(0.0f, 0.0, 1.0);
		glVertex2f((GLfloat)ways[i-1].x, (GLfloat)ways[i-1].y);
		glVertex2f((GLfloat)ways[i].x, (GLfloat)ways[i].y);
	}
	glEnd();

	glPointSize(3);
	glBegin(GL_POINTS);
	auto centers = p->GetCenters();
	for (unsigned i = 0; i < centers.size(); i++)
	{
		glVertex3f(centers[i].x, centers[i].y, 0.0);
	}
	glEnd();

	glDisable(GL_LINE_STIPPLE);
	
}
 
void myDisplay(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
    DrawLines();
	glFlush();
}


// 鼠标点击的回调函数，目的为获取用户的鼠标点输入，并作为寻路的起点和终点来进行寻路
// param ：
//	button为GLUT_LEFT_BUTTON或GLUT_RIGHT_BUTTON分别表示左右按键。
//	state为按键的状态，若为按下则为GLUT_DOWN
void myClick(int button, int state, int x, int y)
{
	if (state == 1) //抬起是1
	{
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
		}
		printf("click---state=%d,clicknum=%d,t=%d, x=%d,y=%d\n", state,clicknum,t, x, y);
		myDisplay();
	}
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(30, 30);  //定义窗口位置
	glutInitWindowSize(1000, 800);  //定义窗口大小
	glutCreateWindow("寻路测试");
	//gluOrtho2D(x_mix, x_max, y_mix, y_max)
	//用于截取世界坐标系相应区域。在世界坐标系中以从(x_mix, x_max)到(y_mix, y_max)的直线为对角线的
	//矩形部分截取图像，该截图可以被用于显示。
	gluOrtho2D(0, 1000, 800, 0);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myClick);
	glutMainLoop();
	return 0;
}