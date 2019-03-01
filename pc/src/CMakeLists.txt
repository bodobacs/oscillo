#/src
project(scopeapps)


#################### test liblog #####################

add_executable(test_liblog test_liblog.cpp)
target_link_libraries(test_liblog liblog)

#################### read USB serial #####################

add_executable(test_binaryfromtty serialsample.c)

#################### sdlscopeclient ####################

#for rpi
#include_directories(${CMAKE_SOURCE_DIR}/../bcm2835-1.57/src/)
#link_directories(your/library/dir)
#target_link_libraries(test_blink ${CMAKE_SOURCE_DIR}/../libbcm2835/libbcm2835.a)


#################### test csimptty  ####################

add_executable(test_csimpTTY test_csimpTTY.cpp)
target_link_libraries(test_csimpTTY csimptty liblog)

#################### test 0to0buffer ####################

add_executable(test_0to0buffer test_0to0buffer.cpp)
target_link_libraries(test_0to0buffer csimptty liblog)

#################### test_0to0ncurses ####################

add_executable(test_0to0ncurses test_0to0ncurses.cpp)
target_link_libraries(test_0to0ncurses csimptty liblog ncurses)

#################### test sdl ####################

add_executable(testsdl testsdl.cpp)
target_link_libraries(testsdl csimptty csimplog csimpsdl)

#################### osc sdl a ####################

add_executable(oscsdla oscsdla.cpp)
target_link_libraries(oscsdla csimptty csimplog csimpsdl)

#################### testing quadtree library ####################

add_executable(testquadtree testquadtree.cpp)
target_link_libraries(testquadtree csimpgui csimplog csimpsdl)

#################### testing gui library ####################

add_executable(testgui testgui.cpp)
target_link_libraries(testgui csimpgui csimplog csimpsdl)
