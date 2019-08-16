#!/bin/sh

mkdir build

cd build

cmake ..

make

cp -rf ../html/error ./