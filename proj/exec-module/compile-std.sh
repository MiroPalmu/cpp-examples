#!/usr/bin/sh

CXX="/home/miro/data/scratch/gcc-git/install/bin/g++"

"$CXX" -std=c++26 -fmodules -fsearch-include-path -c bits/std.cc
