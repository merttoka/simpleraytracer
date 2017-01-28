#!/bin/bash
#Run this in terminal
if [ ! -d build ]; then
  mkdir -p build;
fi

g++ tinyxml/tinyxml.cpp tinyxml/tinystr.cpp tinyxml/tinyxmlparser.cpp tinyxml/tinyxmlerror.cpp Timing/ctimer.cpp Matrix3x3.cpp Vector3D.cpp main.cpp -o build/raytracer.exe
cd build

if [ ! $1 ]; then
	echo "WARNING:    No filename provided, interpreating as \"out.bmp\""
	echo ''
	./raytracer out.bmp
else
	./raytracer $1
fi
exit 0