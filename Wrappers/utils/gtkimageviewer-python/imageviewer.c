/* -- THIS FILE IS GENERATED - DO NOT EDIT *//* -*- Mode: C; c-basic-offset: 4 -*- */

#include <Python.h>



#line 4 "imageviewer.override"
#include <Python.h>
               
#include "pygobject.h"
#include "gtk_image_viewer.h"
#line 13 "imageviewer.c"


/* ---------- types from other modules ---------- */
static PyTypeObject *_PyGtkWidget_Type;
#define PyGtkWidget_Type (*_PyGtkWidget_Type)
static PyTypeObject *_PyGdkPixbuf_Type;
#define PyGdkPixbuf_Type (*_PyGdkPixbuf_Type)
static PyTypeObject *_PyGtkAdjustment_Type;
#define PyGtkAdjustment_Type (*_PyGtkAdjustment_Type)


/* ---------- forward type declarations ---------- */
PyTypeObject G_GNUC_INTERNAL PyGtkImageViewer_Type;

#line 28 "imageviewer.c"



/* ----------- GtkImageViewer ----------- */

#line 19 "imageviewer.override"
static int
_wrap_gtk_image_viewer_new(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "pixbuf", NULL };
    PyGObject *pixbuf = NULL;
    char *filepath = NULL;

    if(PyArg_ParseTuple(args, "z", &filepath)) {
	if(filepath != NULL)
	    self->obj = (GObject *)gtk_image_viewer_new_from_file(filepath);
	else
	    self->obj = (GObject *)gtk_image_viewer_new(NULL);
    }
    else if(PyArg_ParseTupleAndKeywords(args, kwargs,"O!:GtkImageViewer.__init__", kwlist, &PyGdkPixbuf_Type, &pixbuf)) {

	self->obj = (GObject *)gtk_image_viewer_new(GDK_PIXBUF(pixbuf->obj));
    }
    else {
        return -1;
    }

    if (!self->obj) {
        PyErr_SetString(PyExc_RuntimeError, "could not create GtkImageViewer object");
        return -1;
    }
    pygobject_register_wrapper((PyObject *)self);
    return 0;
}
#line 63 "imageviewer.c"


static PyObject *
_wrap_gtk_image_viewer_set_image(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "_img", NULL };
    PyGObject *_img;
    guint ret;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"O!:GtkImageViewer.set_image", kwlist, &PyGdkPixbuf_Type, &_img))
        return NULL;
    
    ret = gtk_image_viewer_set_image(GTK_IMAGE_VIEWER(self->obj), GDK_PIXBUF(_img->obj));
    
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_set_file(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "filename", NULL };
    char *filename;
    guint ret;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"s:GtkImageViewer.set_file", kwlist, &filename))
        return NULL;
    
    ret = gtk_image_viewer_set_file(GTK_IMAGE_VIEWER(self->obj), filename);
    
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_get_image(PyGObject *self)
{
    GdkPixbuf *ret;

    
    ret = gtk_image_viewer_get_image(GTK_IMAGE_VIEWER(self->obj));
    
    /* pygobject_new handles NULL checking */
    return pygobject_new((GObject *)ret);
}

static PyObject *
_wrap_gtk_image_viewer_get_image_width(PyGObject *self)
{
    guint ret;

    
    ret = gtk_image_viewer_get_image_width(GTK_IMAGE_VIEWER(self->obj));
    
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_get_image_height(PyGObject *self)
{
    guint ret;

    
    ret = gtk_image_viewer_get_image_height(GTK_IMAGE_VIEWER(self->obj));
    
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_get_is_panning(PyGObject *self)
{
    int ret;

    
    ret = gtk_image_viewer_get_is_panning(GTK_IMAGE_VIEWER(self->obj));
    
    return PyBool_FromLong(ret);

}

static PyObject *
_wrap_gtk_image_viewer_get_canvas_width(PyGObject *self)
{
    guint ret;

    
    ret = gtk_image_viewer_get_canvas_width(GTK_IMAGE_VIEWER(self->obj));
    
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_get_canvas_height(PyGObject *self)
{
    guint ret;

    
    ret = gtk_image_viewer_get_canvas_height(GTK_IMAGE_VIEWER(self->obj));
    
    return PyLong_FromUnsignedLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_set_transform(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "x", "y", "scale_x", "scale_y", NULL };
    double x, y, scale_x, scale_y;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"dddd:GtkImageViewer.set_transform", kwlist, &x, &y, &scale_x, &scale_y))
        return NULL;
    
    gtk_image_viewer_set_transform(GTK_IMAGE_VIEWER(self->obj), x, y, scale_x, scale_y);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_set_min_zoom(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "min_zoom", NULL };
    double min_zoom;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"d:GtkImageViewer.set_min_zoom", kwlist, &min_zoom))
        return NULL;
    
    gtk_image_viewer_set_min_zoom(GTK_IMAGE_VIEWER(self->obj), min_zoom);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_set_max_zoom(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "min_zoom", NULL };
    double min_zoom;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"d:GtkImageViewer.set_max_zoom", kwlist, &min_zoom))
        return NULL;
    
    gtk_image_viewer_set_max_zoom(GTK_IMAGE_VIEWER(self->obj), min_zoom);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_get_min_zoom(PyGObject *self)
{
    double ret;

    
    ret = gtk_image_viewer_get_min_zoom(GTK_IMAGE_VIEWER(self->obj));
    
    return PyFloat_FromDouble(ret);
}

