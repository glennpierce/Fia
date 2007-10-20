#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Project       FreeImagePy
# file name:    library.py
#
# $Id$

import ctypes as C
from FreeImagePy.constants import *
from ctypes.util import find_library

class internlLibrary(object):
    """ Class for the real library load
    """
    __shared_state = {}    
    def __init__(self, libraryName):
        """ Initialize the library and set the self.init value
            to error if an exception is raise
        """
        #Thanks to Alex Martelli and his
        #http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/66531
        self.__dict__ = self.__shared_state
        
        self.__init = (0, '')
        os.environ['PATH'] = os.environ['PATH'] + ';' + os.path.abspath(os.path.dirname(__file__))
        
        if sys.platform == 'win32':
            functForLoad = C.windll
        else:
            functForLoad = C.cdll
        
        if not libraryName: libraryName = "freeimagealgorithms"
        libName = find_library(libraryName)
        
        if not libName:
            raise FreeImagePy_LibraryNotFound, \
                "I cannot find the library at %s. Did you pass me a vaid path?" % libraryName
        try:
            self.lib = getattr(functForLoad, libName)
            if sys.platform != "win32": self.lib.FreeImage_Initialise(False)
        except Exception, ex:
            print "Exception on load dll %s :%s"  % (libraryName, ex)
            self.__init = (1, str(ex))
    
        #Set the status
        if self.__init[0] == 0:
            self.__init = (0, 'Library. Loaded at: %s - %s' % \
                              (str(self.lib), libraryName) )
            
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
    
