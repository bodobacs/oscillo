cmake_minimum_required (VERSION 3.0)
project(discoveringsdl2)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${discoveringsdl2_SOURCE_DIR}/cmake.modules")
# Set an output directory for our binaries
set(BIN_DIR ${discoveringsdl2_SOURCE_DIR}/bin)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

# Look up SDL2 and add the include directory to our include path
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/mylibs)
include_directories(${CMAKE_SOURCE_DIR}/src)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# find CMakeLists.txt in subdirs
add_subdirectory(${CMAKE_SOURCE_DIR}/mylibs/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/)
add_subdirectory(${CMAKE_SOURCE_DIR}/testapps/)




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

add_library(csimptty mylibs/csimpTTY.cpp)

add_executable(test_0to0buffer test_apps/test_0to0buffer.cpp)
target_link_libraries(test_0to0buffer csimptty liblog)


##test_0to0ncurses

add_executable(test_0to0ncurses test_apps/test_0to0ncurses.cpp)
target_link_libraries(test_0to0ncurses csimptty liblog ncurses)

