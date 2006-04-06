/*
 * libics. ICS version 2.0 and 1.0 reading and writing.
 *
 * Copyright (C) 2000-2002          Cris L. Luengo Hendriks
 *                                  Pattern Recognition Group
 *                                  Delft University of Technology
 *                                  The Netherlands
 * mailto: cris@ph.tn.tudelft.nl
 *
 * Large chunks of this library written by
 *    Bert Gijsbers
 *    Dr. Hans T.M. van der Voort
 * And also Damir Sudar, Geert van Kempen, Jan Jitze Krol,
 * Chiel Baarslag and Fons Laan.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/*
 * FILE : libics.h
 *
 * This is the main include file, and the only file you need to include in your
 * source code if you use the top-level functions in this library.
 *
 * Last change: November 4, 2002
 */

#ifndef LIBICS_H
#define LIBICS_H

#ifdef __cplusplus
extern "C" {
#endif

#define ICSLIB_VERSION "1.3"

#if defined(__WIN32__) && !defined(WIN32)
#define WIN32
#endif

#ifdef WIN32
/*** Windows Specifics ***/

#define MAXPATHLEN 260 /* This is the value found in windef.h, which I don't
                          want to include here. */

#ifdef BUILD_ICSLIB
#define ICSEXPORT __declspec(dllexport)
#else
#ifdef USE_ICSLIB_DLL
#define ICSEXPORT __declspec(dllimport)
#else
#define ICSEXPORT
#endif
#endif

/*** End of Windows Specifics ***/
#else
/*** UNIX Specifics ***/

#include <sys/param.h> /* Definition for MAXPATHLEN */
#define ICSEXPORT

/*** End of UNIX Specifics ***/
#endif

/* For the moment the largest imel is a double complex of 16 bytes: */
#define ICS_MAX_IMEL_SIZE 16

/* These determine the sizes of static arrays and strings: */
#define ICS_MAXDIM 5         /* number of allowed dimensions in the image */
#define ICS_MAX_LAMBDA 16    /* number of allowed sensor channels */
#define ICS_STRLEN_PARAM 10  /* length of a parameter string */
#define ICS_STRLEN_TOKEN 20  /* length of a token string */
#define ICS_STRLEN_OTHER 128 /* length of other strings */
#define ICS_LINE_LENGTH 256  /* the maximum length of each of the lines in the .ics file. */

/*
 * These are the known data types for imels. If you use another type,
 * you can't use the top-level functions:
 */
typedef enum {
    Ics_unknown = 0,
    Ics_uint8,          /* integer, unsigned,  8 bpp */
    Ics_sint8,          /* integer, signed,    8 bpp */
    Ics_uint16,         /* integer, unsigned, 16 bpp */
    Ics_sint16,         /* integer, signed,   16 bpp */
    Ics_uint32,         /* integer, unsigned, 32 bpp */
    Ics_sint32,         /* integer, signed,   32 bpp */
    Ics_real32,         /* real,    signed,   32 bpp */
    Ics_real64,         /* real,    signed,   64 bpp */
    Ics_complex32,      /* complex, signed, 2*32 bpp */
    Ics_complex64       /* complex, signed, 2*64 bpp */
} Ics_DataType;

/*
 * These are the compression methods supported by this library:
 */
typedef enum {
    IcsCompr_uncompressed = 0, /* No compression */
    IcsCompr_compress,         /* Using Zlib (ICS_ZLIB must be defined) */
    IcsCompr_gzip              /* Using Zlib too (ICS_ZLIB must be defined) */
} Ics_Compression;

/*
 * These are structures that define the image representation. They are
 * only used inside the ICS data structure:
 */
typedef struct {
    size_t Size;                               /* Number of imels in this dimension */
    double Origin;                             /* Position of first imel */
    double Scale;                              /* Distance between imels */
    char Order[ICS_STRLEN_PARAM];              /* Order of this dimension */
    char Label[ICS_STRLEN_TOKEN];              /* Label for this dimension */
    char Unit[ICS_STRLEN_TOKEN];               /* Units for Origin and Scale */
} Ics_DataRepresentation;

typedef struct {
    Ics_DataType DataType;                     /* Numeric representation for the imels */
    size_t SigBits;                            /* Number of significant bits */
    double Origin;                             /* Offset for imel values */
    double Scale;                              /* Scaling for imel values */
    char Unit[ICS_STRLEN_TOKEN];               /* Units for Origin and Scale */
    /* Order is always "bits", Label is always "intensity" */
} Ics_ImelRepresentation;

/*
 * This is the data structure that holds all the information in
 *  the ICS file:
 */
typedef struct _ICS {
    int Version;                               /* ICS version: 1 for v.1.0, 2 for v.2.0 */
    int Reading;                               /* 1 if open to read, 0 if open to
                                                  write. Used by top-level only */
    const void* Data;                          /* Pointer to the data to write */
    size_t DataLength;                         /* Size of the data buffer */
    char Filename[MAXPATHLEN];                 /* '.ics' filename (including path) */
    int Dimensions;                            /* Number of elements in Dim */
    Ics_DataRepresentation Dim[ICS_MAXDIM];    /* Image representaion */
    Ics_ImelRepresentation Imel;               /* Imel representation */
    char Coord[ICS_STRLEN_TOKEN];              /* Coordinate system used */
    Ics_Compression Compression;               /* Compression technique used */
    int CompLevel;                             /* Parameter for the compression */
    int ByteOrder[ICS_MAX_IMEL_SIZE];          /* Byte  storage order */
    char* History;                             /* Some extra ICS information (history
                                                  and stuff) */
    size_t HistorySize;                        /* Size of the buffer allocated for
                                                  the history stuff */

    /* To read the data in blocks we need this: */
    void* BlockRead;                           /* Contains the status of the data file */

    /* New version 2.0 parameters: */
    char SrcFile[MAXPATHLEN];                  /* Source file name */
    size_t SrcOffset;                          /* Offset into source file */

    /* Special microscopic parameters: */
    int WriteSensor;                           /* Set to 1 if the next params are needed */
    char Type[ICS_STRLEN_TOKEN];               /* sensor type */
    char Model[ICS_STRLEN_OTHER];              /* model or make */
    int SensorChannels;                        /* Number of channels */
    double PinholeRadius[ICS_MAX_LAMBDA];      /* Backprojected microns */
    double LambdaEx[ICS_MAX_LAMBDA];           /* Excitation wavelength in nanometers */
    double LambdaEm[ICS_MAX_LAMBDA];           /* Emission wavelength in nm */
    int ExPhotonCnt[ICS_MAX_LAMBDA];           /* # of excitation photons */
    double RefrInxMedium;                      /* Refractive index of embedding medium */
    double NumAperture;                        /* Numerical Aperture */
    double RefrInxLensMedium;                  /* Refractive index of design medium */
    double PinholeSpacing;                     /* Nipkow Disk pinhole spacing */

    /* SCIL_Image compatibility parameter */
    char ScilType[ICS_STRLEN_TOKEN];           /* SCIL_TYPE string */
} ICS;

/* These are the error codes: */
typedef enum {
    IcsErr_Ok = 0,
    IcsErr_FSizeConflict,       /* Non fatal error: unexpected data size */
    IcsErr_OutputNotFilled,     /* Non fatal error: the output buffer could not be
                                   completely filled (meaning that your buffer was
                                   too large) */
    IcsErr_Alloc,               /* Memory allocation error */
    IcsErr_BitsVsSizeConfl,     /* Image size conflicts with bits per element */
    IcsErr_BufferTooSmall,      /* The buffer was too small to hold the given ROI */
    IcsErr_CompressionProblem,  /* Some error occurred during compression */
    IcsErr_CorruptedStream,     /* The compressed input stream is currupted */
    IcsErr_DecompressionProblem,/* Some error occurred during decompression */
    IcsErr_DuplicateData,       /* The ICS data structure already contains
                                   incompatible stuff */
    IcsErr_EmptyField,          /* Empty field (intern error) */
    IcsErr_EndOfHistory,        /* All history lines have already been returned */
    IcsErr_EndOfStream,         /* Unexpected end of stream */
    IcsErr_FailWriteLine,       /* Failed to write a line in .ics file */
    IcsErr_FCloseIcs,           /* File close error on .ics file */
    IcsErr_FCloseIds,           /* File close error on .ids file */
    IcsErr_FOpenIcs,            /* File open error on .ics file */
    IcsErr_FOpenIds,            /* File open error on .ids file */
    IcsErr_FReadIcs,            /* File read error on .ics file */
    IcsErr_FReadIds,            /* File read error on .ids file */
    IcsErr_FWriteIcs,           /* File write error on .ics file */
    IcsErr_FWriteIds,           /* File write error on .ids file */
    IcsErr_HistoryCorrupt,      /* The history buffer is corrupt. Sorry? */
    IcsErr_IllegalROI,          /* The given ROI extends outside the image */
    IcsErr_IllIcsToken,         /* Illegal Ics token detected */
    IcsErr_LineOverflow,        /* Line overflow in ics file */
    IcsErr_MissBits,            /* Missing "bits" element in .ics file */
    IcsErr_MissCat,             /* Missing main category */
    IcsErr_MissingData,         /* There is no Data defined */
    IcsErr_MissLayoutSubCat,    /* Missing layout subcategory */
    IcsErr_MissParamSubCat,     /* Missing parameter subcategory */
    IcsErr_MissRepresSubCat,    /* Missing representation subcategory */
    IcsErr_MissSensorSubCat,    /* Missing sensor subcategory */
    IcsErr_MissSensorSubSubCat, /* Missing sensor subsubcategory */
    IcsErr_MissSubCat,          /* Missing sub category */
    IcsErr_NoLayout,            /* Layout parameters missing or not defined */
    IcsErr_NoScilType,          /* There doesn't exist a SCIL_TYPE value for this image */
    IcsErr_NotIcsFile,          /* Not an ICS file */
    IcsErr_NotValidAction,      /* The function won't work on the ICS given */
    IcsErr_TooManyChans,        /* Too many channels specified */
    IcsErr_TooManyDims,         /* Data has too many dimensions */
    IcsErr_UnknownCompression,  /* Unknown compression type */
    IcsErr_UnknownDataType      /* The datatype is not recognized */
} Ics_Error;

/* Used by IcsGetHistoryString */
typedef enum {
    IcsWhich_First,             /* Get the first string */
    IcsWhich_Next               /* Get the next string */
} Ics_HistoryWhich;

/*
 * Function declarations and short explanation:
 */

ICSEXPORT const char* IcsGetLibVersion (void);
/* Returns a string that can be used to compare with ICSLIB_VERSION to check
 * if the version of the library is the same as that of the headers. */

ICSEXPORT int IcsVersion (const char* filename);
/* Returns 0 if it is not an ICS file, or the version number if it is. */

ICSEXPORT Ics_Error IcsLoadPreview (const char* filename, size_t planenumber,
                void** dest, size_t *xsize, size_t *ysize);
/* Read a preview (2D) image out of an ICS file. The buffer is malloc'd, xsize
 * and ysize are set to the image size. The data type is always uint8. You need
 * to free() the data block when you're done. */

ICSEXPORT Ics_Error IcsOpen (ICS* *ics, const char* filename, const char* mode);
/* Open an ICS file for reading (mode = "r") or writing (mode = "w").
 * When writing, append a "2" to the mode string to create an ICS version
 * 2.0 file. */

ICSEXPORT Ics_Error IcsClose (ICS* ics);
/* Close the ICS file. The ics 'stream' is no longer valid after this.
 * No files are actually written until this function is called. */

ICSEXPORT Ics_Error IcsGetLayout (ICS* ics, Ics_DataType* dt, int *ndims, size_t* dims);
/* Retrieve the layout of an ICS image. Only valid if reading. */

ICSEXPORT Ics_Error IcsSetLayout (ICS* ics, Ics_DataType dt, int ndims, const size_t* dims);
/* Set the layout for an ICS image. Only valid if writing. */

ICSEXPORT size_t IcsGetDataSize (ICS* ics);
ICSEXPORT size_t IcsGetImelSize (ICS* ics);
ICSEXPORT size_t IcsGetImageSize (ICS* ics);
/* These three functions retrieve info from the ICS file.
 * IcsGetDataSize(ics) == IcsGetImelSize(ics) * IcsGetImageSize(ics) */

ICSEXPORT Ics_Error IcsGetData (ICS* ics, void* dest, size_t n);
/* Read the actual image data from an ICS file. Only valid if reading. */

ICSEXPORT Ics_Error IcsGetROIData (ICS* ics, const size_t* offset,
               const size_t* size, const size_t* sampling,
               void* dest, size_t n);
/* Read a square region of the actual image from an ICS file. To use the defaults
 * in one of the parameters, set the pointer to NULL. Only valid if reading. */

ICSEXPORT Ics_Error IcsGetDataBlock (ICS* ics, void* dest, size_t n);
/* Read a portion of the actual image data from an ICS file. Only valid if
 * reading. */

ICSEXPORT Ics_Error IcsSkipDataBlock (ICS* ics, size_t n);
/* Skip a portion of the actual image from an ICS file. Only valid if
 * reading. */

ICSEXPORT Ics_Error IcsGetPreviewData (ICS* ics, void* dest, size_t n,
                                       size_t planenumber);
/* Read a plane of the actual image data from an ICS file, and convert it
 * to uint8. Only valid if reading. */

ICSEXPORT Ics_Error IcsSetData (ICS* ics, const void* src, size_t n);
/* Set the image data for an ICS image. The pointer to this data must
 * be accessible until IcsClose has been called. Only valid if writing. */

ICSEXPORT Ics_Error IcsSetSource (ICS* ics, const char* fname, size_t offset);
/* Set the image source parameter for an ICS version 2.0 file. Only
 * valid if writing. */

ICSEXPORT Ics_Error IcsSetCompression (ICS* ics, Ics_Compression compression, int level);
/* Set the compression method and compression parameter. Only valid if writing. */

ICSEXPORT Ics_Error IcsAddHistory (ICS* ics, const char* key, const char* stuff);
/* Add history lines to the ICS file. Only valid if writing. */

ICSEXPORT Ics_Error IcsGetHistoryString (ICS* ics, char* history, Ics_HistoryWhich which);
/* Get HISTORY lines from the ICS file. history must have at least ICS_LINE_LENGTH
 * characters allocated. Only valid if reading. */

ICSEXPORT Ics_Error IcsGetNumHistoryStrings (ICS* ics, int *num);
/* Get the number of HISTORY lines from the ICS file. Only valid if reading. */

ICSEXPORT Ics_Error IcsGetPosition (ICS* ics, int dimension, double *origin,
                                    double *scale, char* units);
/* Get the position of the image in the real world: the origin of the first
 * pixel, the distances between pixels and the units in which to measure.
 * If you are not interested in one of the parameters, set the pointer to NULL.
 * Dimensions start at 0. Only valid if reading. */

ICSEXPORT Ics_Error IcsSetPosition (ICS* ics, int dimension, double origin,
                                    double scale, const char* units);
/* Set the position of the image in the real world: the origin of the first
 * pixel, the distances between pixels and the units in which to measure.
 * If units is NULL or empty, it is set to the default value of "undefined".
 * Dimensions start at 0. Only valid if writing. */

ICSEXPORT Ics_Error IcsGetOrder (ICS* ics, int dimension, char* order, char* label);
/* Get the ordering of the dimensions in the image. The ordering is defined
 * by names and labels for each dimension. The defaults are x, y, z, t (time)
 * and p (probe). Dimensions start at 0. Only valid if reading. */

ICSEXPORT Ics_Error IcsSetOrder (ICS* ics, int dimension, const char* order,
                                 const char* label);
/* Set the ordering of the dimensions in the image. The ordering is defined
 * by providing names and labels for each dimension. The defaults are
 * x, y, z, t (time) and p (probe). Dimensions start at 0. Only valid if writing. */

ICSEXPORT Ics_Error IcsGetCoordinateSystem (ICS* ics, char* coord);
/* Get the coordinate system used in the positioning of the pixels.
 * Related to IcsGetPosition(). The default is "video". Only valid if
 * reading. */

ICSEXPORT Ics_Error IcsSetCoordinateSystem (ICS* ics, const char* coord);
/* Set the coordinate system used in the positioning of the pixels.
 * Related to IcsSetPosition(). The default is "video". Only valid if
 * writing. */

ICSEXPORT Ics_Error IcsGetSignificantBits (ICS* ics, size_t *nbits);
/* Get the number of significant bits. Only valid if reading. */

ICSEXPORT Ics_Error IcsSetSignificantBits (ICS* ics, size_t nbits);
/* Set the number of significant bits. Only valid if writing. */

ICSEXPORT Ics_Error IcsGetImelUnits (ICS* ics, double *origin, double *scale, char* units);
/* Set the position of the pixel values: the offset and scaling, and the
 * units in which to measure. If you are not interested in one of the
 * parameters, set the pointer to NULL. Only valid if reading. */

ICSEXPORT Ics_Error IcsSetImelUnits (ICS* ics, double origin, double scale, const char* units);
/* Set the position of the pixel values: the offset and scaling, and the
 * units in which to measure. If units is NULL or empty, it is set to the
 * default value of "relative". Only valid if writing. */

ICSEXPORT Ics_Error IcsGetScilType (ICS* ics, char* sciltype);
/* Get the string for the SCIL_TYPE parameter. This string is used only
 * by SCIL_Image. Only valid if reading. */

ICSEXPORT Ics_Error IcsSetScilType (ICS* ics, const char* sciltype);
/* Set the string for the SCIL_TYPE parameter. This string is used only
 * by SCIL_Image. It is required if you want to read the image using
 * SCIL_Image. Only valid if writing. */

ICSEXPORT Ics_Error IcsGuessScilType (ICS* ics);
/* As IcsSetScilType, but creates a string according to the DataType
 * in the ICS structure. It can create a string for g2d, g3d, f2d, f3d,
 * c2d and c3d. Only valid if writing. */

ICSEXPORT const char* IcsGetErrorText (Ics_Error error);
/* Returns a textual representation of an error. */

#ifdef __cplusplus
}
#endif

#endif /* LIBICS_H */
