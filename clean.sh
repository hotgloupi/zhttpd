#!/bin/sh

find . -name CMakeFiles -exec rm -rfv {} \;
find . -name cmake_install.cmake -exec rm -rfv {} \;
find . -name CMakeCache.txt -exec rm -rfv {} \;
find . -name Makefile -exec rm -rfv {} \;
find . -name '*.so' -print -delete
find . -name '*~' -print -delete



