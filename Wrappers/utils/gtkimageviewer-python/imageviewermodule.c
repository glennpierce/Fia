/*
 * Copyright (c) 2001 by Glenn Pierce <glennpierce@gmail.com>
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
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <pygobject.h>
 
void imageviewer_register_classes (PyObject *d); 
extern PyMethodDef imageviewer_functions[];
 
DL_EXPORT(void)
initimageviewer(void)
{
    PyObject *m, *d;
 
    init_pygobject ();
 
    m = Py_InitModule ("imageviewer", imageviewer_functions);
    d = PyModule_GetDict (m);
 
    imageviewer_register_classes (d);
 
    if (PyErr_Occurred ()) {
        Py_FatalError ("can't initialise module imageviewer");
    }
}
