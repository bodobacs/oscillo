cmake_minimum_required (VERSION 3.0)
project(sdlscope)

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${sdlscope_SOURCE_DIR}/cmake.modules")
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


# Set an output directory for our binaries
#set(BIN_DIR ${sdlscope_SOURCE_DIR}/bin)

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

#target_include_directories(ChickenShooter ${SDL2_INCLUDE_DIR})

include_directories(${SDL2_INCLUDE_DIR})
message(STATUS ${SDL2_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/mylibs)
include_directories(${CMAKE_SOURCE_DIR}/src)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# find CMakeLists.txt in subdirs
add_subdirectory(${CMAKE_SOURCE_DIR}/mylibs/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/)


