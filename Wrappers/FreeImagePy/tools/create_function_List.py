#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Project       FreeImagePy
# file name:    create_function_List.py
# Version:      1.1.0
#
# DESCRIPTION
# FreeImage python bind - convert suite
# This file help me to don't forgot to wrap some functions, and
# create for me a lot of code, automatically... :)
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

#Function example
#_FreeImage_AcquireMemory@12

import os, sys
functFile = "functList.txt"
if not os.path.exists(functFile):
 print "Error, I cant find file %s " % functFile
 sys.exit(1)

freeImageFile = os.path.join(os.path.normpath(os.path.join(os.path.dirname(sys.argv[0]), "../")), "FreeImagePy.py")
if not os.path.exists(freeImageFile):
 print "Error, I cant find file %s " % freeImageFile
 sys.exit(1)

f2 = open(freeImageFile, "r")

f = open(functFile, 'r')
f1 = open("out", "w")
funcList = [i.strip() for i in f.readlines() if i.strip() ]
sourceFile = ' '.join([i.strip() for i in f2.readlines() if i.strip() ])

f1.write("******************************\n\n" )
f1.write("Functions that not yet wrapped\n\n" )
f1.write("******************************\n\n" )

lstNotFound = list()
for i in funcList:
 if not i.startswith('_'):
  print "I cannot work with: %s" % i
  continue
 name, number = i[1:].split('@')
 if sourceFile.find(name) != -1: continue

 f1.write("('%s', '@%s')\n" % (name, number))
 lstNotFound.append(name)

f1.write("\n\n\n")

f1.write("*********************************\n\n" )
f1.write("Functions wrapped but not present\n\n" )
f1.write("*********************************\n\n" )

f1.write("\n\n\n")

for i in funcList:
 if not i.startswith('_'):
  #print "I cannot work with: %s" % i
  continue
 name, number = i[1:].split('@')
 if name in lstNotFound: continue
 strToFind = "def %s" % name.split("_")[1]
 if sourceFile.find(strToFind) != -1: continue
 f1.write('    %s(self, bitmap):\n        """\n        """\n        self.__ctrlColorUsed()\n        return self.__lib.%s(bitmap)\n\n' % (strToFind, strToFind.split(' ')[1]))
 #print strToFind

f.close()
f1.close()
f2.close()
