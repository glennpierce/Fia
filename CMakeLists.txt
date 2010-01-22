cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

# The name of our project is "FreeImageAlgorithms".  CMakeLists files in this project can
# refer to the root source directory of the project as ${FreeImageAlgorithms_SOURCE_DIR} and
# to the root binary directory of the project as ${FreeImageAlgorithms_BINARY_DIR}.
PROJECT(FreeImageAlgorithms)

SUBDIRS(src Tests)

SET(CMAKE_DEBUG_POSTFIX "_d")

ADD_DEFINITIONS(-D GENERATE_DEBUG_IMAGES -D _CRT_SECURE_NO_WARNINGS)

# Look for the FreeImage library
FIND_PACKAGE(FreeImage REQUIRED) 

# Make sure the compiler can find include files from our library.
INCLUDE_DIRECTORIES(include src/agg/include Tests ${FREEIMAGE_INCLUDE_PATH})

SET (LIBRARY_OUTPUT_PATH ${FreeImageAlgorithms_BINARY_DIR}/bin)
SET (EXECUTABLE_OUTPUT_PATH ${FreeImageAlgorithms_BINARY_DIR}/bin)

CONFIGURE_FILE(${FreeImageAlgorithms_SOURCE_DIR}/Constants.h.template ${FreeImageAlgorithms_SOURCE_DIR}/include/Constants.h)

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

IF(WIN32)
SET(CMAKE_CXX_STANDARD_LIBRARIES "user32.lib gdi32.lib")
ENDIF(WIN32)

MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH: " ${EXECUTABLE_OUTPUT_PATH} )