static PyObject *
_wrap_gtk_image_viewer_get_max_zoom(PyGObject *self)
{
    double ret;

    
    ret = gtk_image_viewer_get_max_zoom(GTK_IMAGE_VIEWER(self->obj));
    
    return PyFloat_FromDouble(ret);
}

static PyObject *
_wrap_gtk_image_viewer_redraw(PyGObject *self)
{
    int ret;

    
    ret = gtk_image_viewer_redraw(GTK_IMAGE_VIEWER(self->obj));
    
    return PyInt_FromLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_zoom_around_fixed_point(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "new_scale_x", "new_scale_y", "old_x", "old_y", "new_x", "new_y", NULL };
    int ret;
    double new_scale_x, new_scale_y, old_x, old_y, new_x, new_y;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"dddddd:GtkImageViewer.zoom_around_fixed_point", kwlist, &new_scale_x, &new_scale_y, &old_x, &old_y, &new_x, &new_y))
        return NULL;
    
    ret = gtk_image_viewer_zoom_around_fixed_point(GTK_IMAGE_VIEWER(self->obj), new_scale_x, new_scale_y, old_x, old_y, new_x, new_y);
    
    return PyInt_FromLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_expose_area(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "x0", "y0", "w", "h", NULL };
    int x0, y0, w, h;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"iiii:GtkImageViewer.expose_area", kwlist, &x0, &y0, &w, &h))
        return NULL;
    
    gtk_image_viewer_expose_area(GTK_IMAGE_VIEWER(self->obj), x0, y0, w, h);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_get_scroll_width(PyGObject *self)
{
    int ret;

    
    ret = gtk_image_viewer_get_scroll_width(GTK_IMAGE_VIEWER(self->obj));
    
    return PyInt_FromLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_get_scroll_height(PyGObject *self)
{
    int ret;

    
    ret = gtk_image_viewer_get_scroll_height(GTK_IMAGE_VIEWER(self->obj));
    
    return PyInt_FromLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_set_scroll_height(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "scroll_height", NULL };
    int scroll_height;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"i:GtkImageViewer.set_scroll_height", kwlist, &scroll_height))
        return NULL;
    
    gtk_image_viewer_set_scroll_height(GTK_IMAGE_VIEWER(self->obj), scroll_height);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_set_scroll_region(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "x0", "y0", "x1", "y1", NULL };
    int ret;
    double x0, y0, x1, y1;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"dddd:GtkImageViewer.set_scroll_region", kwlist, &x0, &y0, &x1, &y1))
        return NULL;
    
    ret = gtk_image_viewer_set_scroll_region(GTK_IMAGE_VIEWER(self->obj), x0, y0, x1, y1);
    
    return PyInt_FromLong(ret);
}

static PyObject *
_wrap_gtk_image_viewer_set_scroll_width(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "scroll_width", NULL };
    int scroll_width;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"i:GtkImageViewer.set_scroll_width", kwlist, &scroll_width))
        return NULL;
    
    gtk_image_viewer_set_scroll_width(GTK_IMAGE_VIEWER(self->obj), scroll_width);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_get_use_linear_zoom_steps(PyGObject *self)
{
    int ret;

    
    ret = gtk_image_viewer_get_use_linear_zoom_steps(GTK_IMAGE_VIEWER(self->obj));
    
    return PyBool_FromLong(ret);

}

