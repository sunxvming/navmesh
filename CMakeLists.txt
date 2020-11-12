cmake_minimum_required(VERSION 3.0)

project (navmesh) 

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})

# 解决vs SAFESEH报错
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO /NODEFAULTLIB:glut32.lib")	


# 编译那些源码
file(GLOB SOURCES "navmesh/*.cpp")


# 编译目标
add_executable(${PROJECT_NAME} ${SOURCES})

# 包含路径
target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${ROOT_DIR}/Include
)

# 添加外部静态
add_library(glut32 STATIC IMPORTED)
set_property(TARGET glut32 PROPERTY IMPORTED_LOCATION ${ROOT_DIR}/Library/glut32.lib)
target_link_libraries(${PROJECT_NAME} glut32 )  #添加glut32.lib

# 复制动态库到执行目录
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/bin)
file(COPY ${ROOT_DIR}/Library/glut32.dll
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release)
file(COPY ${ROOT_DIR}/Library/glut32.dll
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug)    