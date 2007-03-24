MACRO(POST_BUILD_COPY target)

IF(WIN32)

ADD_CUSTOM_COMMAND(TARGET "${target}"
                   POST_BUILD
                   COMMAND move "\"$\(TargetPath\)\"" "\"$\(TargetDir\)..\"")

ADD_CUSTOM_COMMAND(TARGET "${target}"
                   POST_BUILD
                   COMMAND move "\"$\(TargetDir\)$\(TargetName\).lib\"" "\"$\(TargetDir\)..\"")

ENDIF(WIN32)

ENDMACRO(POST_BUILD_COPY)


# The name of our project is "FreeImageAlgorithms".  CMakeLists files in this project can
# refer to the root source directory of the project as ${FreeImageAlgorithms_SOURCE_DIR} and
# to the root binary directory of the project as ${FreeImageAlgorithms_BINARY_DIR}.
PROJECT(FreeImageAlgorithms)

SUBDIRS(src Tests)

SET(CMAKE_DEBUG_POSTFIX "_d")

# Make sure the compiler can find include files from our Hello library.
INCLUDE_DIRECTORIES(include Anti-Grain-Lite Tests)

# Look for the FreeImage library
FIND_LIBRARY(FreeImage NAMES FreeImage PATHS ${LIBRARY_OUTPUT_PATH})

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
