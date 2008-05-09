SET(FIA_SRCS FreeImageAlgorithms_Arithmetic.cpp
	     FreeImageAlgorithms_Border.cpp
	     FreeImageAlgorithms_Colour.cpp
	     FreeImageAlgorithms_Convolution.cpp
	     FreeImageAlgorithms_Convolution.txx
	     FreeImageAlgorithms_DistanceTransform.cpp
	     FreeImageAlgorithms_Drawing.cpp
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
	     agg.cpp
		 
		 # Make the headers appear in visual studio 
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Arithmetic.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Colour.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Convolution.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Drawing.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_FFT.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Filters.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_HBitmap.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_IO.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_LinearScale.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Logic.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Morphology.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Palettes.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Particle.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Statistics.h
		 ${FreeImageAlgorithms_SOURCE_DIR}/include/FreeImageAlgorithms_Utilities.h
)

IF (WIN32)
  SET(FIA_SRCS ${FIA_SRCS} FreeImageAlgorithms_HBitmap.cpp)
ENDIF (WIN32)

ADD_DEFINITIONS(-DFREEIMAGE_EXPORTS)

ADD_LIBRARY (freeimagealgorithms SHARED ${FIA_SRCS})

IF (UNIX)
  INSTALL(TARGETS freeimagealgorithms RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
ENDIF (UNIX)

# Link the executable to the FreeImage library.
TARGET_LINK_LIBRARIES (freeimagealgorithms ${FREEIMAGE_LIBRARY})
