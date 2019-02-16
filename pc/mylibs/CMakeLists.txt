#mylibs
project(mylibs)

add_library(csimplog csimpLog.cpp)
target_link_libraries(csimplog csimpsdl)

add_library(csimpsdl csimpSDL.cpp)
target_link_libraries(csimpsdl ${SDL2_LIBRARY})
#${SDL2_LIBRARY})

add_library(csimptty csimpTTY.cpp)
target_link_libraries(csimptty)

add_library(liblog liblog.cpp)
target_link_libraries(liblog)

