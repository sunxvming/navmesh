
import pygame
from math import pi
import main
import navmesh

SCREEN_WIDTH = 1200
SCREEN_HEIGHT = 800
#初始化
pygame.init()
# 设置主屏幕大小
size = (SCREEN_WIDTH, SCREEN_HEIGHT)
screen = pygame.display.set_mode(size)
#设置标题
pygame.display.set_caption("test")
# 设置一个控制主循环的变量
done = False
#创建时钟对象
clock = pygame.time.Clock()

vertices, triangles = main.get_obj_info('level1.obj','')
nav = navmesh.NavMash(vertices, triangles)
pols = nav.gen_navmesh()

while not done:
    # 设置游戏的fps,每秒60帧
    clock.tick(60)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True  # 若检测到关闭窗口，则将done置为True


    minx = pols[0].lt.x
    miny = pols[0].lt.y
    maxx = pols[0].rb.x
    maxy = pols[0].rb.y

    dx = maxx - minx
    dy = maxy - miny
    timex = SCREEN_WIDTH / dx / 1.1
    timey = SCREEN_HEIGHT / dy / 1.1


    for p in pols:
        tmp = p.get_coord_list()
        points = [[timex * (p[0] - minx), timey * (p[1] - miny)] for p in tmp]  #实际坐标转换为屏幕坐标
        pygame.draw.polygon(screen, (255, 0, 0), points, 2)
        for subp in p.sub_polygons:
            tmp = subp.get_coord_list()
            points = [[timex * (p[0] - minx), timey * (p[1] - miny)] for p in tmp]
            pygame.draw.polygon(screen, (0, 255, 0), points, 2)

    pygame.display.flip()
# 点击关闭，退出pygame程序
pygame.quit()