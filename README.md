> 这个是俺的[github](https://github.com/sunxvming)上的一个[导航网格寻路小demo](https://github.com/sunxvming/NavMesh)的readme文件，如果读后兴趣的话可以去俺的github上去溜达一圈，可能会有点小收获呢，哈哈哈哈 :)





## 小demo说明
每个mmorpg游戏中都会少不了有寻路模块，且还是开发中的一个难点，涉及到地形资源、客户端、服务器端，涉及到的算法还比较多，还得注意它的性能开销。相信不少游戏开发者都想弄清楚它是怎么实现的。网上讲解这方面的文章也有不少，github上的开源实现也有一些，不过有很大一部分是unity相关的，或者c#实现的。c++的实现也有，但是没几个像俺这样对初级中级程序员这么友好的。友好的原因如下：
* 俺的这版是导航网格寻路的最简化版，仅是实现了平面多边形(没有嵌套的)的三角剖分和两点之间的寻路，代码量可谓相当少，实现的也比较通俗易懂。
* 俺还会提供一些资料和一些线索来引导你去了解实现的过程。

了解学习一个东西如果一下子把你带到一个很难很深的level的话，恐怕还多人都是吃不消的。就应该循序渐进，按部登阶，从简入深。俺这个就是遵循这样的精神滴。这个是个入门版的，相信聪明的你很快就能看懂学会，说不定修改就直接用到你们的游戏里面！此项目为入门版，进阶版的请看这里[导航网格寻路C++实现版(进阶版)](https://github.com/sunxvming/navmesh-advance)。

## 使用说明
代码这都有，你需要的就是fork或者down下来，然后再你的电脑上编译一下。如果你是用的VS的话，你需要设置一下包含目录和库目录，然后把glut.dll放在你的程序运行目录。编译成功后再运行你会看到如下的程序运行界面：
![](http://www.sunxvming.com/imgs/微信截图_20191117163127.jpg)
其中红色的线是平面多边形，你可以把它看成游戏中的地型。
绿色的线表示经三角剖分后构成的线
蓝色的线代表寻路的路径
程序运行时在窗口点击第一下为寻路的起点，再点击第二下为寻路的终点。点击第三下会把之前的路径清空。

## 主要算法
这些都可以在网上查的到，在俺的小demo中应该也很容易找的到的
* 平面多边形三角剖分(Delaunay剖分)
 + 多边形用什么数据结构来表示
 + 各种几何图形用什么数据结构来表示
 + 如何生成网格以及附带数据
 + 如何确定一个DT点
 + 如何遍历整个多边形来完成三角剖分
* 寻路算法
* 寻路拐点算法
* 判断两条线段是否相交
* 判断点是否在三角形中
* 判断点在向量的那一侧
* 已知三点求三点的夹角

## Delaunay剖分是啥？
Delaunay三角剖分其实并不是一种算法，而是一种三角剖分的标准，实现它有多种算法。它只是给出了一个“好的”三角网格的定义，它的优秀特性是空圆特性和最大化最小角特性，这两个特性避免了狭长三角形的产生，也使得Delaunay三角剖分应用广泛。
空圆特性其实就是对于两个共边的三角形，任意一个三角形的外接圆中都不能包含有另一个三角形的顶点，这种形式的剖分产生的最小角最大(比不满足空圆特性的最小角大)

## 主要类说明
```
// Polygon类，代表寻路多边形的类，主要实现多边形的三角剖分和寻路
Polygon
 vector<Point> points;
 vector<Triangle> triangles;
 vector<Edge> edges;
 Grid grid;
 // 三角化
 Delaunay()
  eindexs // k->v (Pindex->edges的index)，存放着已经处理过的所有三角形的边
  restrains // 存放着所有的约束边，在处理开始就已经知道
 // 寻路
 FindPath()
// 多边形对应的网格结构类，由纵横的线分割的Cell（格子）组成
Grid
 vector<Cell> cells;
 int gride;
 double minx;
 double miny;
 double maxx;
 double maxy;
 int xnum;
 int ynum;
//格子类，其中包含构成多边形的顶点链表和边链表
Cell
 vector<int> points;
 vector<int> edges;
// Triangle类，三角形的表示类
Triangle
 int p1;
 int p2;
 int p3;
 int edges[3];
 Point icenter;//重心
 Point lt;
 Point rb;
// Edge类，表示经过剖分后的线段，一条线段可能包含一个或两个三角，包含两个点
Edge
 int triangles[2];
 int points[2];
// Line类，画网格线的时候用到
Line
 Point p1;
 Point p2;
// Point类，点的表示类
Point
 double x;
 double y;
// Circle类，表示圆
Circle
 Point center;
 double r;
```

## 参考链接
* [平面多边形域的快速约束Delaunay三角化](https://github.com/sunxvming/navmesh/blob/master/doc/%E5%B9%B3%E9%9D%A2%E5%A4%9A%E8%BE%B9%E5%BD%A2%E5%9F%9F%E7%9A%84%E5%BF%AB%E9%80%9F%E7%BA%A6%E6%9D%9FDelaunay%E4%B8%89%E8%A7%92%E5%8C%96.pdf)
* [Nav导航网格寻路](https://blog.csdn.net/ynnmnm/article/details/44833007)
* [深入理解游戏中寻路算法](https://my.oschina.net/u/1859679/blog/1486636)
* [Astar寻路算法-中文](https://github.com/sunxvming/navmesh/blob/master/doc/Astar%E5%AF%BB%E8%B7%AF%E7%AE%97%E6%B3%95-%E4%B8%AD%E6%96%87.doc)