static PyObject *
_wrap_gtk_image_viewer_set_use_linear_zoom_steps(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "do_use_linear_steps", NULL };
    int do_use_linear_steps;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"i:GtkImageViewer.set_use_linear_zoom_steps", kwlist, &do_use_linear_steps))
        return NULL;
    
    gtk_image_viewer_set_use_linear_zoom_steps(GTK_IMAGE_VIEWER(self->obj), do_use_linear_steps);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_zoom_rel(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "rel_zoomfactor", NULL };
    double rel_zoomfactor;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"d:GtkImageViewer.zoom_rel", kwlist, &rel_zoomfactor))
        return NULL;
    
    gtk_image_viewer_zoom_rel(GTK_IMAGE_VIEWER(self->obj), rel_zoomfactor);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_zoom_fit(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "do_keep_aspect_ratio", NULL };
    int do_keep_aspect_ratio;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"i:GtkImageViewer.zoom_fit", kwlist, &do_keep_aspect_ratio))
        return NULL;
    
    gtk_image_viewer_zoom_fit(GTK_IMAGE_VIEWER(self->obj), do_keep_aspect_ratio);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_zoom_100(PyGObject *self)
{
    
    gtk_image_viewer_zoom_100(GTK_IMAGE_VIEWER(self->obj));
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_set_hadjustment(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "adjustment", NULL };
    PyGObject *adjustment;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"O!:GtkImageViewer.set_hadjustment", kwlist, &PyGtkAdjustment_Type, &adjustment))
        return NULL;
    
    gtk_image_viewer_set_hadjustment(GTK_IMAGE_VIEWER(self->obj), GTK_ADJUSTMENT(adjustment->obj));
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_set_vadjustment(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "adjustment", NULL };
    PyGObject *adjustment;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"O!:GtkImageViewer.set_vadjustment", kwlist, &PyGtkAdjustment_Type, &adjustment))
        return NULL;
    
    gtk_image_viewer_set_vadjustment(GTK_IMAGE_VIEWER(self->obj), GTK_ADJUSTMENT(adjustment->obj));
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_wrap_gtk_image_viewer_set_flip(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "do_vflip", "do_hflip", NULL };
    int do_vflip, do_hflip;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"ii:GtkImageViewer.set_flip", kwlist, &do_vflip, &do_hflip))
        return NULL;
    
    gtk_image_viewer_set_flip(GTK_IMAGE_VIEWER(self->obj), do_vflip, do_hflip);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static const PyMethodDef _PyGtkImageViewer_methods[] = {
    { "set_image", (PyCFunction)_wrap_gtk_image_viewer_set_image, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "set_file", (PyCFunction)_wrap_gtk_image_viewer_set_file, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "get_image", (PyCFunction)_wrap_gtk_image_viewer_get_image, METH_NOARGS,
      NULL },
    { "get_image_width", (PyCFunction)_wrap_gtk_image_viewer_get_image_width, METH_NOARGS,
      NULL },
    { "get_image_height", (PyCFunction)_wrap_gtk_image_viewer_get_image_height, METH_NOARGS,
      NULL },
    { "get_is_panning", (PyCFunction)_wrap_gtk_image_viewer_get_is_panning, METH_NOARGS,
      NULL },
    { "get_canvas_width", (PyCFunction)_wrap_gtk_image_viewer_get_canvas_width, METH_NOARGS,
      NULL },
    { "get_canvas_height", (PyCFunction)_wrap_gtk_image_viewer_get_canvas_height, METH_NOARGS,
      NULL },
    { "set_transform", (PyCFunction)_wrap_gtk_image_viewer_set_transform, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "set_min_zoom", (PyCFunction)_wrap_gtk_image_viewer_set_min_zoom, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "set_max_zoom", (PyCFunction)_wrap_gtk_image_viewer_set_max_zoom, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "get_min_zoom", (PyCFunction)_wrap_gtk_image_viewer_get_min_zoom, METH_NOARGS,
      NULL },
    { "get_max_zoom", (PyCFunction)_wrap_gtk_image_viewer_get_max_zoom, METH_NOARGS,
      NULL },
    { "redraw", (PyCFunction)_wrap_gtk_image_viewer_redraw, METH_NOARGS,
      NULL },
    { "zoom_around_fixed_point", (PyCFunction)_wrap_gtk_image_viewer_zoom_around_fixed_point, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "expose_area", (PyCFunction)_wrap_gtk_image_viewer_expose_area, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "get_scroll_width", (PyCFunction)_wrap_gtk_image_viewer_get_scroll_width, METH_NOARGS,
      NULL },
    { "get_scroll_height", (PyCFunction)_wrap_gtk_image_viewer_get_scroll_height, METH_NOARGS,
      NULL },
    { "set_scroll_height", (PyCFunction)_wrap_gtk_image_viewer_set_scroll_height, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "set_scroll_region", (PyCFunction)_wrap_gtk_image_viewer_set_scroll_region, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "set_scroll_width", (PyCFunction)_wrap_gtk_image_viewer_set_scroll_width, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "get_use_linear_zoom_steps", (PyCFunction)_wrap_gtk_image_viewer_get_use_linear_zoom_steps, METH_NOARGS,
      NULL },
    { "set_use_linear_zoom_steps", (PyCFunction)_wrap_gtk_image_viewer_set_use_linear_zoom_steps, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "zoom_rel", (PyCFunction)_wrap_gtk_image_viewer_zoom_rel, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "zoom_fit", (PyCFunction)_wrap_gtk_image_viewer_zoom_fit, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "zoom_100", (PyCFunction)_wrap_gtk_image_viewer_zoom_100, METH_NOARGS,
      NULL },
    { "set_hadjustment", (PyCFunction)_wrap_gtk_image_viewer_set_hadjustment, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "set_vadjustment", (PyCFunction)_wrap_gtk_image_viewer_set_vadjustment, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { "set_flip", (PyCFunction)_wrap_gtk_image_viewer_set_flip, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { NULL, NULL, 0, NULL }
};

PyTypeObject G_GNUC_INTERNAL PyGtkImageViewer_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "imageviewer.ImageViewer",                   /* tp_name */
    sizeof(PyGObject),          /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)0,        /* tp_dealloc */
    (printfunc)0,                      /* tp_print */
    (getattrfunc)0,       /* tp_getattr */
    (setattrfunc)0,       /* tp_setattr */
    (cmpfunc)0,           /* tp_compare */
    (reprfunc)0,             /* tp_repr */
    (PyNumberMethods*)0,     /* tp_as_number */
    (PySequenceMethods*)0, /* tp_as_sequence */
    (PyMappingMethods*)0,   /* tp_as_mapping */
    (hashfunc)0,             /* tp_hash */
    (ternaryfunc)0,          /* tp_call */
    (reprfunc)0,              /* tp_str */
    (getattrofunc)0,     /* tp_getattro */
    (setattrofunc)0,     /* tp_setattro */
    (PyBufferProcs*)0,  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                      /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)0,     /* tp_traverse */
    (inquiry)0,             /* tp_clear */
    (richcmpfunc)0,   /* tp_richcompare */
    offsetof(PyGObject, weakreflist),             /* tp_weaklistoffset */
    (getiterfunc)0,          /* tp_iter */
    (iternextfunc)0,     /* tp_iternext */
    (struct PyMethodDef*)_PyGtkImageViewer_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)_wrap_gtk_image_viewer_new,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};



