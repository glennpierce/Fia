#!/usr/bin/env python
# -*- coding: utf-8 -*-

# FreeImageAlgorithms wrapper inherits types from the FreeImage wrapper.
# Written by Glenn Pierce glennpierce@gmail.com

import sys
import ctypes as C
from warnings import warn

#Load the constants
from FreeImagePy.constants import *
import FreeImagePy.FreeImagePy as FI

import library

class FIAImage(FI.Image):
    
    """ 
    FreeImageAlgorithms class wrapper
    
    The method with the first line uppercase are internal library methods
    the other one are internal method for help the user
        
    @author: Glenn Pierce
    """
    
    #Enable the message output
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
        self.__lib = library.internlLibrary("..\\FreeImageAlgorithmsPy\\FreeImageAlgorithms")
     
        if self.__lib.getStatus()[0]:
            return 'Error: %s' % str(self.__lib.getStatus())
        
        #Functions list that now acutally wrap. The third value are the return
        #type, if it exists, or if I'm able to translate from C code :)
        self.__lstFunction = ( 
        
            #General funtions
            ('FreeImageAlgorithms_SetOutputMessage',    '@4'),
			('FreeImageAlgorithms_SetRainBowPalette',	'@4'),
            ('FreeImageAlgorithms_Histogram',           '@28'),
         
        )
        
        for function in self.__lstFunction:
            try:
                self.__lib.setBind(function)
            except AttributeError, ex:
                warn("Error on bind %s." % function[0], FunctionNotFound)

        self.funct_printErrors = self.MessageOutput(self.printErrors)
        self.__lib.SetOutputMessage(self.funct_printErrors)
    
    # ------------------ #
    #  General funtions  #
    # ------------------ #
    
    def setRainBowPalette(self):
        """ Set a rainbow palette for the bitmap.
        """
        return self.__lib.SetRainBowPalette(self.getBitmap())
        
#FreeImageAlgorithms_Histogram(FIBITMAP *src, double min, double max,
#							  int number_of_bins, unsigned long *hist);
                              
    def getHistogram(self, min, max, bins):
        "Get the histogram of a greylevel image"
        DW_array = DWORD * bins # type
        histo = DW_array()
            
        self.__lib.Histogram(self.getBitmap(), C.c_double(min), C.c_double(max), bins, C.byref(histo))
 
        return [int(x) for x in histo]
    
    
    