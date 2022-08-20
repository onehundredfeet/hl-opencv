#!/bin/sh

if [ -z "$1" ]
then
      CONFIG=Debug
else
      CONFIG=$1
fi

pushd build/${CONFIG}/opencv
ninja install
popd


pushd build/${CONFIG}/hl-opencv
ninja install
popd
