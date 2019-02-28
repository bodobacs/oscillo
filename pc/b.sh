#!/bin/sh

cd build

#cmake -DCMAKE_TOOLCHAIN_FILE=../../toolchain.rpi.cmake ..

cmake -DCMAKE_BUILD_TYPE=Debug ../

#cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo
#cmake ../

make

