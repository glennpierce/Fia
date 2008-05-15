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

#New example file

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
   FIPYOld = FreeImagePy.freeimage()
   import FreeImagePy as FIPY
   F = FIPY.Image()
  except FreeImagePy.FreeImagePy_LibraryNotFound, ex:
   self.printMsg( "Error:\n %s " % str(ex) )
   raw_input("Press ok for exit...")
   return

  self.printMsg( "Freeimage status: %s %s\n" % F.status )

  self.printMsg( "Message: %s\n" % F.GetCopyrightMessage() )
  self.printMsg( "Version: %s\n" % F.GetVersion() )

  #File names path
  imagePath = 'freeimage.jpg'
  imagePathOutPng = 'out_freeimage.png'
  imagePathOutRotate1 = 'out_reeimageRotate1.png'
  imagePathOutRotate2 = 'out_freeimageRotate2.png'
  newImagePIL = "out_freeimageToPil.png"
  outMultiPage = "./out_outMultiPage.tif"
  newMultiPage = "./out_newMultiFromNewMethod.tif"
  
  #Load and save manually. On save you can pass me the type, otherwise
  #I try to understand what image type you want.
  #Example:
  # image.png -> file: image, type: png, out: image.png
  # image.tif -> file image, type: tiff, out: image.tiff (standard compression)
  # image.tiffg4 -> file image, type: tiff G4, out: image.tiff (with g4 compression)
  # image.jpeg -> file image, type: jpeg, out: image.jpeg (standard compression)
  # image.jpeggo -> file image, type: jpeg, out: image.jpeg (good compression)

  # See extToType for more info

  F.load(imagePath)
  F.save(imagePathOutPng)
  
  if FIPY.utils.USE_PIL:
      PIL = F.convertToPil()
      PIL.save(newImagePIL)
      self.printMsg("PIL found and image saved (with PIL) on %s" % newImagePIL)
  else:
      self.printMsg("No PIL found, skip PIL test")

  del F

  #Load, rotate and save
  F = FIPY.Image(imagePath)
  newI = F.rotate(RR(-360, 360))
  newI.save(imagePathOutRotate1)
  del newI, F

  F = FIPY.Image(imagePath)
  rotated = F.rotate(RR(-360, 360))
  rotated.save(imagePathOutRotate2)
  del F
  
  #Help (old) method for convert the image to a multipage image (Old method)
  self.printMsg( "Return from convert to mutlipage: %s \n" % \
        str(FIPYOld.convertToMultiPage((imagePath, imagePathOutRotate1, imagePathOutRotate2), outMultiPage, FreeImagePy.FIF_TIFF)) )

  F = FIPY.Image()
  F.new((1000,1000), multiBitmap=newMultiPage)
  F.appendPage(imagePath) #append from file name
  F.deletePage(0) #delete the first page that freeimage NEED to have for work with a multi-page
  F.appendPage(bitmap=rotated, insert=0) #and/or insert from bitmap loaded
  del rotated

  #and split the image into a lot of singles image...   (file to load, output format, output ext, filename start)
  self.printMsg( "Return from convert to single pages: %s \n" % \
        str( F.convertToPages('output_convertToPages.png') ) )

  #Load a file from a buffer
  f = open(imagePathOutPng, 'rb')
  F = FIPY.Image()
  F.fromBuffer(f.read())

  F.save("output_fromBuffer.png")
  F.exit()

  #Metadata
  F = FIPY.Image(imagePath)
  self.printMsg( "Metadata into image: %s" % F.metadata )
  del F

  #Close
  FIPYOld.DeInitialise()
  self.printMsg( "Time passed: %f sec\n" % time.clock() )

  raw_input("Do you like FreeImage? Press ok.")

mytest = goTest()
mytest.go()
mytest.exit()
