#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk
import imageviewer

class HelloWorld:

    def delete_event(self, widget, event, data=None):
        return False

    def destroy(self, widget, data=None):
        gtk.main_quit()

    def __init__(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    
        self.window.connect("delete_event", self.delete_event)
        self.window.connect("destroy", self.destroy)
    
        self.window.set_border_width(10)

	self.window.set_title("Image Widget Demo");

	self.window.set_size_request (650, 200)

        self.im = imageviewer.ImageViewer("freeimage.jpg")
    
	hadjust = gtk.Adjustment(0, 0, 1, 0.1, 0.1, 0.5)
	vadjust  = gtk.Adjustment(0, 0, 1, 0.1, 0.1, 0.5)

	self.im.set_hadjustment(hadjust)
	self.im.set_vadjustment(vadjust)

	self.scrolled_win = gtk.ScrolledWindow(hadjust, vadjust)
	self.scrolled_win.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)

	self.scrolled_win.add(self.im)

        self.window.add(self.scrolled_win)

	self.scrolled_win.show()    
        self.im.show()
        self.window.show()

    def main(self):
        gtk.main()

if __name__ == "__main__":
    hello = HelloWorld()
    hello.main()

