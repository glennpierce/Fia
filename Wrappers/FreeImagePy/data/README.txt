################################################################
#                                                              #
#    Project       FreeImagePy                                 #
#    Version:      1.2.4                                       #
#                                                              #
#    DESCRIPTION                                               #
#    FreeImage python wrapper                                  #
#                                                              #
#    Developer:                                                #
#    Michele Petrazzo <michele.petrazzo@unipex.it>             #
#                                                              #
#    License:                                                  #
#         GNU General Public License (GPL)                     #
#         FreeImage Public License (FIPL)                      #
#         For more info see LICENSE.txt                        #
#                                                              #
#                                                              #
#    Copyright (C) 2005  Unipex s.r.l.,  All Rights Reserved.  #
#    Via Vittorio Veneto 83/A                                  #
#    33050 Gonars (UD) - Italy                                 #
#    tel. +39 0432 931511 - fax +39 0432 931378                #
#    www.unipex.it - michele.petrazzo@unipex.it                #
#                                                              #
################################################################

Welcome to FreeImagePy, FreeImage python wrapper

This library use python, ctypes and the FreeImage library (.dll, .so)
for work.

Features:
 - A lot of function bound, but not all, sorry!
 - Internal helper functions, like genericLoaded, loadAndSave, convertToMultipage
   and convertToSinglePage
 - Functions naming: internal FreeImage library has the functions's names with the first
   character uppercase. I leave it unchaged and add my internal functions with the first
   char lowercase.
 - Depth color control: some functions work only with some color depth, so I make
   a control if the bitmap that the developer pass me to work on has the right depth,
   unless, I raise WrongColorDepth
 - Status control: after instanced the library, call the GetStatus function and I'll
   return the libray status
 - Generic library path support: when you instance me, you can pass me the library path,
   unless I'll search for FreeImage.dll, or /usr/lib/libfreeimage.so.3 file.
 - Thanks to ctypes I'll work on all the O.S. where FreeImage and ctypes can be
   compiled (I think a lot)

Requirement:
 - Python, of course... :)
      (2.3.x, 2.4.x)
      http://www.python.org/

 - ctypes
      (0.9.9.x at least)
      http://starship.python.net/crew/theller/ctypes/

 - FreeImage into library form (.dll, .so)
      (3.7.x for win, and cvs for Unix, due to a bug)
      http://freeimage.sourceforge.net/
