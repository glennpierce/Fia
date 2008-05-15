# python build_ext - Linux
# python build_ext -c mingw32 - Windows

from distutils.core import setup, Extension

include_dirs = ['C:\Python25\include\pygtk-2.0',
                'C:\Python25\include\pygtk-2.0\pygtk',
                'C:\Python25\include',
                'C:\GTK\include\glib-2.0',
                'C:\GTK\lib\glib-2.0\include',
                'C:\GTK\include\gtk-2.0',
                'C:\GTK\lib\gtk-2.0\include',
                'C:\GTK\include\cairo',
                'C:\GTK\include\pango-1.0',
                'C:\GTK\include\\atk-1.0']

              
libraries = ['gtk-win32-2.0.dll',
             'gdk-win32-2.0.dll',
             'gobject-2.0.dll',
             'glib-2.0.dll',
             'gdk_pixbuf-2.0.dll',
             'intl.dll',
             'iconv.dll']
               
library_dirs = ['C:\GTK\lib']
               
module1 = Extension('imageviewer',
                    define_macros = [('MAJOR_VERSION', '1'),
                                     ('MINOR_VERSION', '0')],
                    include_dirs = include_dirs,
                    libraries = libraries,
                    library_dirs = library_dirs,
                    sources = ['imageviewer.c',
                               'imageviewermodule.c',
                               'gtk_image_viewer.c',
                               'gtk_image_viewer_marshalers.c'])
                               
setup (name = 'imageviewer',
       version = '1.0',
       description = 'This is a wrapper for gtkimageviewer',
       author = 'Glenn Pierce',
       author_email = 'glennpierce@gmail.com',
       url = 'http://www.python.org/doc/current/ext/building.html',
       long_description = '''Python wrapper for gtkimageviewer.''',
       ext_modules = [module1])