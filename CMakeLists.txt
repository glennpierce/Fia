MACRO(SET_LIBRARY_OUTPUTS target output)

IF(UNIX)

SET(LIBRARY_OUTPUT_PATH "${LIBRARY_OUTPUT_PATH}")

ELSE(UNIX)

ADD_CUSTOM_COMMAND(TARGET "${target}"
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND}
                   ARGS -E copy "${output}/$\(OUTDIR\)/$\(TargetName\).dll" "${output}")

ADD_CUSTOM_COMMAND(TARGET "${target}"
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND}
                   ARGS -E copy "${output}/$\(OUTDIR\)/$\(TargetName\).lib" "${output}")
ENDIF(UNIX)

ENDMACRO(SET_LIBRARY_OUTPUTS)


# The name of our project is "FreeImageAlgorithms".  CMakeLists files in this project can
# refer to the root source directory of the project as ${FreeImageAlgorithms_SOURCE_DIR} and
# to the root binary directory of the project as ${FreeImageAlgorithms_BINARY_DIR}.
PROJECT(FreeImageAlgorithms)

SUBDIRS(src Tests)

SET(CMAKE_DEBUG_POSTFIX "_d")

# Make sure the compiler can find include files from our Hello library.
INCLUDE_DIRECTORIES(include Anti-Grain-Lite Tests)

# Look for the FreeImage library
FIND_LIBRARY(FreeImage NAMES FreeImage PATHS ${CMAKE_CURRENT_SOURCE_DIR}/libs NO_DEFAULT_PATH)

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libs)