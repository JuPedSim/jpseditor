#!/bin/sh

RED='\033[0;31m'
NC='\033[0m' # No Color

#Version=0.8.4
CMD=jpseditor

#echo "INFO: running  dynlibbundler .."
#dylibbundler -od -b -x ../bin/${CMD}.app/Contents/MacOS/${CMD} -d ../bin/${CMD}.app/Contents/libs/

echo "INFO: running macdeployqt .."
macdeployqt ./qmake-build-release//${CMD}.app -dmg
