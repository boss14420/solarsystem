cmake_minimum_required(VERSION 2.6)
project (solarsystem)

#find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED gles)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)

if(USE_OPENGLES)
    pkg_search_module(GLESv2 REQUIRED glesv2)
    set(GL_LIBRARY ${GLESv2LIBRARIES})
    add_definitions(-DUSE_OPENGLES)
else(USE_OPENGLES)
    find_package(OpenGL REQUIRED)
    set(GL_LIBRARY ${OPENGL_LIBRARY})
endif(USE_OPENGLES)

include_directories(
    .
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)



SET(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++11 -Wall")
SET(CMAKE_CXX_FLAGS "-O3 -march=native -Wall -std=c++11 -pipe")
SET(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS})

add_executable(solarsystem
    main.cc
    config.hh
    planetdata.hh
    planetdata.cc
    model.hh
    model.cc
    planet.hh
    planet.cc
    solarsystem.hh
    solarsystem.cc
    common/shader.cpp
    common/shader.hpp
    common/texture.cpp
    common/texture.hpp
    common/objloader.cpp
    common/objloader.hpp
    common/vboindexer.cpp
    common/vboindexer.hpp
)

target_link_libraries(solarsystem
    ${GL_LIBRARY}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
)