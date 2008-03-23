#!/usr/bin/env python

import sys, os 
import matplotlib  
import imageviewer
import platform

# Add our wrapper directory to the module path
sys.path.append("..")
print "Path: ", sys.path

import FreeImagePy as FI
import FreeImageAlgorithmsPy.FreeImageAlgorithmsPy as FIA

matplotlib.use('GTK')  

from matplotlib.figure import Figure  
from matplotlib.backends.backend_gtk import FigureCanvasGTK, NavigationToolbar  
 
try:  
    import pygtk  
    pygtk.require("2.0")  
except:  
    pass

try:
    import gtk
    import gtk.glade
except:
    sys.exit(1)
        
class Gui:  

    def buildUI(self):
        self.F = None
        gladefile = "Cyrinic.glade"  
        self.windowname = "Cyrinic"  
        self.wTree = gtk.glade.XML(gladefile, self.windowname)  
        self.win = self.wTree.get_widget(self.windowname)
        self.imagescrolledwin = self.wTree.get_widget("imagescrolledwindow")
        self.imagelabel = self.wTree.get_widget("imagelabel")
        dic = {"on_quit" : gtk.main_quit,
               "show_about" : self.showAbout,
               "on_openmenuitem_clicked" : self.onImageOpen,
               "on_histogram" : self.onHistogram
               } 
        self.wTree.signal_autoconnect(dic) 
        self.imageviewer = imageviewer.ImageViewer(None)
        self.imagescrolledwin.add(self.imageviewer)
        self.win.show_all()
        
    def __init__(self):  
        self.buildUI()

    def getWidgetTree(self):
        return self.wTree
        
    def setOutputText(self, text):
        self.__logbuffer.set_text(text)

    def OpenImageDialog(self):
        dialog = gtk.FileChooserDialog("Open..",
                                       None,
                                       gtk.FILE_CHOOSER_ACTION_OPEN,
                                      (gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
                                       gtk.STOCK_OPEN, gtk.RESPONSE_OK))
        dialog.set_default_response(gtk.RESPONSE_OK)

        filter = gtk.FileFilter()
        filter.set_name("All files")
        filter.add_pattern("*")
        dialog.add_filter(filter)
 
        image_patterns = ["*.bmp", "*.png", "*.jpg", "*.gif", "*.tif", "*.xpm"]
        image_mime_types = ["image/bmp", "image/png", "image/jpeg", "image/gif"]
 
        filter = gtk.FileFilter()
        filter.set_name("Images")
        
        for mime in image_mime_types:
                filter.add_mime_type(mime)
                
        for pattern in image_patterns:
                filter.add_pattern(pattern)
 
        dialog.add_filter(filter)
        dialog.set_filter(filter)

        return dialog

    def setImage(self, filepath, pixbuf):
        self.imageviewer.set_image(pixbuf)
        self.win.set_title("Cyrinic - " + filepath)
        self.imagelabel.set_text(os.path.split(filepath)[1])
        
    def displayGraph(self, xaxis, yaxis, hist):
         
        if(hist == None):
            return
         
        try:
            dialog = gtk.Window()
            figure = Figure(figsize=(6,4), dpi=72)
            axis = figure.add_subplot(111)
            canvas = FigureCanvasGTK(figure) # a gtk.DrawingArea
            canvas.show()
            dialog.add(canvas)   
            
            # empty axis if neccesary, and reset title and stuff
            axis.clear()
            axis.set_xlabel(xaxis)
            axis.set_ylabel(yaxis)
            
            if len(hist) > 1:
                axis.plot(hist[0], 'r')
                axis.plot(hist[1], 'g')
                axis.plot(hist[2], 'b')
            else:
                axis.plot(hist[0], 'r')
            
            dialog.show()
 
        except ValueError:
            sys.exit(1)   
            
    def onImageOpen(self, widget):
        
        dialog = self.OpenImageDialog()
        response = dialog.run()
        
        if response == gtk.RESPONSE_OK:
        
            if self.F == None:
                del self.F
                self.F = None
            
            self.F = FIA.FIAImage()
            self.F.load(dialog.get_filename())
            pixbuf = FI.utils.convertToGdkPixbuf(self.F)
                  
            if pixbuf:
                self.setImage(dialog.get_filename(), pixbuf)            
              
        dialog.destroy()
        
    def onHistogram(self, widget):
        hist= self.F.getHistogram(0, 255, 255)
        self.displayGraph("Intensity", "Count", hist)
        
    def showAbout(self, widget):
        dialog = gtk.AboutDialog()
        dialog.set_name("Cyrinic")
        dialog.set_authors(["Glenn Pierce"]);
        dialog.set_copyright("Glenn Pierce");
        dialog.run()
        dialog.destroy()

if __name__ == "__main__":
    app = Gui()
    gtk.main()
