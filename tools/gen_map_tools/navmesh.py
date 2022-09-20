
import math

def index_of_first(lst, pred):
    for i,v in enumerate(lst):
        if pred(v):
            return i
    return -1


MAX_POINT = 100000;

DIS_PRECISION = 1000

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def sub(self, point):   
        return Point(self.x - point.x, self.y - point.y)

    def __str__(self) -> str:
        return "[{},{}]".format(self.x, self.y)

    def __repr__(self) -> str:
        return self.__str__()

    def __eq__(self, o: object) -> bool:
        return self.x == o.x and self.y == o.y

class Edge:
    # p1, p2是点的索引
    def __init__(self, p1, p2):
        self.p1 = p1
        self.p2 = p2

    def __str__(self) -> str:
        return "[{},{}]".format(self.p1, self.p2)

    def __repr__(self) -> str:
        return self.__str__()

    def __eq__(self, o: object) -> bool:
        return self.p1 == o.p1 and self.p2 == o.p2

    # 输入点的索引，返回另一个点的索引
    def other(self, p):
        return self.p1 if self.p2 == p else self.p2
    
    # 生成edge的索引
    @staticmethod
    def edge_index(p1, p2):
        if p1 < p2:
            return p1 * MAX_POINT + p2
        return p2 * MAX_POINT + p1


    # 根据edgeindex生成Edge对象
    @staticmethod
    def make_edge(edgeindex):
        p1 = edgeindex // MAX_POINT;
        p2 = edgeindex % MAX_POINT;
        return Edge(p1, p2)




class Polygon:
    
    def __init__(self, points:list):
        self.points = self.merge( points )

        p0 = points[0];
        minx = p0.x;
        miny = p0.y;
        maxx = p0.x;
        maxy = p0.y;
        maxxn = 0;   # maxx的索引
        cnt = len(points) - 1

        for i in range(1,cnt):
            x = points[i].x
            y = points[i].y
            if (x < minx):
                minx = x
            if (x > maxx):
                maxx = x
                maxxn = i
            if (y < miny):
                miny = y
            if (y> maxy):
                maxy = y

        # 左上角为坐标原点，向下向右坐标为坐标轴的正方向
        self.lt = Point(minx, miny)   # left top
        self.rb = Point(maxx, maxy)   # right bottom

    
        # 判断点的集合是否是顺时针
        for i in range(cnt + 2):   # 最大到 cnt + 1
            if (i == cnt + 1):
                print("points:",self.points)
                raise Exception("Clock Detect Fail")

            n = maxxn + i;
            if (n > cnt):
                n -= (cnt+1);
            p = points[n];
            pre = points[cnt if n - 1 < 0 else n - 1];
            nxt = points[0 if n + 1 > cnt else n + 1];
            v1 = p.sub(pre);
            v2 = nxt.sub(p);
            dxy =  v1.x* v2.y - v2.x * v1.y;  # 二维向量叉乘判断是否顺时针
            if (dxy != 0):
                self.clock = dxy > 0;
                break;

        self.sub_polygons = [];


    def __str__(self) -> str:
        str="""
        Polygon:
            lt:{self.lt}    
            rb:{self.rb}
            clock:{self.clock}
            points:{self.points}
            sub polygons:{self.sub_polygons}
        """

        return str.format(self=self)

    def __repr__(self) -> str:
        return self.__str__()

    def get_coord_list(self):
        return [[p.x, p.y] for p in self.points]

    # 把特别靠近的顶点合并
    def merge(self, points):
        # todo 之后实现
        return points


    # 多边形是否包含另一个多边形p
    def contain(self, p:'Polygon'): 
        plt = p.lt;
        prb = p.rb;
        if (self.lt.x > plt.x or self.lt.y > plt.y):
            return False;
        if (self.rb.x < prb.x or self.rb.y <  prb.y):
            return False;
        return True;

    # 返回包围自己的多边形
    def contained(self, polygons:list['Polygon'] ): 
        for i in range(len(polygons)):
            polygon = polygons[i]
            if (polygon is not self and polygon.contain(self)):
                return polygon;
    
        return None


    def add_sub_polygon(self, p:'Polygon'): 
        self.sub_polygons.append(p);


    def to_list(self, clock, list ):
        change = self.clock != clock;

        list.append(len(self.points)-1);      
        if (change):
            for i in range(len(self.points) -2, -1, -1):
                x = self.points[i].x 
                y = self.points[i].y
                list.append(x);
                list.append(y)
        else:
            for i in range(len(self.points) - 1):
                x = self.points[i].x 
                y = self.points[i].y
                list.append(x);
                list.append(y);
        return True;


