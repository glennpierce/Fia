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

    
# A class that provides only gui objects that plugins should access
class CyrinicOutput:
    
    def __init__(self, gui):
        self.__gui = gui
        self.__outputboxes = gui.getWidgetTree().get_widget("outputboxes")
        
    def setOutputText(self, text):
        self.__gui.setOutputText(text)
        
    def getMenubar(self):
        return self.__gui.getWidgetTree().get_widget("menubar")
        
    def displayGraph(self, xaxis, yaxis, hist):
         
        if(hist == None):
            return
         
        try:
            self.figure = Figure(figsize=(6,4), dpi=72)
            self.axis = self.figure.add_subplot(111)

            self.canvas = FigureCanvasGTK(self.figure) # a gtk.DrawingArea
            self.canvas.show()
            self.__outputboxes.pack_end(self.canvas, True, True)   
            
            # empty axis if neccesary, and reset title and stuff
            self.axis.clear()
            self.axis.set_xlabel(xaxis)
            self.axis.set_ylabel(yaxis)
            
            self.axis.plot(hist, 'r')
 
        except ValueError:
            sys.exit(1)   
        
class Gui:  

    def buildUI(self):
        gladefile = "Cyrinic.glade"  
        self.windowname = "Cyrinic"  
        self.wTree = gtk.glade.XML(gladefile, self.windowname)  
        self.win = self.wTree.get_widget(self.windowname)
        self.imagescrolledwin = self.wTree.get_widget("imagescrolledwindow")
        self.imagelabel = self.wTree.get_widget("imagelabel")
        self.__outputboxes = self.wTree.get_widget("outputboxes")
        self.__output = CyrinicOutput(self)
        dic = {"on_quit" : gtk.main_quit,
               "show_about" : self.showAbout,
               "on_openmenuitem_clicked" : self.onImageOpen} 
        self.wTree.signal_autoconnect(dic) 
        self.__output.displayGraph("Intensity", "Count", None)
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
        
    def onImageOpen(self, widget):
        
        dialog = self.OpenImageDialog()
        response = dialog.run()
        
        if response == gtk.RESPONSE_OK:
            
            F = FIA.FIAImage()
            F.load(dialog.get_filename())
            pixbuf = FI.utils.convertToGdkPixbuf(F)
                  
            if pixbuf:
                self.setImage(dialog.get_filename(), pixbuf)
             
            del F
              
        dialog.destroy()
        
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
