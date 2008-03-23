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
        
    return libName
            
            
class FIAImage(FI.Image):
    
    """ 
    FreeImageAlgorithms class wrapper
    
    The method with the first line uppercase are internal library methods
    the other one are internal method for help the user
        
    @author: Glenn Pierce
    """
    
    # Enable the message output
    if sys.platform == 'win32':
        MessageOutput = C.WINFUNCTYPE(VOID, C.c_int, C.c_char_p)
    else:
        MessageOutput = C.CFUNCTYPE(VOID, C.c_int, C.c_char_p)
    
    def printErrors(self, imageFormat, message):
        print 'Error returned. ', FIFTotype[imageFormat], message
    
    
    
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
     
        
        #Functions list that now acutally wrap. The third value are the return
        #type, if it exists, or if I'm able to translate from C code :)
#        self.__lstFunction = ( 
#        
#            #General funtions
#            ('FreeImageAlgorithms_SetOutputMessage',    '@4'),
#			('FreeImageAlgorithms_SetRainBowPalette',	'@4'),
#            ('FreeImageAlgorithms_Histogram',           '@28'),
#         
#        )
        
#        for function in self.__lstFunction:
#            try:
#                self.__lib.setBind(function)
#            except AttributeError, ex:
#                warn("Error on bind %s." % function[0], FunctionNotFound)

#        self.funct_printErrors = self.MessageOutput(self.printErrors)
#        self.__lib.SetOutputMessage(self.funct_printErrors)
    
    # ------------------ #
    #  General funtions  #
    # ------------------ #
    
#    def setRainBowPalette(self):
#        """ Set a rainbow palette for the bitmap.
#        """
#        return self.__lib.SetRainBowPalette(self.getBitmap())


#    def getHistogram(self, min, max, bins):
#        "Get the histogram of a greylevel image"
#        DW_array = DWORD * bins # type
#        histo = DW_array()
            
#        self.__lib.Histogram(self.getBitmap(), C.c_double(min), C.c_double(max), bins, C.byref(histo))
 
#        return [int(x) for x in histo]
    
    
    
