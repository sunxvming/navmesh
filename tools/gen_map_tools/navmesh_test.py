
import navmesh
import unittest


class TestPoint(unittest.TestCase):

    def test_sub(self):
        p1 = navmesh.Point(6,6)
        p2 = navmesh.Point(2,2)
        self.assertEqual(p1.sub(p2), navmesh.Point(4,4))


class TestEdge(unittest.TestCase):

    def test_other(self):
        e1 = navmesh.Edge(1, 2)
        self.assertEqual(e1.other(1), 2)
        self.assertEqual(e1.other(2), 1)

    def test_edge_index(self):
        self.assertEqual(navmesh.Edge.edge_index(1,2), 1*navmesh.MAX_POINT + 2)
        self.assertEqual(navmesh.Edge.edge_index(2,1), 1*navmesh.MAX_POINT + 2)

    def test_make_edge(self):
        # print(navmesh.Edge.make_edge(1*navmesh.MAX_POINT + 2))
        # print(navmesh.Edge(1,2))
        self.assertEqual(navmesh.Edge.make_edge(1*navmesh.MAX_POINT + 2), navmesh.Edge(1,2))


class TestPolygon(unittest.TestCase):
    def test_init(self):
        points = [
            navmesh.Point(0,0), 
            navmesh.Point(0,1), 
            navmesh.Point(1,1),
            navmesh.Point(1,0),
        ]
        polygon = navmesh.Polygon(points)
        # print(polygon)
        self.assertEqual(polygon.points, points)
        self.assertEqual(polygon.clock, False)


        points = [
            navmesh.Point(1,0),
            navmesh.Point(1,1),
            navmesh.Point(0,1), 
            navmesh.Point(0,0), 
        ]
        polygon = navmesh.Polygon(points)
        self.assertEqual(polygon.points, points)
        self.assertEqual(polygon.clock, True)

    def test_contain(self):
        p1 = navmesh.Polygon([
            navmesh.Point(0,0), 
            navmesh.Point(0,1), 
            navmesh.Point(1,1),
            navmesh.Point(1,0),
        ])
        p2 = navmesh.Polygon([
            navmesh.Point(0.5,0.5), 
            navmesh.Point(0.5,0.6), 
            navmesh.Point(0.6,0.6),
            navmesh.Point(0.6,0.5),
        ])
        self.assertTrue(p1.contain(p2))

        p1 = navmesh.Polygon([
            navmesh.Point(0,0), 
            navmesh.Point(0,1), 
            navmesh.Point(1,1),
            navmesh.Point(1,0),
        ])
        p2 = navmesh.Polygon([
            navmesh.Point(0.5,0.5), 
            navmesh.Point(0.5,0.6), 
            navmesh.Point(0.6,0.6),
            navmesh.Point(0.6,0.5),
        ])
        self.assertFalse(p2.contain(p1))

    def test_contained(self):
        p1 = navmesh.Polygon([
            navmesh.Point(0,0), 
            navmesh.Point(0,1), 
            navmesh.Point(1,1),
            navmesh.Point(1,0),
        ])
        p2 = navmesh.Polygon([
            navmesh.Point(0.5,0.5), 
            navmesh.Point(0.5,0.6), 
            navmesh.Point(0.6,0.6),
            navmesh.Point(0.6,0.5),
        ])
        p3 = navmesh.Polygon([
            navmesh.Point(0.5,0.5), 
            navmesh.Point(0.5,0.6), 
            navmesh.Point(0.6,0.6),
            navmesh.Point(0.6,0.5),
        ])
        polygons = [p1, p2, p3]
        self.assertEqual(p1.contained(polygons), None)
        self.assertEqual(p2.contained(polygons), p1)
        self.assertEqual(p3.contained(polygons), p1)


    def test_to_list(self):
    
        points = [
            navmesh.Point(0,0), 
            navmesh.Point(0,1), 
            navmesh.Point(1,1),
            navmesh.Point(1,0),
            navmesh.Point(0,0), 
        ]
        polygon = navmesh.Polygon(points)
        self.assertEqual(polygon.clock, False)

        list = []
        polygon.to_list(True, list)
        self.assertEqual(list, [4, 1, 0, 1, 1, 0, 1, 0, 0])

        list = []
        polygon.to_list(False, list)
        self.assertEqual(list, [4, 0, 0, 0, 1,  1, 1, 1, 0])



class TestNavMesh(unittest.TestCase):
    def test_find_loopback(self):
        # 四边形套四边形的地图
        vertices = [
            [0,0,0],
            [3,0,0],
            [3,0,3],
            [0,0,3],
            [1,0,1],
            [2,0,1],
            [2,0,2],
            [1,0,2],
            [1.0001,0,2.0001],
        ]
        indices = [
            1,5,6,
            1,2,6,
            2,6,7,
            2,3,7,
            3,7,8,
            3,4,8,
            4,8,5,
            4,1,5,
        ]

        mesh = navmesh.NavMash(vertices, indices)
        # print(mesh)
        pols = mesh.find_loopback()
        # print(pols)
        pols = mesh.gen_navmesh()
        # print("pols:", pols)
if __name__ == '__main__':
    unittest.main()