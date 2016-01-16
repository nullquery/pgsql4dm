#!/bin/bash
export CPLUS_INCLUDE_PATH=/usr/include/$(gcc -print-multiarch)

g++ -g -Wl,--no-undefined -std=c++11 -m32 -shared \
 -lpq \
 -o libpgsql4dm.so src/pgsqlcpp/{*.cpp,*.h} src/pgsql4dm/{*.cpp,*.h}

