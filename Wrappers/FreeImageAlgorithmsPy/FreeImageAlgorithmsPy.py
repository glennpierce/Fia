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
            
 
# A ctype void_p but I define a new type so I can compare on the type and take the approiate action as this is a out parameter
class c_double_out(C.c_void_p):
    pass
 
class c_int_out(C.c_void_p):
    pass
    
class c_long_out(C.c_void_p):
    pass
    
FIA_FUNCTION_LIST = {
    
    # Name, Return Type, (Param1 type, Param1 type2) ....... etc
    # Defaulr param of BITMAP gets translated to self.bitmap
    'setRainBowPalette' : ('FIA_SetRainBowPalette', C.c_int, (C.c_void_p,)), 
    'histEq' : ('FIA_HistEq', C.c_int, (C.c_void_p,)), 
    'getGreyLevelAverage' : ('FIA_GetGreyLevelAverage', C.c_double, (C.c_void_p,)), 
    'monoImageFindWhiteArea' : ('FIA_MonoImageFindWhiteArea', C.c_int, (C.c_void_p, c_int_out)), 
    'getHistogram' : ('FIA_Histogram', C.c_int, (C.c_void_p, C.c_double, C.c_double, C.c_int, c_long_out)), 
	}
	       
class FIAImage(FI.Image):
    
    """ 
    FreeImageAlgorithms class wrapper
    
    The method with the first line uppercase are internal library methods
    the other one are internal method for help the user
        
    @author: Glenn Pierce
    """
    
    __lib = LoadLibrary("freeimagealgorithms")
    
    # Enable the message output
    if sys.platform == 'win32':
        MessageOutput = C.WINFUNCTYPE(VOID, C.c_char_p)
    else:
        MessageOutput = C.CFUNCTYPE(VOID, C.c_char_p)

    def __init__(self, f=None):
        """
        Init method for the class
        
        @param libraryName: library path, otherwise I'll look for it into the
                            standard path
        @type libraryName: string
        """
        
        super(FIAImage, self).__init__(f, None)      
        self.initCalled = 0
    
    def wrappedFunction(self, name):
    
        func_props = FIA_FUNCTION_LIST.get(name)
        
        if func_props == None:
            raise AttributeError, attr
 
        function = getattr(FIAImage.__lib, func_props[0])    # Function name
        function.restype = func_props[1]            # return type
        function.argtypes = func_props[2]           # Argument types
 
        return function
    

    def clone(self):
        """ Clone the current bitmap
            @return: new FIAImage instance
        """ 
        F = FIAImage()
        F.loadFromBitmap(self.Clone(self.getBitmap()))
        return F

    def setRainBowPalette(self):
        """ Set a rainbow palette for the bitmap."""
        cFunction = self.wrappedFunction('setRainBowPalette')
        return cFunction(self.getBitmap())
       
    def histEq(self):
        """ Perform histogram equalisation and return a new FIAImage."""
        F = FIAImage()  
        cFunction = self.wrappedFunction('histEq')
        bitmap = cFunction(self.getBitmap())
        F.loadFromBitmap(bitmap)
        return F
       
       
    def monoImageFindWhiteArea(self):
        cFunction = self.wrappedFunction('monoImageFindWhiteArea')
        ret = C.c_int()
        err = cFunction(self.getBitmap(), C.byref(ret))
        return (err, ret.value)
       
    def getHistogram(self, min, max, bins):
        "Get the histogram of a greylevel image"
        DW_array = DWORD * bins # type
        red_histo = DW_array()
        green_histo = DW_array()
        blue_histo = DW_array()
              
        cFunction = self.wrappedFunction('getHistogram')
        bitmap = self.getBitmap()
                  
        if self.getBPP() >= 24 and self.GetImageType(bitmap) == FIT_BITMAP:
            cFunction(bitmap, C.c_byte(min),
                C.c_byte(max), bins, C.byref(red_histo),  C.byref(green_histo),
                C.byref(blue_histo))
            return ([int(x) for x in red_histo], [int(x) for x in green_histo], [int(x) for x in blue_histo])    
        else: 
            cFunction(bitmap, C.c_double(min), C.c_double(max), bins, C.byref(red_histo))
            return ([int(x) for x in red_histo])
    
        return None
    
    
    
