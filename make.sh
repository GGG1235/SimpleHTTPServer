#!/bin/sh

mkdir build

cd build

cmake ..

make

cd ..

cp -rf ./html/error ./