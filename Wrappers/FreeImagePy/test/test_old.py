#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Project       FreeImagePy
# file name:    test/test.py
# Version:      1.2.0
#
# DESCRIPTION
# FreeImage python bind - test suite
#
# Developer:
# Michele Petrazzo <michele.petrazzo@unipex.it>
#
# History :
# creation: 08/2005 - Michele Petrazzo
#
# License:
#         GNU General Public License (GPL)
#         FreeImage Public License (FIPL)
#         For more info see LICENSE.txt
#
# Copyright (C) 2005  Unipex s.r.l.,  All Rights Reserved.
# Via Vittorio Veneto 83/A
# 33050 Gonars (UD) - Italy
# tel. +39 0432 931511 - fax +39 0432 931378
# www.unipex.it - michele.petrazzo@unipex.it
#

#AAA---  Old example file. See test_V1.2 more a more pythonic example....

import time, os, sys
from random import randrange as RR
import FreeImagePy

time.clock()

class goTest:
 def __init__(self):
  self.f = open("output.txt", "w")
 
 def printMsg(self, msg):
  print msg
  self.f.write("%s \n" % str(msg))

 def exit(self):
  self.f.close()

 def go(self):
  try:
   FIPY = FreeImagePy.freeimage()
  except FreeImagePy.FreeImagePy_LibraryNotFound, ex:
   self.printMsg( "Error:\n %s " % str(ex) )
   raw_input("Press ok for exit...")
   return

  self.printMsg( "Freeimage status: %s %s\n" % FIPY.getStatus() )

  self.printMsg( "Message: %s\n" % FIPY.GetCopyrightMessage() )
  self.printMsg( "Version: %s\n" % FIPY.GetVersion() )

  #File names path
  imagePath = 'freeimage.jpg'
  imagePathOutPng = 'freeimage.png'
  imagePathOutRotate1 = 'freeimageRotate1.png'
  imagePathOutRotate2 = 'freeimageRotate2.png'

  #Load and save manually
  image = FIPY.genericLoader(imagePath)
  FIPY.Save(FreeImagePy.FIF_PNG, image, imagePathOutPng)
  FIPY.Unload(image)

  #Load, rotate and save, but not unload the image, because I want to use it after
  image = FIPY.genericLoader(imagePath) # call the generic loader
  image = FIPY.RotateClassic(image, RR(-360, 360)) # -> here I cannot pass close = 1
  FIPY.Save(FreeImagePy.FIF_PNG, image, imagePathOutRotate1)
  #FIPY.Unload(image)

  #Load, rotate and save and unload the image
  image = FIPY.RotateClassic(image, RR(-360, 360), close=1)  # -> here yes ...
  FIPY.Save(FreeImagePy.FIF_PNG, image, imagePathOutRotate2)
  FIPY.Unload(image)

  #Simple method for load and save the image in one step into current directory
  #I define some "standard" method, like jpeg, jpegfa, jpegac, tiff, tiffg3 and so on...
  #that has all that I need for save an image, like FIF type, extension, flags.
  #See FreeImage source at line 38
  self.printMsg( "Return from Load and save: %s \n" % FIPY.loadAndSave(imagePathOutRotate1, './out_from_LoadAndSave_', 'bmp') )

  #Help method for convert the image to a multipage image
  self.printMsg( "Return from convert to mutlipage: %s \n" % \
        str(FIPY.convertToMultiPage((imagePath, imagePathOutRotate1, imagePathOutRotate2), './out.tif', FreeImagePy.FIF_TIFF)) )

  #and split the image into a lot of singles image...   (file to load, output format, output ext, filename start)
  self.printMsg( "Return from convert to single pages: %s \n" % \
        str(FIPY.convertToSinglePages('./out.tif', FreeImagePy.FIF_PNG, '.png', 'outputFromSinglesPages')) )

  #Load a file from a buffer
  f = open(imagePathOutPng, 'rb')
  fromHandle = FIPY.loadFromBuffer(f.read())
  if fromHandle:
   self.printMsg("File loaded from handle, success")
   FIPY.Unload(fromHandle)
  else:
   self.printMsg("Problems on load a file from a buffer")

  FIPY.DeInitialise()
  self.printMsg( "Time passed: %f sec\n" % time.clock() )

  raw_input("Do you like FreeImage? Press ok.")

mytest = goTest()
mytest.go()
mytest.exit()
