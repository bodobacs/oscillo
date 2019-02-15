#mylibs
project(mylibs)

add_library(csimplog csimpLog.cpp)
target_link_libraries(csimplog)

add_library(csimpsdl csimpSDL.cpp)
target_link_libraries(csimpsdl)

add_library(isdl mylibs/isdl.cpp)
target_link_libraries(isdl ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${OPENGL_LIBRARIES})

add_library(csimptty mylibs/csimpTTY.cpp)
target_link_libraries(csimptty )

add_library(liblog mylibs/liblog.cpp)
target_link_libraries(liblog )

