cmake_minimum_required(VERSION 2.6)
project (solarsystem)

if (WIN32)
	#find_library(PNG png)
	#find_library(SDL2 SDL2)
	SET(LIBPNG_LIBRARIES png z)
	SET(SDL2_LIBRARIES mingw32 SDL2main SDL2)
else (WIN32)
	#find_package(OpenGL REQUIRED)
	find_package(PkgConfig REQUIRED)
	#pkg_search_module(GLFW REQUIRED gles)
	#pkg_search_module(GLFW REQUIRED glfw3)
	pkg_search_module(GLEW REQUIRED glew)
	pkg_search_module(SDL2 REQUIRED sdl2)
	#pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
	pkg_search_module(LIBPNG REQUIRED libpng)
endif (WIN32)
if(USE_OPENGLES)
    pkg_search_module(GLESv2 REQUIRED glesv2)
    set(GL_LIBRARY ${GLESv2_LIBRARIES})
    add_definitions(-DUSE_OPENGLES)
    if(USE_HALFFLOAT)
        add_definitions(-DUSE_HALFFLOAT)
    endif(USE_HALFFLOAT)
else(USE_OPENGLES)
    find_package(OpenGL REQUIRED)
    set(GL_LIBRARY ${OPENGL_LIBRARY})
endif(USE_OPENGLES)


include_directories(
    .
    ${SDL2_INCLUDE_DIRS}
    ${GLESv2_INCLUDE_DIRS}
)

if(WIN32)
# Compile external dependencies
add_subdirectory (external)
include_directories(
	external/glew-1.9.0/include/
	external/glm-0.9.4.0/
	"C:\\MinGW\\include\\libpng16"
)
add_definitions(
  -DTW_STATIC
  -DTW_NO_LIB_PRAGMA
  -DTW_NO_DIRECT3D
  -DGLEW_STATIC
  -D_CRT_SECURE_NO_WARNINGS
)
set(GLEW_LIBRARIES GLEW_190)
endif(WIN32)

link_directories(
    ${SDL2_LIBRARY_DIRS}
)

set(HEADER_FILES 
    "src/config.hh"
    "src/constants.h"
    "src/common/texture.hpp"
    "src/common/vboindexer.hpp"
    "src/common/objloader.hpp"
    "src/common/text2D.hpp"
    "src/graphics/shader.hh"
    "src/graphics/model.hh"
    "src/graphics/mesh.hh"
    "src/graphics/circle.hh"
    "src/graphics/background.hh"
    #"src/graphics/text2Dprinter.hh"
    "src/objects/planet.hh"
    "src/objects/planetdata.hh"
    "src/objects/solarsystem.hh"
    )

set(SRC_FILES
    "src/main.cc"
    "src/common/texture.cpp"
    "src/common/vboindexer.cpp"
    "src/common/objloader.cpp"
    "src/common/text2D.cpp"
    "src/graphics/shader.cc"
    "src/graphics/model.cc"
    "src/graphics/mesh.cc"
    "src/graphics/circle.cc"
    "src/graphics/background.cc"
    #"src/graphics/text2Dprinter.cc"
    "src/objects/planet.cc"
    "src/objects/planetdata.cc"
    "src/objects/solarsystem.cc"
    )

message(STATUS ${SDL2_INCLUDE_DIRS})
#message(STATUS ${SDL2_TTF_INCLUDE_DIRS})
message(STATUS ${GLESv2_INCLUDE_DIRS})
message(STATUS ${LIBPNG_INCLUDE_DIRS})
message(STATUS ${SDL2_LIBRARIES})
#message(STATUS ${SDL2_TTF_LIBRARIES})
message(STATUS ${GL_LIBRARY})
message(STATUS ${LIBPNG_LIBRARIES})



SET(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++11 -Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -Wall -std=c++11 -pipe -DNDEBUG")
#SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})

add_executable(solarsystem
    ${SRC_FILES}
    ${HEADER_FILES}
)

target_link_libraries(solarsystem
    ${GL_LIBRARY}
    ${SDL2_LIBRARIES}
    ${LIBPNG_LIBRARIES}
    ${GLEW_LIBRARIES}
)