class NavMash:
    def __init__(self, vertices:list, indices:list):
        if len(vertices) > MAX_POINT:
            raise Exception("point count is too much")

        self.polygons = []
        self.indices = indices

        #===处理顶点和边的索引
        indices = [int(i) - 1 for i in indices]  # 索引从1开始，转换为从0开始
        vertices_tmp = []  
        for i in range(len(vertices)):
            vertices_tmp.append(Point(vertices[i][0], vertices[i][2]))


        # 去除相同的点
        points = []  
        adpts = {}
    
        for i in range(len(vertices_tmp)):
            pt = vertices_tmp[i]
            index = index_of_first(points,lambda p:math.floor(pt.x*DIS_PRECISION)/DIS_PRECISION == math.floor(p.x*DIS_PRECISION)/DIS_PRECISION 
                        and math.floor(pt.y*DIS_PRECISION)/DIS_PRECISION == math.floor(p.y*DIS_PRECISION)/DIS_PRECISION)

            if index > -1:
                adpts[i] = index
            else:
                adpts[i] = len(points)
                points.append(pt)
        indexs = []
        
        for i in range(len(indices)):
            n = indices[i]
            if n in adpts:
                n = adpts[n]
            indexs.append(n)

        self.vertices = points
        self.indices = indexs

    def __str__(self) -> str:
        str="""
        NavMash:
            vertices:{self.vertices}
            indices:{self.indices}
            polygons:{self.polygons}
        """

        return str.format(self=self)


    def find_loopback(self)->list[Polygon]:
        # 遍历所有三角的edge，找到所有没有被两个三角形给引用的边，用于找回环，分外回环和内回环
        edge2triangle_num = {};  # key是边的index，value是以该边为三角形的数量
        for i in range(0, len(self.indices), 3):
            p1 = self.indices[i];
            p2 = self.indices[i+1];
            p3 = self.indices[i+2];

            edge1 = Edge.edge_index(p1, p2);
            edge2 = Edge.edge_index(p2, p3);
            edge3 = Edge.edge_index(p3, p1);


            if (edge1 in edge2triangle_num):
                edge2triangle_num[edge1] = edge2triangle_num[edge1] + 1;
            else:
                edge2triangle_num[edge1] = 1;

            if (edge2 in edge2triangle_num):
                edge2triangle_num[edge2] = edge2triangle_num[edge2] + 1;
            else:
                edge2triangle_num[edge2] = 1;

            if (edge3 in edge2triangle_num):
                edge2triangle_num[edge3] = edge2triangle_num[edge3] + 1;
            else:
                edge2triangle_num[edge3] = 1;

        # print("edge2triangle_num:",edge2triangle_num)

        # 点所在边集合
        border = []  # 多边形边界，每个边界边只有一个三角形
        point2edges = {}   # {point_index:[border中边的index]} 点到边的映射,一个点可以映射到两个边
        for edge in edge2triangle_num:
            if (edge2triangle_num[edge] == 1):
                i = len(border)
                edge = Edge.make_edge(edge)   # 通过边的索引获得边的对象,边的对象包含两个点
                border.append(edge);
                
                p1 = edge.p1;
                p2 = edge.p2;

                if p1 not in point2edges:
                    point2edges[p1] = []
                point2edges[p1].append(i)
                if len(point2edges[p1]) > 2:
                    pass
                    # print("point2edges[p1] > 2, len is {}, p is {}, edge is {}".format(len(point2edges[p1]), p1, edge))


                if p2 not in point2edges:
                    point2edges[p2] = []
                point2edges[p2].append(i)
                if len(point2edges[p2]) > 2:
                    # print("point2edges[p2] > 2, len is {}, p is {}, edge is {}".format(len(point2edges[p2]), p2, edge))
                    pass



            elif edge2triangle_num[edge] >= 3:
                print("error: edge2triangle_num[edge] != 1")

        # print("border:", border)
        # print("point2edges:", point2edges)

        # 从任意边出发，找到回环，会有多个回环,有最外层的回环和内部包含的回环
        polygons = []  # 多边形集合
        visited = {}
        for i in range(0, len(border)):
            if(i not in visited):
                edge = border[i]
                p = edge.p2
                polygon = []   # 一个回环，内含所有回环的点，而非点的索引
                polygon.append(self.vertices[edge.p1])
                polygon.append(self.vertices[edge.p2])
                visited[i] = True

                while True:
                    edges = point2edges[p]
                    # 异常点判断
                    if len(edges) != 2:
                        print("error: len(edges) is {}, p is {}, edges is {}".format(len(edges), p, edges))
                        # break
                    e0 = edges[0]
                    e1 = edges[1]
                    
                    if len(edges) > 2:
                        e0 = edges[2]
                        e1 = edges[3] 
                    e = None
                    if e0 not in visited:
                        e = border[e0]
                        visited[e0] = True
                    elif e1 not in visited:
                        e = border[e1]
                        visited[e1] = True

                    # 转了一圈，回到起点
                    if e is None:
                        break

                    nxtp = e.other(p)
                    polygon.append(self.vertices[nxtp])
                    p = nxtp
                if len(polygon) >= 3:  
                    pol = Polygon(polygon)
                    polygons.append(pol)
                    

        return polygons

    def add_polygon(self,p:Polygon):
        self.polygons.append(p)        



    def gen_navmesh(self):
        self.polygons = []
        polygons = self.find_loopback()
        for i in range(len(polygons)):
            polygon = polygons[i];
            container = polygon.contained(polygons)
            if (container == None): # 外回环
                self.add_polygon(polygon)
            else: # 内回环，添加到外回环中
                container.add_sub_polygon(polygon)
        return self.polygons

    def get_coordinate(self)->list[float]:
        self.gen_navmesh()

        coords = []
        for i in range(len(self.polygons)):
            polygon = self.polygons[i]
            if len(polygon.sub_polygons) > 0:
                coords.append(len(polygon.sub_polygons))
                polygon.to_list(True,coords)

                for j in range(len(polygon.sub_polygons)):
                    sub_polygon = polygon.sub_polygons[j]
                    coords.append(0)
                    sub_polygon.to_list(False,coords)

            else:
                coords.append(0)
                polygon.to_list(True,coords)
        return coords

    def gen_navmesh_file(self,filepath):
        coords = self.get_coordinate()
        with open(filepath, 'w') as f:
            for i in range(len(coords)):
                f.write(str(coords[i]) + '\n')


