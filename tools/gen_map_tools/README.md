## 依赖安装
此python转换工具依赖了pygame，使用前请先用一下命令进行依赖安装
```
pip install -r requirements.txt
```

## 使用说明
把`.obj`模型文件转换成自定义的地图文件
```
python .\main.py -i .\map\map1.obj -o .\map\map1.txt
```


用pygame把`.obj`模型文件的外轮廓和内轮廓画出来
```
python .\draw_obj.py -f .\map\map1.obj
```

