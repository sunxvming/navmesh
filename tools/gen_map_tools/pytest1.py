import imp
import pygame
from math import pi
import navmesh

#初始化
pygame.init()
# 设置主屏幕大小
size = (500, 450)
screen = pygame.display.set_mode(size)
#设置标题
pygame.display.set_caption("pytest")
# 设置一个控制主循环的变量
done = False
#创建时钟对象
clock = pygame.time.Clock()

vertices = [
    [0,0,0],
    [300,0,0],
    [300,0,300],
    [0,0,300],
    [100,0,100],
    [200,0,100],
    [200,0,200],
    [100,0,200],
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

pols = mesh.gen_navmesh()


while not done:
    # 设置游戏的fps,每秒60帧
    clock.tick(60)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True  # 若检测到关闭窗口，则将done置为True

    for p in pols:
        pygame.draw.polygon(screen, (255, 0, 0), p.get_coord_list(), 2)
        for subp in p.sub_polygons:
            pygame.draw.polygon(screen, (0, 255, 0), subp.get_coord_list(), 2)

    pygame.display.flip()
# 点击关闭，退出pygame程序
pygame.quit()