/* ----------- functions ----------- */

static PyObject *
_wrap_gtk_image_viewer_new_from_file(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "filename", NULL };
    char *filename;
    GtkWidget *ret;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,"s:gtk_image_viewer_new_from_file", kwlist, &filename))
        return NULL;
    
    ret = gtk_image_viewer_new_from_file(filename);
    
    /* pygobject_new handles NULL checking */
    return pygobject_new((GObject *)ret);
}

const PyMethodDef imageviewer_functions[] = {
    { "gtk_image_viewer_new_from_file", (PyCFunction)_wrap_gtk_image_viewer_new_from_file, METH_VARARGS|METH_KEYWORDS,
      NULL },
    { NULL, NULL, 0, NULL }
};

/* initialise stuff extension classes */
void
imageviewer_register_classes(PyObject *d)
{
    PyObject *module;

    if ((module = PyImport_ImportModule("gtk")) != NULL) {
        _PyGtkWidget_Type = (PyTypeObject *)PyObject_GetAttrString(module, "Widget");
        if (_PyGtkWidget_Type == NULL) {
            PyErr_SetString(PyExc_ImportError,
                "cannot import name Widget from gtk");
            return ;
        }
        _PyGtkAdjustment_Type = (PyTypeObject *)PyObject_GetAttrString(module, "Adjustment");
        if (_PyGtkAdjustment_Type == NULL) {
            PyErr_SetString(PyExc_ImportError,
                "cannot import name Adjustment from gtk");
            return ;
        }
    } else {
        PyErr_SetString(PyExc_ImportError,
            "could not import gtk");
        return ;
    }
    if ((module = PyImport_ImportModule("gtk.gdk")) != NULL) {
        _PyGdkPixbuf_Type = (PyTypeObject *)PyObject_GetAttrString(module, "Pixbuf");
        if (_PyGdkPixbuf_Type == NULL) {
            PyErr_SetString(PyExc_ImportError,
                "cannot import name Pixbuf from gtk.gdk");
            return ;
        }
    } else {
        PyErr_SetString(PyExc_ImportError,
            "could not import gtk.gdk");
        return ;
    }


#line 622 "imageviewer.c"
    pygobject_register_class(d, "GtkImageViewer", GTK_TYPE_IMAGE_VIEWER, &PyGtkImageViewer_Type, Py_BuildValue("(O)", &PyGtkWidget_Type));
}
