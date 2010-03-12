#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Project       FreeImagePy
# file name:    library.py
#
# $Id$

import ctypes as C
import constants as CO
from ctypes.util import find_library
import os, sys
from warnings import warn
from funct_list import FUNCTION_LIST

class internlLibrary(object):
    """ Class for the real library load
    """
    __instance = None
    __already_called = False
    
    def __new__(cls, *args, **kwargs):
        if cls.__instance is None:
            cls.__instance = object.__new__(cls)
        return cls.__instance

    #Enable the message output
    if sys.platform == 'win32':
        MessageOutput = C.WINFUNCTYPE(CO.VOID, C.c_int, C.c_char_p)
    else:
        MessageOutput = C.CFUNCTYPE(CO.VOID, C.c_int, C.c_char_p)
    
    def __init__(self, libraryName=None):
        """ Initialize the library and set the self.init value
            to error if an exception is raise
        """
        #If I am already called, return
        if self.__already_called:
            return None
        
        self.__init = (0, '')
        pathAdd = ( os.path.abspath( os.path.dirname(__file__) ), "./" )
        os.environ['PATH'] += ';' + ';'.join(pathAdd)
        
        if sys.platform == 'win32':
            functForLoad = C.windll
        else:
            functForLoad = C.cdll
        
        if not libraryName:
            if sys.platform.startswith("darwin"): 
                raise CO.FreeImagePy_LibraryNotFound("On darwin systems you have to pass me the complete freeimage path!")
            else:
                libraryName = "freeimage"
        
        if sys.platform.startswith("darwin"): 
            #no need to find the library complete path since we except the complte one passed!
            libName = libraryName
        else:
            libName = find_library(libraryName)

        if not libName:
            msg = "I cannot find the library at %s. Did you pass me a vaid path?" % "%s (%s)" % (libraryName, str(os.environ['PATH']) ) 
            raise CO.FreeImagePy_LibraryNotFound(msg)
        
        try:
            if sys.platform.startswith("darwin"): 
                #on darwin load the lib with the normal LoadLibrary function.
                self.lib = functForLoad.LoadLibrary(libName)
            else:
                #on the other systems, get the library like an attribute of the load function (ctypes.windll or ctypes.cdll)
                self.lib = getattr(functForLoad, libName)
                
            if sys.platform != "win32": self.lib.FreeImage_Initialise(False)
            
        except Exception, ex:
            print "Exception on load dll %s :%s"  % (libraryName, ex)
            self.__init = (1, str(ex))
        
        
        #Set the status
        if self.__init[0] == 0:
            self.__init = (0, 'Library. Loaded at: %s - %s' % \
                              (str(self.lib), libraryName) )
        
        for function in FUNCTION_LIST:
            try:
                self.setBind(function)
            except AttributeError, ex:
                warn("Error on bind %s." % function[0], CO.FunctionNotFound)

        self.funct_printErrors = self.MessageOutput(self.showErrors)
        self.lib.FreeImage_SetOutputMessage(self.funct_printErrors)
    
    def showErrors(self, imageFormat, message):
        """ Print message on errors. Overwrite me if you want.
        """
        print 'Error returned. ', CO.FIFTotype[imageFormat], message
    
    def setBind(self, function):
        """ Call me when you want to set a new bind
        """
        restype = None
        name, add = function[0:2]
        if len(function) == 4:
            restype = function[3]
        
        #The real name
        nameToBind = name.split('_', 1)[1]
        
        #Control the platform where we are working
        if sys.platform == 'win32':
            functionName = getattr(self.lib, '_%s%s' % (name, add) ) 
        else:
            functionName = getattr(self.lib, name)
        
        setattr( self, nameToBind, functionName )
        
        if restype: functionName.restype = restype
        
    def getStatus(self):
        """ Return the current status
            Now I'm used only for cotrol the correct initialialize, but
            in the future...
        """
        return self.__init
    
