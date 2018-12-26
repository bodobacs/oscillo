cmake_minimum_required (VERSION 3.0)

# ******* SDL 1.2, OpenGL, GLU ********
# small frame for my projects
# CMakeLists.txt bőven üzenetes, hogy lehessen debuggolni

project (mysdl_projects)

##################### SDL #######################

INCLUDE(FindSDL)

if(${SDL_FOUND})
message("SDL lib found")
message(STATUS "SDL_VERSION_STRING: ${SDL_VERSION_STRING}")
message(STATUS "SDL_INCLUDE_DIR: ${SDL_INCLUDE_DIR}")
message(STATUS "SDL_LIBRARY: ${SDL_LIBRARY}")
message(STATUS "SDL_IMAGE_INCLUDE_DIRS: ${SDL_IMAGE_INCLUDE_DIRS}")
else()
message("SDL lib NOT found")
endif()

##################### OPENGL #######################

#not legacy opengl library
set(OpenGL_GL_PREFERENCE GLVND)
INCLUDE(FindOpenGL)

if(${OPENGL_FOUND})
message("OpenGL lib found")
endif()

if(${OPENGL_GLU_FOUND})
message("GLU lib found")
endif()

message(STATUS "OPENGL_INCLUDE_DIR: ${OPENGL_INCLUDE_DIR}")
message(STATUS "OPENGL_gl_LIBRARY: ${OPENGL_gl_LIBRARY}")
message(STATUS "OPENGL_glu_LIBRARY: ${OPENGL_glu_LIBRARY}")
message(STATUS "OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")

##################### test isdl class #######################

include_directories(mylibs/ ${SDL_INCLUDE_DIR} ${SDL_IMAGE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

add_library(isdl mylibs/isdl.cpp)
target_link_libraries(isdl ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${OPENGL_LIBRARIES})

add_library(liblog mylibs/liblog.cpp)

add_executable(test_isdllib test_apps/test_isdl.cpp)
target_link_libraries(test_isdllib isdl liblog)

#################### my LOG library test #####################

add_executable(test_liblog test_apps/test_liblog.cpp)
target_link_libraries(test_liblog liblog)

#################### read USB serial #####################

add_executable(test_binaryfromtty readUSBserial/binaryfromtty.c)

#################### sdlscopeclient ####################

#for rpi
#include_directories(${CMAKE_SOURCE_DIR}/../bcm2835-1.57/src/)
#link_directories(your/library/dir)
#target_link_libraries(test_blink ${CMAKE_SOURCE_DIR}/../libbcm2835/libbcm2835.a)

add_executable(test_csimpTTY test_apps/test_csimpTTY.cpp mylibs/csimpTTY.cpp)
