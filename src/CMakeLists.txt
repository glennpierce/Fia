SET(FIA_SRCS FreeImageAlgorithms_Arithmetic.cpp
	     FreeImageAlgorithms_Border.cpp
	     FreeImageAlgorithms_Colour.cpp
	     FreeImageAlgorithms_Convolution.cpp
	     FreeImageAlgorithms_DistanceTransform.cpp
	     FreeImageAlgorithms_Drawing.cpp
	     FreeImageAlgorithms_Error.cpp
	     FreeImageAlgorithms_FFT.cpp
	     FreeImageAlgorithms_FillHole.cpp
	     FreeImageAlgorithms_Filters.cpp
	     FreeImageAlgorithms_FindImageMaxima.cpp
	     FreeImageAlgorithms_FloodFill.cpp
	     FreeImageAlgorithms_IO.cpp
	     FreeImageAlgorithms_LinearScale.cpp
	     FreeImageAlgorithms_Logic.cpp
	     FreeImageAlgorithms_MedianFilter.cpp
	     FreeImageAlgorithms_Morphology.cpp
	     FreeImageAlgorithms_Palettes.cpp
	     FreeImageAlgorithms_ParticleInfo.cpp
	     FreeImageAlgorithms_Statistics.cpp
	     FreeImageAlgorithms_Threshold.cpp
	     FreeImageAlgorithms_Utilities.cpp
	     FreeImageAlgorithms_ConvexHull.cpp
	     kiss_fft.c
	     kiss_fftnd.c
	     profile.c
	     ${FreeImageAlgorithms_SOURCE_DIR}/Anti-Grain-Lite/agg.cpp
)


IF (WIN32)
  SET(FIA_SRCS ${FIA_SRCS} FreeImageAlgorithms_HBitmap.cpp)
#ELSE (WIN32)
#  SET(FIA_SRCS ${FIA_SRCS} UnixSupport.c)
ENDIF (WIN32)

ADD_DEFINITIONS(-DFREEIMAGE_EXPORTS)

ADD_LIBRARY (freeimagealgorithms SHARED ${FIA_SRCS})

# Link the executable to the FreeImage library.
TARGET_LINK_LIBRARIES (freeimagealgorithms freeimage)

COPY_TARGET(freeimagealgorithms)
COPY_LIB(freeimagealgorithms)
