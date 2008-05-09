#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Project       FreeImagePy
# file name:    utils.py
#
# $Id $
#

# DESCRIPTION
# FreeImage python bind
#
# Developer:
# Michele Petrazzo <michele.petrazzo@unipex.it>
#
# History : (see history.txt for more info)
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

from FreeImagePy import *
import imp

USE_PIL, USE_GTK = True, USE_WX = True, True

try:
    imp.find_module("Image")
except ImportError:
    USE_PIL = False

try:
    imp.find_module("wx")
except ImportError:
    USE_WX = False

try:
    imp.find_module("gtk")
except ImportError:
    USE_GTK = False
    
if USE_PIL:
    def convertToPil(image):
        """
        """
        import Image as PIL
        cu = image.getBPP()
        if not cu in COL_To_PIL.keys():
            raise ValueError, "Color not supported in convertToPil, %s" % cu

        w,h = image.size
        pil = PIL.new(COL_To_PIL[cu], (w, h))
        pil.fromstring(image.getBuffer())
        del PIL
        return pil
        
else:
    def convertToPil(self, *args,**kw):
        print "PIL not installed, please install it and retry"
        return None

if USE_WX:
    def convertToWx(dib):
        import wx
        w,h = dib.size
        image = wx.EmptyImage(w,h)
        new_image = dib.clone()
        new_image.bpp = 24
        b = new_image.getBuffer()[: w*h*3]
        image.SetData( b )
        del wx
        return image
else:
    def convertToWx(self, *args,**kw):
        print "wx not installed, please install it and retry"
        return None
    
if USE_GTK:
    def convertToGdkPixbuf(dib):
        import gtk
        try:
            w,h = dib.size
            new_dib = dib.ConvertTo24Bits(dib.getBitmap())
            pitch = dib.GetPitch(new_dib)
            
            # We have to switch the order of the red and blue channels.
            # This is inefficent but not sure how else tou do it with compiling c code 
            red_channel = dib.GetChannel(new_dib, FICC_RED)
            blue_channel = dib.GetChannel(new_dib, FICC_BLUE)
            dib.SetChannel(new_dib, blue_channel, FICC_RED)
            dib.SetChannel(new_dib, red_channel, FICC_BLUE)
            
            # Allocate a raw buffer
            bits = C.create_string_buffer("\x00" * h * pitch)

            dib.ConvertToRawBits(C.byref(bits), new_dib, pitch, 24,
                FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, False)
         
            pixbuf = gtk.gdk.pixbuf_new_from_data(bits, gtk.gdk.COLORSPACE_RGB, False, 8, w, h, pitch)
            
        except ValueError, detail:
        
            print "pixbuf_new_from_data: %s" % (detail)
            
            return None
            
        del gtk
        return pixbuf      
else:
    def convertToGdkPixbuf(self, *args,**kw):
        print "Gtk not installed, please install it and retry"
        return None
    
def getParametersFromExt(fileName, FIF=None):
    # Try to understand the type from extension
    ext = None
    if "." in fileName and not FIF:
        ext = os.path.splitext(fileName)[1].replace(".", "")
        #control for the ambigous extensions
        if ext.startswith("tif") and not ext.startswith("tiff"):
            ext = "tiff" + ext[3:]
        elif ext.startswith("jpg"):
            ext = "jpeg" + ext[3:]
   
    if FIF == None and ext in extToType.keys():
        FIF, flags, ext = extToType[ext]
        fileName = os.path.splitext(fileName)[0] + ext
    elif FIF in extToType.keys():
        FIF, flags, ext = extToType[FIF]
        if not fileName.endswith(ext):
            fileName += ext
    else:
        keyType = filter(lambda x: extToType[x][0] == FIF, extToType)
        if keyType:
            FIF, flags, ext = extToType[ keyType[0] ]
        else:
            return None, None, "no_ext", fileName
    
    return FIF, ext, flags, fileName        
