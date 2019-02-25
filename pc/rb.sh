#!/bin/sh

cd build

#cmake -DCMAKE_TOOLCHAIN_FILE=../../toolchain.rpi.cmake ..

make clean

make

