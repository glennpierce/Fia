# The name of our project is "FreeImageAlgorithms".  CMakeLists files in this project can
# refer to the root source directory of the project as ${FreeImageAlgorithms_SOURCE_DIR} and
# to the root binary directory of the project as ${FreeImageAlgorithms_BINARY_DIR}.
PROJECT(FreeImageAlgorithms)

SUBDIRS(src Tests)

SET(CMAKE_DEBUG_POSTFIX "_d")

# Make sure the compiler can find include files from our library.
INCLUDE_DIRECTORIES(include Tests)

# Look for the FreeImage library
FIND_LIBRARY(freeimage NAMES freeimage PATHS /usr/lib ${LIBRARY_OUTPUT_PATH})

SET (LIBRARY_OUTPUT_PATH ${FreeImageAlgorithms_BINARY_DIR}/bin)
SET (EXECUTABLE_OUTPUT_PATH ${FreeImageAlgorithms_BINARY_DIR}/bin)

CONFIGURE_FILE(${FreeImageAlgorithms_SOURCE_DIR}/Tests/Constants.h.template ${FreeImageAlgorithms_SOURCE_DIR}/Tests/Constants.h)

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH: " ${EXECUTABLE_OUTPUT_PATH} )

MESSAGE( STATUS "MSPROJECT_TEMPLATE_DIRECTORY: " ${MSPROJECT_TEMPLATE_DIRECTORY} )
