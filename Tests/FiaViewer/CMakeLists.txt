project(FiaViewer) # the name of your project

cmake_minimum_required(VERSION 2.6.0)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project
FIND_PACKAGE(FreeImage REQUIRED)

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTGUI TRUE)
SET(QT_USE_QTOPENGL TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTHELP TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTASSISTANT TRUE)

INCLUDE_DIRECTORIES(${FiaViewer_BINARY_DIR} ${FREEIMAGE_INCLUDE_PATH} ../../include)

SET(SRCS FiaViewer.cpp
	 main.cpp
)

SET(MOCS FiaViewer.h
)

#SET(QRCS qtdemo.qrc)

SET(UIRS  FiaViewer.ui)

# The next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
INCLUDE(${QT_USE_FILE})

QT4_WRAP_UI(UI_OUT_SRCS ${UIRS})

# generate rules for building source files from the resources
#QT4_ADD_RESOURCES(QRCS_SRCS ${QRCS})

# generate rules for building source files that moc generates
QT4_WRAP_CPP(MOC_SRCS ${MOCS})

IF(QT4_INSTALLED)

# build sources, moc'd sources, and rcc'd sources
ADD_EXECUTABLE(FiaViewer ${SRCS} ${MOC_SRCS} ${QRCS_SRCS} ${UI_OUT_SRCS})

# Link the "qtproject" target against the Qt libraries.
# Which libraries exactly, is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable.
TARGET_LINK_LIBRARIES(FiaViewer ${QT_LIBRARIES} ${FREEIMAGE_LIBRARY} freeimagealgorithms)

ENDIF(QT4_INSTALLED)
