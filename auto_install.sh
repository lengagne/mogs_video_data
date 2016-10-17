#!/bin/bash

sudo apt-get update 

sudo apt-get install cmake g++ libboost-all-dev libcv-dev libhighgui-dev libcvaux-dev libopencv-gpu-dev qt4-dev-tools

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
sudo ldconfig
