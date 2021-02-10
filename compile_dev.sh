#!/bin/bash

if [ ! -d "build" ]; then
	echo "Creating build/ folder"
	mkdir build
fi

cd build
cmake ..
make
