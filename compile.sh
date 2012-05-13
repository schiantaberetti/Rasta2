#!/bin/bash

#recompile libfeat.a
rm libfeat-src/*.o
rm lib/libfeat.a
cd libfeat-scr/
make -C libfeat-src

#compile the program
make
