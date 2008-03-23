#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Project       FreeImagePy
# file name:    test/test_FI_PythonFile.py
# Version:      1.2.0
#
# DESCRIPTION
# FreeImage python bind - test suite
# Test error capture in FreeImagePy
#
# Developer:
# Lenard Lindstrom
#
# History :
# creation: 09/2005 - Lenard Lindstrom
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

__all__ = ['Test']

import unittest
import sys
import os.path
import FreeImagePy

if __name__ == '__main__':
    # No __file__ ?!?
    __file__ = sys.argv[0]
imagePath = os.path.join(os.path.dirname(__file__), 'freeimage.jpg')
imageWidth = 600
imageHeight = 100

class LoadTestCase(unittest.TestCase):
    """Test the loadFromFile method"""
    def setUp(self):
        self.freeimage = FreeImagePy.freeimage()
        exception_value = "My value"
        self.exception_value = exception_value
        class MyException(Exception):
            pass
        self.MyException = MyException
        class RootFile(object):
            def __init__(self):
                self.posn = 0
        class HasSeek(RootFile):
            def __init__(self):
                super(HasSeek, self).__init__()
            def seek(self, offset, whence):
                if whence == 0:
                    self.posn = offset
                elif whence == 1:
                    self.posn += offset
                else:
                    raise IOError, "Unrecognized whence value %d" % whence
        class HasTell(RootFile):
            def __init__(self):
                super(HasTell, self).__init__()
            def tell(self):
                return self.posn
        class HasRead(RootFile):
            def __init__(self):
                super(HasRead, self).__init__()
            def read(self, n):
                raise IOError, "Not implemented"
        class NoTell(HasSeek, HasRead):
            pass
        self.NoTell = NoTell
        class NoSeek(HasTell, HasRead):
            pass
        self.NoSeek = NoSeek
        class NoRead(HasSeek, HasTell):
            pass
        self.NoRead = NoRead
        class RaiseMyException(HasSeek, HasTell):
            def read(self, n):
                raise MyException, exception_value
        self.RaiseMyException = RaiseMyException
        class RaiseMemoryError(HasSeek, HasTell):
            def read(self, n):
                raise MemoryError, exception_value
        self.RaiseMemoryError = RaiseMemoryError
        class DummyFreeImage(object):
            def LoadFromHandle(self, type, io_p, handle, flags):
                io = io_p._obj
                io.tell_proc(None)
                io.seek_proc(None, 0, 0)
                io.read_proc(None, 0, 0, None)
                return 0
        self.dummy_freeimage = DummyFreeImage()

    def testException(self):
        # MemoryError
        try:
            self.freeimage.loadFromFile(self.RaiseMemoryError(),
                                        FreeImagePy.FIF_BMP)
        except MemoryError:
            typ, e, tb = sys.exc_info()
            s = str(e)
            while tb.tb_next is not None:
                tb = tb.tb_next
            cd = tb.tb_frame.f_code
            del typ, e, tb
            self.failUnless(s == self.exception_value,
                            "MemoryError value incorrect")
            self.failUnless(cd == FreeImagePy.IO.load.func_code,
                            "MemoryError traceback for wrong code block")
        except Exception, e:
            self.fail("Expecting %s; got %s" % (MemoryError, e.__class__))

        # Other exception
        try:
            self.freeimage.loadFromFile(self.RaiseMyException(),
                                        FreeImagePy.FIF_BMP)
        except self.MyException:
            typ, e, tb = sys.exc_info()
            s = str(e)
            while tb.tb_next is not None:
                tb = tb.tb_next
            cd = tb.tb_frame.f_code
            del typ, e, tb
            self.failUnless(s == self.exception_value,
                            "Non MemoryError value incorrect")
            self.failUnless(cd == self.RaiseMyException.read.func_code,
                            "Non MemoryError traceback for wrong code block")
        except Exception, e:
            self.fail("Expecting %s; got %s" % (self.MyException, e.__class__))

        # No read method
        try:
            self.freeimage.loadFromFile(self.NoRead(), FreeImagePy.FIF_JPEG)
            self.fail("No exception raised for missing read")
        except FreeImagePy.IOAttributeError, e:
            s = str(e)
            self.failUnless(s == "Unsupported file operation read",
                            "Incorrect error message returned for no read\n"
                            "'%s'\n" % s)
        except Exception, e:
            self.fail("No read: Expecting %s; got %s" % (self.MyException, e.__class__))

        # No seek method
        try:
            FreeImagePy.FileIO(self.dummy_freeimage, self.NoSeek()).load(0)
        except FreeImagePy.IOAttributeError, e:
            s = str(e)
            self.failUnless(s == "Unsupported file operation seek",
                            "Incorrect error message returned for no seek\n"
                            "'%s'\n" % s)
        except Exception, e:
            self.fail("No seek: Expecting %s; got %s" % (self.MyException, e.__class__))

        # No tell method
        try:
            FreeImagePy.FileIO(self.dummy_freeimage, self.NoTell()).load(0)
        except FreeImagePy.IOAttributeError, e:
            s = str(e)
            self.failUnless(s == "Unsupported file operation tell",
                            "Incorrect error message returned"
                            "for no tell\n'%s'\n" % s)
        except Exception, e:
            self.fail("No tell: Expecting %s; got %s" % (self.MyException, e.__class__))

        # No read for IO.getType
        try:
            self.freeimage.loadFromFile(self.NoRead())
            self.fail("No exception raised for IO.getType and missing tell")
        except FreeImagePy.IOAttributeError, e:
            s = str(e)
            self.failUnless(s == "Unsupported file operation read",
                            "Incorrect error message returned"
                            "by IO.getType for no read\n'%s'\n" % s)
        except Exception, e:
            raise
            self.fail("IO.getType and no read: Expecting %s; got %s" %
                      (self.MyException, e.__class__))

    def testRead(self):
        f = file(imagePath, 'rb')
        # Test loadFromFile with an FIF
        bitmap = self.freeimage.loadFromFile(f, FreeImagePy.FIF_JPEG)
        self.failUnless(bitmap != 0, "Image not loaded")
        try:
            self.failUnless(self.freeimage.GetWidth(bitmap) == imageWidth and
                            self.freeimage.GetHeight(bitmap) == imageHeight,
                            "Image loaded improperly")
        finally:
            self.freeimage.Unload(bitmap)
        # Test getType
        f.seek(0)
        bitmap = self.freeimage.loadFromFile(f)
        self.failUnless(bitmap != 0, "IO.getType: Image not loaded")
        try:
            self.failUnless(self.freeimage.GetWidth(bitmap) == imageWidth and
                            self.freeimage.GetHeight(bitmap) == imageHeight,
                            "IO.getType: Image loaded improperly")
        finally:
            self.freeimage.Unload(bitmap)

