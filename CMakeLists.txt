cmake_minimum_required(VERSION 2.6)
project (solarsystem)

#find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED gles)
#pkg_search_module(GLFW REQUIRED glfw3)
#pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(SDL2 REQUIRED sdl2)

if(USE_OPENGLES)
    pkg_search_module(GLESv2 REQUIRED glesv2)
    set(GL_LIBRARY ${GLESv2_LIBRARIES})
    add_definitions(-DUSE_OPENGLES)
else(USE_OPENGLES)
    find_package(OpenGL REQUIRED)
    set(GL_LIBRARY ${OPENGL_LIBRARY})
endif(USE_OPENGLES)

include_directories(
    .
    ${SDL2_INCLUDE_DIRS}
    ${GLESv2_INCLUDE_DIRS}
)

link_directories(
    ${SDL2_LIBRARY_DIRS}
)

set(HEADER_FILES 
    "src/config.hh"
    "src/constants.h"
    "src/common/texture.hpp"
    "src/common/vboindexer.hpp"
    "src/common/objloader.hpp"
    "src/graphics/shader.hh"
    "src/graphics/model.hh"
    "src/graphics/mesh.hh"
    "src/graphics/circle.hh"
    "src/objects/planet.hh"
    "src/objects/planetdata.hh"
    "src/objects/solarsystem.hh"
    )

set(SRC_FILES
    "src/main.cc"
    "src/common/texture.cpp"
    "src/common/vboindexer.cpp"
    "src/common/objloader.cpp"
    "src/graphics/shader.cc"
    "src/graphics/model.cc"
    "src/graphics/mesh.cc"
    "src/graphics/circle.cc"
    "src/objects/planet.cc"
    "src/objects/planetdata.cc"
    "src/objects/solarsystem.cc"
    )

message(STATUS ${SDL2_INCLUDE_DIRS})
message(STATUS ${GLESv2_INCLUDE_DIRS})
message(STATUS ${SDL2_LIBRARIES})
message(STATUS ${GL_LIBRARY})



SET(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++11 -Wall")
SET(CMAKE_CXX_FLAGS "-O3 -march=native -Wall -std=c++11 -pipe")
SET(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS})

add_executable(solarsystem
    ${SRC_FILES}
    ${HEADER_FILES}
)

target_link_libraries(solarsystem
    ${GL_LIBRARY}
    ${SDL2_LIBRARIES}
    ${GLFW_LIBRARIES}
)
