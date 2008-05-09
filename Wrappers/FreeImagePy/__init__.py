#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Project       FreeImagePy
# file name:    __init__.py
#
# $Id $
#
# DESCRIPTION
# FreeImage python bind
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

from FreeImagePy import *

__doc__ = "FreeImagePy, a freeimage python wrap"

_FI = freeimage()
__version__ = "FreeImagePy 1.3.0, FreeImage %s" % _FI.version
del _FI
