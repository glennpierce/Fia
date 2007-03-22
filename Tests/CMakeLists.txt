SET(FIA_TEST_SRCS AllTests.cpp
	     	  CuTest.cpp
	     	  FreeImageAlgorithms_ArithmeticTests.cpp
	     	  FreeImageAlgorithms_DrawingTests.cpp
	     	  FreeImageAlgorithms_IOTests.cpp
	     	  FreeImageAlgorithms_LogicTests.cpp
	     	  FreeImageAlgorithms_Testing.cpp
	     	  FreeImageAlgorithmsColourTests.cpp
	     	  FreeImageAlgorithmsConvolutionTests.cpp
	     	  FreeImageAlgorithmsFFTTests.cpp
	    	  FreeImageAlgorithmsLinearScale.cpp
	     	  FreeImageAlgorithmsMorphologyTests.cpp
	     	  FreeImageAlgorithmsParticleTests.cpp
	     	  FreeImageAlgorithmsStatisticTests.cpp
	     	  FreeImageAlgorithmsUtilityTests.cpp
)


ADD_EXECUTABLE (FreeImageAlgorithmsTests ${FIA_TEST_SRCS})

# Make sure the linker can find the Hello library once it is built.
LINK_DIRECTORIES (${FREEIMAGEALGORITHMS_BINARY_DIR})

# Link the executable to the Hello library.
TARGET_LINK_LIBRARIES (FreeImageAlgorithmsTests FreeImageAlgorithms FreeImage)

POST_BUILD_COPY(FreeImageAlgorithmsTests)