cmake_minimum_required(VERSION 3.0)

project(navmesh)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})


if(WIN32)
        
    # 添加外部静态
    add_library(freeglut STATIC IMPORTED)

    # 判断编译系统是MSVC还是MinGW
    if(MSVC)
        message("Build on WIN32. Using MSVC compiler.")
        SET(GLUT_DIR ${ROOT_DIR}/libs/freeglut_msvc)
        set_property(TARGET freeglut PROPERTY IMPORTED_LOCATION ${GLUT_DIR}/lib/x64/freeglut.lib)
    elseif(MINGW)
        message("Build on WIN32. Using MinGW compiler.")
        SET(GLUT_DIR ${ROOT_DIR}/libs/freeglut_mingw)
        set_property(TARGET freeglut PROPERTY IMPORTED_LOCATION ${GLUT_DIR}/lib/x64/libfreeglut.a)
    else()
        message(FATAL_ERROR "Build on WIN32. Using an unknown compiler.")
    endif()



elseif(LINUX)
    message("Build on LINUX.")
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(GLUT REQUIRED)
    find_package(OpenGL REQUIRED)

else()
    message(FATAL_ERROR, "Unsupported OS")
endif()


# 解决vs SAFESEH报错
# set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO /NODEFAULTLIB:glut32.lib")

# 编译那些源码
file(GLOB SOURCES "src/*.cpp")

# 编译目标
add_executable(${PROJECT_NAME} ${SOURCES})


set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/bin)

if(WIN32)
    # 包含路径
    target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${ROOT_DIR}/libs/freeglut_msvc/include
    )

    target_link_libraries(${PROJECT_NAME} freeglut opengl32 glu32) # 添加freeglut.lib

    # 复制动态库到执行目录
    if(MSVC)
        file(COPY ${GLUT_DIR}/bin/x64/freeglut.dll
            DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release)
        file(COPY ${GLUT_DIR}/bin/x64/freeglut.dll
            DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug)
    elseif(MINGW)
        file(COPY ${GLUT_DIR}/bin/x64/freeglut.dll
            DESTINATION ${EXECUTABLE_OUTPUT_PATH})
    endif()

elseif(LINUX)
    target_link_libraries(${PROJECT_NAME} glut OpenGL GLU)

endif()
