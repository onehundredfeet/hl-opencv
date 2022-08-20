cmake_minimum_required(VERSION 3.10)
project (opencv C CXX )

set(APP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "Application directory")

set(OPENCV_DIR "${APP_DIR}/ext/opencv"
    CACHE PATH "Location of 'opencv'")

set(INSTALLED_DIR "installed/debug")
set(OPENCV_INC_DIR ${INSTALLED_DIR}/include/opencv4)
set(OPENCV_LIB_DIR ${INSTALLED_DIR}/lib)
set(OPENCV_LIB_3RD_DIR ${INSTALLED_DIR}/lib/opencv4/3rdparty)
set(CONFIG_NAME $<$<STREQUAL:$<CONFIG>,"">:"Default">$<$<NOT:$<STREQUAL:$<CONFIG>,"">>:$<CONFIG>>)

if (NOT HDLL_DESTINATION) 
    set(HDLL_DESTINATION "/usr/local/lib")
endif()

if (NOT HL_INCLUDE_DIR) 
    set(HL_INCLUDE_DIR "/usr/local/include")
endif()

if (NOT COMMON_LIB_DIR) 
    set(COMMON_LIB_DIR "/usr/local/lib")
endif()

# Enable Blend2D static build.

#### Project definitions
find_library(LIBOPENCVCORE NAMES opencv_core HINTS ${OPENCV_LIB_DIR} )
find_library(LIBOPENCVIMGPROC NAMES opencv_imgproc HINTS ${OPENCV_LIB_DIR} )
find_library(LIBOPENCVFEATURES2D NAMES opencv_features2d HINTS ${OPENCV_LIB_DIR} )
find_library(LIBOPENCVGAPI NAMES opencv_gapi HINTS ${OPENCV_LIB_DIR} )
find_library(LIBOPENCVCODEC NAMES opencv_imgcodecs HINTS ${OPENCV_LIB_DIR} )

find_library(LIBOPENCV_IPPIW NAMES ippiw HINTS ${OPENCV_LIB_3RD_DIR} )
find_library(LIBOPENCV_IPPICV NAMES ippicv HINTS ${OPENCV_LIB_3RD_DIR} )
find_library(LIBOPENCV_ITTNOTIFY NAMES ittnotify HINTS ${OPENCV_LIB_3RD_DIR} )
find_library(LIBOPENCV_ADE NAMES ade HINTS ${OPENCV_LIB_3RD_DIR} )
find_library(LIBOPENCV_PNG NAMES libpng HINTS ${OPENCV_LIB_3RD_DIR} )
find_library(LIBOPENCV_JPEG NAMES libjpeg-turbo HINTS ${OPENCV_LIB_3RD_DIR} )
find_library(LIBOPENCV_JPEG2L NAMES libopenjp2 HINTS ${OPENCV_LIB_3RD_DIR} )
find_library(LIBOPENCV_ZLIB NAMES zlib HINTS ${OPENCV_LIB_3RD_DIR} )
find_library(LIBOPENCV_EXR NAMES IlmImf HINTS ${OPENCV_LIB_3RD_DIR} )
find_library(LIBOPENCV_WEBP NAMES libwebp HINTS ${OPENCV_LIB_3RD_DIR} )
find_library(LIBOPENCV_TIFF NAMES libtiff HINTS ${OPENCV_LIB_3RD_DIR} )

find_library(LIB_ACCELERATE Accelerate)
find_library(LIB_OPENCL OpenCL)

add_library(opencv.hdll SHARED
#src/hl-idl-helpers.cpp
src/opencv.cpp
)


set_target_properties(opencv.hdll
PROPERTIES
PREFIX ""
OUTPUT_NAME opencv
SUFFIX .hdll
)

set_property(TARGET opencv.hdll PROPERTY CXX_STANDARD 17)

cmake_policy(SET CMP0015 NEW)

target_include_directories(opencv.hdll
PRIVATE
${HL_INCLUDE_DIR}
${OPENCV_INC_DIR}
)

if (UNIX)
    # Some special flags are needed for GNU GCC compiler
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC -fpermissive")
    #not sure why the ${HL_LIB_DIR} is necessary given the above.
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -shared  ")

    find_library(LIBHL NAMES hl HINTS ${HL_LIB_DIR} )
else()
    find_library(LIBHL NAMES libhl HINTS ${HL_LIB_DIR} )
endif (UNIX)

set_property (TARGET opencv.hdll APPEND_STRING PROPERTY COMPILE_FLAGS )
 
target_link_libraries(opencv.hdll ${LIBHL} ${LIBOPENCVIMGPROC} ${LIBOPENCVFEATURES2D} ${LIBOPENCVCODEC} ${LIBOPENCV_JPEG2L} ${LIBOPENCV_WEBP} ${LIBOPENCV_EXR} ${LIBOPENCV_TIFF} ${LIBOPENCVCORE} ${LIBOPENCV_PNG} ${LIBOPENCV_JPEG} ${LIBOPENCV_IPPIW} ${LIBOPENCV_IPPICV} ${LIBOPENCV_ITTNOTIFY} ${LIBOPENCVGAPI} ${LIBOPENCV_ADE} ${LIBOPENCV_ZLIB} ${LIB_ACCELERATE} ${LIB_OPENCL})

install(TARGETS opencv.hdll DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/installed/${CONFIG_NAME}")
