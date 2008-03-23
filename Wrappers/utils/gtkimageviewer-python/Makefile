# Set the C flags to include the GTK+ and Python libraries
CFLAGS = `pkg-config --cflags gtk+-2.0 pygtk-2.0` -I/usr/include/python2.5/ -I. -I../src
LDFLAGS = `pkg-config --libs gtk+-2.0 pygtk-2.0` -L../src

imageviewer.so: imageviewer.o imageviewermodule.o ../src/gtk_image_viewer.o ../src/gtk_image_viewer_marshalers.o
	$(CC) $(LDFLAGS) -shared $^ -o $@

# The path to the GTK+ python types
DEFS=`pkg-config --variable=defsdir pygtk-2.0`

# Generate the C wrapper from the defs and our override file
imageviewer.c: imageviewer.defs imageviewer.override
	pygtk-codegen-2.0 --prefix imageviewer \
	--register $(DEFS)/gdk-types.defs \
	--register $(DEFS)/gtk-types.defs \
	--override imageviewer.override \
	imageviewer.defs > $@

# A rule to clean the generated files
clean:
	rm -f imageviewer.so *.o imageviewer.c *~ 

.PHONY: clean
