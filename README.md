
## 导航网格寻路项目说明
每个mmorpg游戏中都会少不了有寻路模块，且还是开发中的一个难点，涉及到地形资源、客户端、服务器端，涉及到的算法还比较多，还得注意它的性能开销。相信不少游戏开发者都想弄清楚它是怎么实现的。网上讲解这方面的文章也有不少，github上的开源实现也有一些，不过有很大一部分是unity相关的，或者c#实现的。c++的实现也有，但是没几个像俺这样对初级中级程序员这么友好的。友好的原因如下：

* 俺的这版是导航网格寻路的最简化版，仅是实现了平面多边形(没有相互嵌套的)的三角剖分和两点之间的寻路，代码量可谓相当少，实现的也比较通俗易懂。
* 俺还会提供一些资料和一些线索来引导你去了解实现的过程。

了解学习一个东西如果一下子把你带到一个很难很深的level的话，恐怕还多人都是吃不消的。就应该循序渐进，按步登阶，从简入深。俺这个就是遵循这样的精神滴。这个是个入门版的，相信聪明的你很快就能看懂学会，说不定修改就直接用到你们的游戏里面！此项目为入门版，功能少且简单，进阶版的请看这里[导航网格寻路C++实现版(进阶版)](https://github.com/sunxvming/navmesh-advance)。


## 寻路功能的实现步骤
### 1.寻路地图文件的生成
对游戏中需要寻路的场景通常都需要对其进行相应的处理以生成专门针对寻路功能所使用的文件，文件里面包含了地图的信息，比如地图的顶点、边、三角形等。这个文件的生成通常是由专门的生成工具来完成的，比如在unity中，可以通过Unity的插件把场景文件导出为寻路地图文件。
### 2.对地图文件进行格栅化
对于寻路地图文件，我们需要对其进行进一步的格栅化处理，格栅化通常指的是将游戏地图划分为离散的网格的过程，网格又分为可行走区域和不可行走区域。格栅化后以便寻路算法能够更有效地进行路径搜索 。
常见的栅格化有正方形网格、三角形网格：

- **正方形网格**：易于实现和理解，方法简单直观，但不适用于不规则地形。其中格子选择的大小对寻路算法影响很大。小格子供更高的地图精度，但算法效率低。大格子效率高但提供的精度低。
- **三角形网格**：适用于复杂地形，节省空间，但实现稍复杂些。

如何你对正方形网格格栅化感兴趣的话，可以看一下[俺的github](https://github.com/sunxvming)上的另一个[路径规划的工程](https://github.com/sunxvming/pathplan)，其中的`pathplan-gird`目录下的程序用的便是正方形网格，程序运行的截图如下：
![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20240214150332.png)

### 3.网格寻路
网格寻路是一种基于网格的寻路算法，它的基本思想是：首先将地图划分为一个个的网格，然后在网格中寻找一条从起点到终点的路径。网格寻路的算法有很多种，比如`A*`、`Dijkstra`、`BFS`、`DFS`等等，通常可以选择`A*`算法来实现。



### 4.根据寻路的网格生成最终的路径
在网格寻路的过程中，我们只是找到了一条从起点到终点的网格路径，但是这条路径并不是我们想要的最终路径，因为这条路径是由网格组成的，而我们需要的是由顶点组成的路径。所以我们需要根据网格路径生成最终的路径。这种算法通常称为路径的二次优化算法。本工程使用拐点算法来实现。

## 编译方式说明
此工程的编译运行环境为windows，需要你的电脑的开发环境有cmake、Visual Studio或make、MinGW 
### 一、cmake+visual studio编译
1.在cmake中设置源码目录和build目录
![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20230113115226.png)

2.点击`configure`设置visual studio的版本，和64位应用程序
![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20240131153911.png)

3.点击`Generrate`生成vs工程，生成的目录为`build`目录，用vs打开生成的工程并运行，可执行程序会生成到当前项目根目录的`bin`目录下


### 二、mingw+make+gcc编译
在项目根目录执行`make`命令即可，可执行程序同样会生成到`bin`目录下

### 三、VScode+cmake插件编译
1.选择编译套件
![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20240131165800.png)

![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20240131165840.png)

2.编译
![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20240131165915.png)

3.运行
![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20240131165954.png)


## 程序运行说明

编译成功后再运行你会看到如下的程序运行界面：
![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20240131170422.png)


界面说明:
* 红色的线是平面多边形，你可以把它看成游戏中的地型。
* 绿色的线表示经三角剖分后构成的线
* 蓝色的线代表寻路的路径
* 灰色的格子代表路径规划所跨越的格子

操作说明：
- 程序运行时在窗口点击第一下为寻路的起点，再点击第二下为寻路的终点。点击第三下会把之前的路径清空。


## 地图文件的生成
地图文件是一个自定义格式的文件，可以通过工具由obj模型文件导出生成，而obj模型文件可以通过Unity的插件把场景文件导出，例如这个[monitor1394/ExportSceneToObj](https://github.com/monitor1394/ExportSceneToObj)github项目。

在本项目的`tools`目录下有一个gen_map_tools的工具，此工具可以将obj模型文件导出成适合本项目的地图文件格式。

这个工具的主要逻辑就是把obj模型的地图文件的外轮廓和内轮廓的多边形识别出来，并按照如下格式生成一个文本文件
> childsize，parentlength, parentpoints，[0，child1 length,child1 points, [0，child2 length, child2 points]]

> 内嵌多边形数量，外多边形点的数量，外多边形的点，[0，子多边形1点的数量，子多边形1的点，[0, 子多边形2点的数量，子多边形2的点]]

下面的截图是`gen_map_tools/map`目录下的两个地图的例子
其中红色的线为外轮廓，绿色的线为内轮廓。
![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20230113205512.png)
![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20230113203242.png)

![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20230113203022.png)

![image.png](https://github.com/sunxvming/navmesh/tree/master/img/20230113203058.png)

具体的说明和使用方法请参考`gen_map_tools`目录下的说明文档。

## 寻路涉及的主要算法

这些都可以在网上查的到，在俺的小demo中应该也很容易找的到的
- 平面多边形三角剖分算法(Delaunay剖分)
	- 多边形用什么数据结构来表示
	- 各种几何图形用什么数据结构来表示
	- 如何生成网格以及附带数据
	- 如何确定一个DT点
	- 如何遍历整个多边形来完成三角剖分
- 寻路算法
- 寻路拐点算法
- 判断两条线段是否相交
- 判断点是否在三角形中
- 判断点在向量的那一侧
- 已知三点求三点的夹角
  

## Delaunay剖分是啥？

Delaunay三角剖分其实并不是一种算法，而是一种三角剖分的标准，实现它有多种算法。它只是给出了一个“好的”三角网格的定义，它的优秀特性是空圆特性和最大化最小角特性，这两个特性避免了狭长三角形的产生，也使得Delaunay三角剖分应用广泛。

**空圆特性**其实就是对于两个共边的三角形，任意一个三角形的外接圆中都不能包含有另一个三角形的顶点，这种形式的剖分产生的最小角最大(比不满足空圆特性的最小角大)

  
## 项目主要类说明

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

## MMO游戏的常见的寻路逻辑
```
FindCross：向指定的方向寻路
FindPath：向指定的点寻路

客户端寻路
    角色寻路
        摇杆移动
            向摇杆方向FindCross
            主要点：摇杆移动每桢渲染，但是FindCross寻路的调用要增加个冷却时间，比如100毫秒，
                    要不然调用寻路的接口会太频繁
        屏幕点击某个位置点
            FindPath，寻路到指定位置
        释放技能(FindCross)
            寻路到目标点后释放技能
        自动任务(自动打怪、自动采集、自动跟随等)
            FindPath，点击需要执行的任务自动寻路到目标点
        
服务器端寻路    
    怪物寻路
        不存在攻击目标
            巡逻
                间隔固定时间在营地半径内进行一次巡逻
                    巡逻时会进行一次寻路(FindCross)
            Idle
                非巡逻时间idle
            
        存在攻击目标
            若在攻击范围内，则进行对攻击目标的一次寻路(FindPath)，随后释放相应的技能
        
    宠物寻路
        按照规则跟随着主人
```

## 参考链接

* [平面多边形域的快速约束Delaunay三角化](https://github.com/sunxvming/navmesh/blob/master/doc/%E5%B9%B3%E9%9D%A2%E5%A4%9A%E8%BE%B9%E5%BD%A2%E5%9F%9F%E7%9A%84%E5%BF%AB%E9%80%9F%E7%BA%A6%E6%9D%9FDelaunay%E4%B8%89%E8%A7%92%E5%8C%96.pdf)
* [Nav导航网格寻路](https://blog.csdn.net/ynnmnm/article/details/44833007)
* [深入理解游戏中寻路算法](https://my.oschina.net/u/1859679/blog/1486636)
* [Astar寻路算法-中文](https://github.com/sunxvming/navmesh/blob/master/doc/Astar%E5%AF%BB%E8%B7%AF%E7%AE%97%E6%B3%95-%E4%B8%AD%E6%96%87.doc)
* [NevMesh.Js你可以在Laya引擎中直接使用的AI寻路](http://ask.layabox.com/question/47899)
* [recastnavigation: Navigation-mesh Toolset for Games](https://github.com/recastnavigation/recastnavigation)