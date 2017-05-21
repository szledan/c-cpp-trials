#! /bin/bash

mkdir -p build
g++ -std=c++11 -pedantic -Wall -o build/prod-cons main.cpp -pthread
