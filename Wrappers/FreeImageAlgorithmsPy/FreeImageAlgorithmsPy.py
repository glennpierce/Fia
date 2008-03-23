#!/usr/bin/env python
# -*- coding: utf-8 -*-

# FreeImageAlgorithms wrapper inherits types from the FreeImage wrapper.
# Written by Glenn Pierce glennpierce@gmail.com

import sys
import ctypes as C
from ctypes.util import find_library
from warnings import warn

#Load the constants
from FreeImagePy.constants import *
import FreeImagePy.FreeImagePy as FI

def LoadLibrary(libraryName):
    """ Initialize the library and set the self.init value
        to error if an exception is raise
    """
    # Add current directory
    os.environ['PATH'] = os.environ['PATH'] + ';' + os.path.abspath(os.path.dirname(__file__))
        
    if sys.platform == 'win32':
        functForLoad = C.windll
    else:
        functForLoad = C.cdll
        
    if not libraryName:
        libraryName = "freeimagealgorithms"
        
    libName = find_library(libraryName)
        
    if not libName:
        raise FreeImagePy_LibraryNotFound, \
        "I cannot find the library at %s. Did you pass me a vaid path?" % libraryName
        
    try:
        lib = getattr(functForLoad, libName)
        if sys.platform != "win32":
            lib.FreeImage_Initialise(False)
    except Exception, ex:
        print "Exception on load dll %s :%s"  % (libraryName, ex)
            
    return lib
            
            
class FIAImage(FI.Image):
    
    """ 
    FreeImageAlgorithms class wrapper
    
    The method with the first line uppercase are internal library methods
    the other one are internal method for help the user
        
    @author: Glenn Pierce
    """
    
    # Enable the message output
    if sys.platform == 'win32':
        MessageOutput = C.WINFUNCTYPE(VOID, C.c_char_p)
    else:
        MessageOutput = C.CFUNCTYPE(VOID, C.c_char_p)
    
    def printErrors(self, message):
        print 'Error returned. ', message
    
    
    
    def __init__(self, f=None):
        """
        Init method for the class
        
        @param libraryName: library path, otherwise I'll look for it into the
                            standard path
        @type libraryName: string
        """
        
        super(FIAImage, self).__init__(f, None)
        
        self.initCalled = 0
        self.__lib = LoadLibrary("freeimagealgorithms")
        self.funct_printErrors = self.MessageOutput(self.printErrors)
        self.__lib.FIA_SetOutputMessage(self.funct_printErrors)
    
#    def setRainBowPalette(self):
#        """ Set a rainbow palette for the bitmap.
#        """
#        return self.__lib.SetRainBowPalette(self.getBitmap())


    def getHistogram(self, min, max, bins):
        "Get the histogram of a greylevel image"
        DW_array = DWORD * bins # type
        red_histo = DW_array()
        green_histo = DW_array()
        blue_histo = DW_array()
                
        bitmap = self.getBitmap()
            
        if self.getBPP() >= 24 and self.GetImageType(bitmap) == FIT_BITMAP:
            self.__lib.FIA_RGBHistogram(bitmap, C.c_byte(min),
                C.c_byte(max), bins, C.byref(red_histo),  C.byref(green_histo),
                C.byref(blue_histo))
            return ([int(x) for x in red_histo], [int(x) for x in green_histo], [int(x) for x in blue_histo])    
        else: 
            self.__lib.FIA_Histogram(bitmap, C.c_double(min), C.c_double(max), bins, C.byref(red_histo))
            return ([int(x) for x in red_histo])
    
        return None
    
    
    
