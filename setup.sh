#!/bin/sh

if [ -z "$1" ]
then
      CONFIG=Debug
else
      CONFIG=$1
fi

mkdir -p build/${CONFIG}
mkdir -p installed/${CONFIG}

mkdir -p build/${CONFIG}/opencv
pushd build/${CONFIG}/opencv
cmake -GNinja -DCMAKE_BUILD_TYPE=${CONFIG} -DCMAKE_INSTALL_PREFIX=../../../installed/${CONFIG} ../../../ext/opencv -DOPENCV_FORCE_3RDPARTY_BUILD=ON -DVIDEOIO_ENABLE_PLUGINS=OFF -DWITH_PROTOBUF=OFF -DWITH_QUIRC=OFF -DWITH_VTK=OFF -Dccitt=OFF -Dmdi=OFF -DWITH_IMGCODEC_SUNRASTER=OFF -DHIGHGUI_ENABLE_PLUGINS=OFF -DBUILD_JAVA=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_PROTOBUF=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTS=OFF -DBUILD_opencv_apps=OFF -DBUILD_opencv_calib3d=OFF -DBUILD_opencv_highgui=OFF -DBUILD_opencv_java_bindings_generator=OFF -DBUILD_opencv_js_bindings_generator=OFF -DBUILD_opencv_objc_bindings_generator=OFF -DBUILD_opencv_photo=OFF -DBUILD_opencv_python_bindings_generator=OFF -DBUILD_opencv_python_tests=OFF -DBUILD_opencv_stitching=OFF -DBUILD_opencv_video=OFF -DBUILD_opencv_videoio=OFF
ninja install
popd


mkdir -p build/${CONFIG}/hl-opencv
pushd build/${CONFIG}/hl-opencv
cmake -GNinja -DCMAKE_BUILD_TYPE=${CONFIG} ../../..
ninja install
popd

#BASEDIR=$(dirname $0)
#pushd ext/mirage/ext/oniguruma
#autoreconf -vfi 
#./configure  --prefix="${PWD}/../../../../installed.debug"
#make install
#popd



#mkdir -p build.debug/hl-mirage

#pushd build.debug/hl-mirage
#cmake -GNinja -DCMAKE_INSTALL_PREFIX=../../installed.debug ../..
#ninja install
#popd