loadtestsuite = unittest.makeSuite(LoadTestCase, 'test')

class SaveTestCase(unittest.TestCase):
    """Test the saveToFile method"""
    def setUp(self):
        self.freeimage = FreeImagePy.freeimage()
        self.bitmap = self.freeimage.genericLoader(imagePath)
        if self.bitmap == 0:
            raise IOError, "Unable to read test file"
        exception_value = "My value"
        self.exception_value = exception_value
        class MyException(Exception):
            pass
        self.MyException = MyException
        class RootFile(object):
            def __init__(self):
                self.posn = 0
        class HasSeek(RootFile):
            def __init__(self):
                super(HasSeek, self).__init__()
            def seek(self, offset, whence):
                if whence == 0:
                    self.posn = offset
                elif whence == 1:
                    self.posn += offset
                else:
                    raise IOError, "Unrecognized whence value %d" % whence
        class HasTell(RootFile):
            def __init__(self):
                super(HasTell, self).__init__()
            def tell(self):
                return self.posn
        class HasRead(RootFile):
            def __init__(self):
                super(HasRead, self).__init__()
            def read(self, n):
                raise IOError, "Not implemented"
        class NoWrite(HasSeek, HasTell):
            pass
        self.NoWrite = NoWrite
        class RaiseMyException(HasSeek, HasTell):
            def write(self, s):
                raise MyException, exception_value
        self.RaiseMyException = RaiseMyException
        class RaiseMemoryError(HasSeek, HasTell):
            def write(self, s):
                raise MemoryError, exception_value
        self.RaiseMemoryError = RaiseMemoryError

    def tearDown(self):
        if self.bitmap != 0:
            self.freeimage.Unload(self.bitmap)
    
    def testExceptions(self):
        # MemoryError
        try:
            self.freeimage.saveToFile(FreeImagePy.FIF_BMP,
                                      self.bitmap,
                                      self.RaiseMemoryError())
        except MemoryError:
            typ, e, tb = sys.exc_info()
            s = str(e)
            while tb.tb_next is not None:
                tb = tb.tb_next
            cd = tb.tb_frame.f_code
            del typ, e, tb
            self.failUnless(s == self.exception_value,
                            "MemoryError value incorrect")
            self.failUnless(cd == FreeImagePy.IO.save.func_code,
                            "MemoryError traceback for wrong code block")
        except Exception, e:
            self.fail("Expecting %s; got %s" % (MemoryError, e.__class__))

        # Other exception
        try:
            self.freeimage.saveToFile(FreeImagePy.FIF_BMP,
                                      self.bitmap,
                                      self.RaiseMyException())
        except self.MyException:
            typ, e, tb = sys.exc_info()
            s = str(e)
            while tb.tb_next is not None:
                tb = tb.tb_next
            cd = tb.tb_frame.f_code
            del typ, e, tb
            self.failUnless(s == self.exception_value,
                            "Non MemoryError value incorrect")
            self.failUnless(cd == self.RaiseMyException.write.func_code,
                            "Non MemoryError traceback for wrong code block")
        except Exception, e:
            self.fail("Expecting %s; got %s" % (self.MyException, e.__class__))

        # No write method
        try:
            self.freeimage.saveToFile(FreeImagePy.FIF_BMP, self.bitmap, self.NoWrite())
            self.fail("No exception raised for missing write")
        except FreeImagePy.IOAttributeError, e:
            s = str(e)
            self.failUnless(s == "Unsupported file operation write",
                            "Incorrect error message returned for no write\n'%s'\n" % s)
        except Exception, e:
            self.fail("Expecting %s; got %s" % (FreeImagePy.IOAttributeError, e.__class__))

    def testWrite(self):
        import StringIO
        f = StringIO.StringIO()
        self.failUnless(self.freeimage.saveToFile(FreeImagePy.FIF_BMP,
                                                  self.bitmap,
                                                  f) == 1,
                        "Error while write file")
        self.failUnless(len(f.getvalue()) > 0,
                        "No image data written")
        # Assume working loadFromFile and IO.getType
        bitmap = self.freeimage.loadFromBuffer(f.getvalue())
        self.failUnless(bitmap != 0, "Unable to load image from StringIO object")
        try:
            self.failUnless(self.freeimage.GetWidth(bitmap) == imageWidth and
                            self.freeimage.GetHeight(bitmap) == imageHeight,
                            "Image saved by saveToFile loaded improperly")
        finally:
            self.freeimage.Unload(bitmap)

savetestsuite = unittest.makeSuite(SaveTestCase, 'test')

suite = unittest.TestSuite((loadtestsuite, savetestsuite,))

def Test():
    unittest.TextTestRunner().run(suite)

if __name__ == '__main__':
    Test()
