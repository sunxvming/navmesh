cmake_minimum_required(VERSION 3.0)

project (navmesh) 

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})

# 定义freeglut库目录
SET(GLUT_DIR ${ROOT_DIR}/libs/freeglut_msvc)

# 解决vs SAFESEH报错
# set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO /NODEFAULTLIB:glut32.lib")	


# 编译那些源码
file(GLOB SOURCES "src/*.cpp")


# 编译目标
add_executable(${PROJECT_NAME} ${SOURCES})

# 包含路径
target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${ROOT_DIR}/libs/freeglut_msvc/include
)

# 添加外部静态
add_library(freeglut STATIC IMPORTED)
set_property(TARGET freeglut PROPERTY IMPORTED_LOCATION ${GLUT_DIR}/lib/freeglut.lib)
target_link_libraries(${PROJECT_NAME} freeglut )  #添加freeglut.lib

# 复制动态库到执行目录
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/bin)
file(COPY ${GLUT_DIR}/bin/freeglut.dll
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release)
file(COPY ${GLUT_DIR}/bin/freeglut.dll
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug)    