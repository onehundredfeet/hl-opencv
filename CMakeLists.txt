cmake_minimum_required(VERSION 3.10)
project (opencv C CXX )

set(APP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "Application directory")

set(OPENCV_DIR "${APP_DIR}/ext/opencv"
    CACHE PATH "Location of 'opencv'")

set(OPENCV_INC_DIR ${OPENCV_DIR}/lib.debug/include/opencv4)
set(OPENCV_LIB_DIR ${OPENCV_DIR}/lib.debug/lib)
set(OPENCV_LIB_3RD_DIR ${OPENCV_DIR}/lib.debug/lib/opencv4/3rdparty)

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
find_library(LIBOPENCV NAMES opencv_imgproc HINTS ${OPENCV_LIB_DIR} )

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
 
target_link_libraries(opencv.hdll ${LIBHL} ${LIBOPENCV})

install(TARGETS opencv.hdll DESTINATION ${HDLL_DESTINATION})
