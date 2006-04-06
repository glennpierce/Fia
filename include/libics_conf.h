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
 * FILE : libics_conf.h
 *
 * This file allows you to configure libics. Only needed to build the
 * library
 *
 * Last change: November 4, 2002
 */

#ifndef LIBICS_CONF_H
#define LIBICS_CONF_H

/*
 * Doubles larger than ICS_MAX_DOUBLE are written in scientific notation.
 */
#define ICS_MAX_DOUBLE 10000000

/*
 * ICS_BUFFSIZE_INCREMENT sets the increment for the buffer allocated
 * to contain the history stuff until written to file.
 */
#define ICS_BUFFSIZE_INCREMENT 2048

/*
 * ZIP_BUF_SIZE is the size of the buffer allocated to do the compression.
 * This is independend from the memory allocated by zlib for the dictionary.
 * SKIP_BUF_SIZE is the buffer allocated to decompress stuff into when skipping
 * a data block (IcsSkipIdsBlock() for compressed files).
 */
#define ZIP_BUF_SIZE 16384
#define SKIP_BUF_SIZE 16384

/*
 * If ICS_DO_GZEXT is defined, the code will search for IDS files
 * which have the .ids.gz or .ids.Z filename extension.
 */
#define ICS_DO_GZEXT

/*
 * If ICS_ZLIB is defined, the zlib dependancy is included, and the
 * library will be able to read GZIP compressed files.
 */
#define ICS_ZLIB

/* ICS_DO_GZEXT implies ICS_ZLIB */
#if defined(ICS_DO_GZEXT) && !defined(ICS_ZLIB)
#define ICS_ZLIB
#endif

/*
 * These are used internally when the precise length of a variable is needed.
 * These are OK for 32-bit systems, but they might be different for 64-bit
 * systems.
 * We also use size_t for a variable that is as wide as a pointer (i.e. can
 * hold the size of any data block).
 */
typedef unsigned char ics_t_uint8;
typedef signed char ics_t_sint8;
typedef unsigned short ics_t_uint16;
typedef signed short ics_t_sint16;
typedef unsigned int ics_t_uint32;
typedef signed int ics_t_sint32;
typedef float ics_t_real32;
typedef double ics_t_real64;

#endif /* LIBICS_CONF_H */
