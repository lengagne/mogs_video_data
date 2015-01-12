#!/bin/bash

sudo apt-get install cmake g++ libboost-all-dev libcv-dev libhighgui-dev libcvaux-dev 

mkdir auto_install 

cd auto_install 

git clone  https://github.com/lengagne/mogs_tinyxml.git
cd mogs_tinyxml
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE=Release ..
make 
sudo make install 

cd ../../..
pwd

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install