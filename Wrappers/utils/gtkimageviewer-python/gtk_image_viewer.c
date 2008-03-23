/* 
 * Copyright (c) 2001 by Dov Grobgeld <dov.grobgeld@weizmann.ac.il>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "gtk_image_viewer.h"
#include "gtk_image_viewer_marshalers.h"

#define DBG(a)
#define DBG2(a)

enum {
  VIEW_CHANGED,
  LAST_SIGNAL
};

enum {
  PROP_0,
  PROP_FILE,
  PROP_IMAGE_WIDTH,
  PROP_IMAGE_HEIGHT,
  PROP_HADJUSTMENT,
  PROP_VADJUSTMENT,
  PROP_USE_LINEAR_ZOOM_STEPS
};

/* Forward declarations */

static void gtk_image_viewer_class_init               (GtkImageViewerClass    *klass);
static void gtk_image_viewer_init                     (GtkImageViewer         *image_viewer);
static void gtk_image_viewer_destroy                  (GtkObject        *object);
static void gtk_image_viewer_realize                  (GtkWidget        *widget);
static void gtk_image_viewer_size_request             (GtkWidget      *widget,
					               GtkRequisition *requisition);
static void gtk_image_viewer_size_allocate            (GtkWidget     *widget,
					               GtkAllocation *allocation);
static gint gtk_image_viewer_expose                   (GtkWidget        *widget,
						       GdkEventExpose   *event);
static gint gtk_image_viewer_button_press             (GtkWidget      *widget,
					               GdkEventButton *event);
static gint gtk_image_viewer_scroll                   (GtkWidget      *widget,
                                                       GdkEventScroll *event);
static gint gtk_image_viewer_button_release (GtkWidget        *widget,
					    GdkEventButton   *event);
static gint gtk_image_viewer_motion_notify       (GtkWidget        *widget,
						GdkEventMotion   *event);
static gint gtk_image_viewer_key_press (GtkWidget      *widget,
					GdkEventKey *event);
static gint gtk_image_viewer_enter_notify(GtkWidget *widget,
					  GdkEventCrossing *event);
static gint gtk_image_viewer_leave_notify(GtkWidget *widget,
					  GdkEventCrossing *event);
static gint gtk_image_viewer_focus_in(GtkWidget *widget,
				      GdkEventFocus *event);
static gint gtk_image_viewer_focus_out(GtkWidget *widget,
				       GdkEventFocus *event);
static gint view_changed(GtkImageViewer *image_widget,
			 int do_force,
			 double scale_x,
			 double scale_y,
			 double x0,
			 double y0);

static gint zoom_reset(GtkWidget *widget);
static gint zoom_in(GtkWidget *widget, int x, int y);
static gint zoom_out(GtkWidget *widget, int x, int y);
static gint zoom_translate(GtkWidget *widget, int dx, int dy);
static void gtk_image_viewer_set_property             (GObject         *object,
						   guint            prop_id,
						   const GValue    *value,
						   GParamSpec      *pspec);
static void gtk_image_viewer_get_property             (GObject         *object,
						   guint            prop_id,
						   GValue          *value,
						   GParamSpec      *pspec);
static void gtk_image_viewer_set_scroll_adjustments   (GtkImageViewer  *image_viewer,
						       GtkAdjustment    *hadjustment,
						       GtkAdjustment    *vadjustment);
static void gtk_image_viewer_adjustment_changed       (GtkAdjustment    *adjustment,
						       gpointer          data);
static void gtk_image_viewer_hadjustment_value_changed       (GtkAdjustment    *adjustment,
							     gpointer          data);
static void gtk_image_viewer_vadjustment_value_changed       (GtkAdjustment    *adjustment,
							     gpointer          data);
static void update_adjustments(GtkImageViewer *image_viewer);


/* Local data */
static GtkWidgetClass *parent_class = NULL;
static guint gtk_image_viewer_signals[LAST_SIGNAL] = { 0 };
static void update_adjustments(GtkImageViewer *image_viewer);

guint
gtk_image_viewer_get_type ()
{
  static GtkType image_viewer_type = 0;

  if (!image_viewer_type)
    {
      static const GTypeInfo image_viewer_info =
      {
	sizeof (GtkImageViewerClass),
	NULL,           /* base_init */
	NULL,           /* base_finalize */
	(GClassInitFunc) gtk_image_viewer_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data */
	sizeof(GtkImageViewer),
	32,
	(GInstanceInitFunc) gtk_image_viewer_init
      };

      image_viewer_type = g_type_register_static (GTK_TYPE_MISC, "GtkImageViewer", &image_viewer_info, 0);
    }

  return image_viewer_type;
}

static void
gtk_image_viewer_class_init (GtkImageViewerClass *class)
{
  GtkObjectClass *object_class;
  GObjectClass   *gobject_class;
  GtkWidgetClass *widget_class;

  object_class = (GtkObjectClass*) class;
  widget_class = (GtkWidgetClass*) class;
  gobject_class = G_OBJECT_CLASS (class);

  parent_class = gtk_type_class (gtk_widget_get_type ());

  object_class->destroy = gtk_image_viewer_destroy;

  widget_class->realize = gtk_image_viewer_realize;

  widget_class->expose_event = gtk_image_viewer_expose;
  widget_class->size_request = gtk_image_viewer_size_request;
  widget_class->size_allocate = gtk_image_viewer_size_allocate;
  widget_class->button_press_event = gtk_image_viewer_button_press;
  widget_class->scroll_event = gtk_image_viewer_scroll;
  widget_class->button_release_event = gtk_image_viewer_button_release;
  widget_class->motion_notify_event = gtk_image_viewer_motion_notify;
  widget_class->key_press_event = gtk_image_viewer_key_press;
  widget_class->enter_notify_event = gtk_image_viewer_enter_notify;
  widget_class->leave_notify_event = gtk_image_viewer_leave_notify;
  widget_class->focus_in_event = gtk_image_viewer_focus_in;
  widget_class->focus_out_event = gtk_image_viewer_focus_out;

  class->set_scroll_adjustments = gtk_image_viewer_set_scroll_adjustments;
  
  gobject_class->set_property = gtk_image_viewer_set_property;
  gobject_class->get_property = gtk_image_viewer_get_property;

  gtk_image_viewer_signals[VIEW_CHANGED] =
    gtk_signal_new("view_changed",
		   GTK_RUN_FIRST,
		   GTK_CLASS_TYPE (object_class),
		   GTK_SIGNAL_OFFSET(GtkImageViewerClass, view_changed),
		   gtk_marshal_NONE__NONE,
		   GTK_TYPE_NONE, 0);

  g_object_class_install_property (gobject_class,
                                   PROP_HADJUSTMENT,
                                   g_param_spec_object ("hadjustment",
							("Horizontal adjustment"),
							("The GtkAdjustment that determines the values of the horizontal position for this image_viewer."),
                                                        GTK_TYPE_ADJUSTMENT,
                                                        G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
                                   PROP_VADJUSTMENT,
                                   g_param_spec_object ("vadjustment",
							("Vertical adjustment"),
							("The GtkAdjustment that determines the values of the vertical position for this image_viewer."),
                                                        GTK_TYPE_ADJUSTMENT,
                                                        G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_FILE,
                                   g_param_spec_string ("file",
							("Image file"),
							("The filename of the image that will be shown by the GtkImageViewer."),
                                                        "image.ppm",
                                                        G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_IMAGE_WIDTH,
                                   g_param_spec_int ("image_width",
						     ("Image width"),
						     ("The width of the image that shown by the GtkImageViewer."),
						     0,0,0,
						     G_PARAM_READABLE));
  g_object_class_install_property (gobject_class,
                                   PROP_IMAGE_HEIGHT,
                                   g_param_spec_int ("image_height",
						     ("Image height"),
						     ("The height of the image that shown by the GtkImageViewer."),
						     0,0,0,
						     G_PARAM_READABLE));

  g_object_class_install_property (gobject_class,
                                   PROP_USE_LINEAR_ZOOM_STEPS,
                                   g_param_spec_int ("use_linear_zoom_steps",
						     ("Whether the interactive zoom steps should be linear"),
						     ("The default zoom steps are exponential. Turning this flag on makes the zoom steps linear instead."),
                                                        0,0,0,
                                                        G_PARAM_READWRITE));
  
  widget_class->set_scroll_adjustments_signal =
    gtk_signal_new ("set_scroll_adjustments",
		    GTK_RUN_LAST,
		    GTK_CLASS_TYPE (object_class),
		    GTK_SIGNAL_OFFSET (GtkImageViewerClass, set_scroll_adjustments),
		    gtk_image_viewer_VOID__OBJECT_OBJECT,
		    GTK_TYPE_NONE, 2, GTK_TYPE_ADJUSTMENT, GTK_TYPE_ADJUSTMENT);
}

/*======================================================================
//  Set default values.
//----------------------------------------------------------------------*/
static void
gtk_image_viewer_init (GtkImageViewer *image_viewer)
{
  GTK_WIDGET_SET_FLAGS (image_viewer, GTK_CAN_FOCUS);
  image_viewer->interp_type = GDK_INTERP_NEAREST;
  image_viewer->do_linear_zoom_steps = FALSE;
  image_viewer->do_use_transfer_map = FALSE;
  image_viewer->do_flip_vertical = FALSE;
  image_viewer->do_flip_horizontal = FALSE;
  image_viewer->current_scale_x = 1.0;
  image_viewer->current_scale_y = 1.0;
  image_viewer->current_x0 = 0;
  image_viewer->current_y0 = 0;
  image_viewer->min_zoom = 1.0/8;
  image_viewer->max_zoom = 128;
  image_viewer->scroll_width = -1;
  image_viewer->scroll_height = -1;
  image_viewer->gc = NULL;
  image_viewer->hadjustment = 0;
  image_viewer->vadjustment = 0;
}

/**
 * gtk_image_viewer_new:
 * @pixbuf: A pixbuf.
 *
 * Create a new image viewer widget displaying the pixbuf @pixbuf.
 *
 * Return value: The new image viewer widget.
 **/

GtkWidget*
gtk_image_viewer_new (GdkPixbuf *pixbuf)
{
  GtkImageViewer *image_viewer;

  image_viewer = gtk_type_new (GTK_TYPE_IMAGE_VIEWER);
  image_viewer->image=NULL;

  if (pixbuf != NULL)
    gtk_image_viewer_set_image(image_viewer, pixbuf);

  gtk_widget_set_double_buffered(GTK_WIDGET(image_viewer),
                                 FALSE);

  return GTK_WIDGET (image_viewer);
}

GtkWidget*
gtk_image_viewer_new_from_file (const char *filename)
{
  GtkWidget *widget;
  GdkPixbuf *image = NULL;
  GError *error = NULL;

  image = gdk_pixbuf_new_from_file(filename, &error);

  widget = gtk_image_viewer_new(image);
  
  /* The reference count of image should now be == 2. Decrease it. */
  if (image)
    gdk_pixbuf_unref(image);

  return widget;
}

static void
gtk_image_viewer_destroy (GtkObject *object)
{
  GtkImageViewer *image_viewer;

  g_return_if_fail (object != NULL);
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (object));

  image_viewer = GTK_IMAGE_VIEWER (object);

  if (image_viewer->gc)
    {
      g_object_unref(image_viewer->gc);
      /* I need this since I am called twice and I don't understand why. :-( */
      image_viewer->gc = NULL;
    }
  
  if (image_viewer->image)
    {
      gdk_pixbuf_unref(image_viewer->image);
      image_viewer->image = NULL;
    }

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    (* GTK_OBJECT_CLASS (parent_class)->destroy) (object);
}

static void
gtk_image_viewer_realize (GtkWidget *widget)
{
  GtkImageViewer *image_viewer;
  GdkWindowAttr attributes;
  gint attributes_mask;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (widget));

  GTK_WIDGET_SET_FLAGS (widget, GTK_REALIZED);
  image_viewer = GTK_IMAGE_VIEWER (widget);

  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.event_mask = gtk_widget_get_events (widget) | 
			  GDK_EXPOSURE_MASK |
			  GDK_BUTTON_PRESS_MASK | 
			  GDK_BUTTON_RELEASE_MASK |
			  GDK_POINTER_MOTION_MASK |
                          GDK_POINTER_MOTION_HINT_MASK | 
			  GDK_KEY_PRESS_MASK |
			  GDK_LEAVE_NOTIFY_MASK |
			  GDK_ENTER_NOTIFY_MASK;
  attributes.visual = gtk_widget_get_visual (widget);
  attributes.colormap = gtk_widget_get_colormap (widget);

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
  widget->window = gdk_window_new (widget->parent->window, &attributes, attributes_mask);

  widget->style = gtk_style_attach (widget->style, widget->window);

  gdk_window_set_user_data (widget->window, widget);

  gtk_style_set_background (widget->style, widget->window, GTK_STATE_ACTIVE);

}

static void 
gtk_image_viewer_size_request (GtkWidget      *widget,
			       GtkRequisition *requisition)
{
  GtkImageViewer *image_viewer;
  int req_width = 1, req_height = 1;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (widget));

  image_viewer = GTK_IMAGE_VIEWER (widget);

  if (image_viewer->image) 
    {
      req_width = gdk_pixbuf_get_width(image_viewer->image);
      req_height = gdk_pixbuf_get_height(image_viewer->image);
    }

  requisition->width = req_width;
  requisition->height = req_height;  
}

static void
gtk_image_viewer_size_allocate (GtkWidget     *widget,
			       GtkAllocation *allocation)
{
  GtkImageViewer *image_viewer;
  int old_cnv_w, old_cnv_h;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (widget));
  g_return_if_fail (allocation != NULL);

  widget->allocation = *allocation;
  image_viewer = GTK_IMAGE_VIEWER (widget);

  if (GTK_WIDGET_REALIZED (widget))
    {

      gdk_window_move_resize (widget->window,
			      allocation->x, allocation->y,
			      allocation->width, allocation->height);

      
    }

  old_cnv_w = image_viewer->canvas_width;
  old_cnv_h = image_viewer->canvas_height;
  image_viewer->canvas_width = widget->allocation.width;
  image_viewer->canvas_height = widget->allocation.height;

  /* Update current_x0 and current_y0 if new size is wider than scale *
   * image size!
   */
  if (image_viewer->image)
    {
      double scale_x = image_viewer->current_scale_x;
      double scale_y = image_viewer->current_scale_y;
      int img_w = gdk_pixbuf_get_width(image_viewer->image)*scale_x;
      int img_h = gdk_pixbuf_get_height(image_viewer->image)*scale_y;
      int cnv_w = image_viewer->canvas_width;
      int cnv_h = image_viewer->canvas_height;

      DBG(fprintf(stderr,"Resize: x0 y0 img_w img_h cnv_w cnv_h = %f %f %d %d %d %d\n",
	      image_viewer->current_x0,image_viewer->current_y0,img_w,img_h,cnv_w,cnv_h));

      if (cnv_w > img_w)
	image_viewer->current_x0 = -(cnv_w-img_w)/2;
      else if (old_cnv_w > img_w)
	image_viewer->current_x0 = 0;
      else if (img_w - image_viewer->current_x0 < cnv_w)
	{
	  DBG(fprintf(stderr, "Resize case 3X\n"));
	  image_viewer->current_x0 = img_w-cnv_w;
	}
      if (cnv_h > img_h)
	image_viewer->current_y0 = -(cnv_h-img_h)/2;
      else if (old_cnv_h > img_h)
	image_viewer->current_y0 = 0;
      else if (img_h - image_viewer->current_y0 < cnv_h)
	{
	  DBG(fprintf(stderr, "Resize case 3Y\n"));
	  image_viewer->current_y0 = img_h-cnv_h;
	}
    }
  
}

static gint
gtk_image_viewer_expose (GtkWidget      *widget,
			 GdkEventExpose *event)
{
  GtkImageViewer *image_viewer;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_IMAGE_VIEWER (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  image_viewer = GTK_IMAGE_VIEWER (widget);

  DBG2(g_print("expose %d %d %d %d\n",
	       event->area.x, event->area.y,
	       event->area.width, event->area.height));
	       
  gtk_image_viewer_expose_area(image_viewer,
			       event->area.x, event->area.y,
			       event->area.width, event->area.height);
  
  return FALSE;
}

gint
gtk_image_viewer_redraw(GtkImageViewer *image_viewer)
{
  if (GTK_WIDGET(image_viewer)->window)
    gtk_image_viewer_expose_area(image_viewer,
				 0,0,
                                 GTK_WIDGET(image_viewer)->allocation.width,
                                 GTK_WIDGET(image_viewer)->allocation.height);
  return FALSE;
}

static gint
gtk_image_viewer_button_press (GtkWidget      *widget,
			      GdkEventButton *event)
{
  int button = event->button;
  int x = event->x;
  int y = event->y;
  GtkImageViewer *image_viewer;
  image_viewer = GTK_IMAGE_VIEWER (widget);

  if (button == 1)
    zoom_in(widget, x, y);
  else if (button == 2)
    {
      gtk_grab_add (GTK_WIDGET(image_viewer));
      image_viewer->is_mouse_button2_pressed = TRUE;
      image_viewer->last_pan_anchor_x = x;
      image_viewer->last_pan_anchor_y = y;
    }
  else if (button == 3)
    zoom_out(widget, x, y);
  
  image_viewer->button = event->button;
  
  return FALSE;
}

static gint
gtk_image_viewer_scroll (GtkWidget      *widget,
                         GdkEventScroll *event)
{
  /* Add the following in order only to get control scroll.
  if (event->state & GDK_CONTROL_MASK)
   */
  {
    int x = event->x;
    int y = event->y;
    if (event->direction)
      zoom_in(widget, x,y);
    else
      zoom_out(widget, x,y);
    return 1;
  }
  return 0;
}

static gint
gtk_image_viewer_button_release (GtkWidget      *widget,
				GdkEventButton *event)
{
  GtkImageViewer *image_viewer;
  int x = event->x;
  int y = event->y;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_IMAGE_VIEWER (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  image_viewer = GTK_IMAGE_VIEWER (widget);

  if (image_viewer->button == event->button)
    {
      if (image_viewer->is_mouse_button2_pressed) {
	image_viewer->is_mouse_button2_pressed = FALSE;
	if (image_viewer->last_pan_anchor_x>0 && image_viewer->last_pan_anchor_y > 0)
	    zoom_translate(widget, image_viewer->last_pan_anchor_x-x,
			   image_viewer->last_pan_anchor_y-y);
	gtk_grab_remove (GTK_WIDGET(image_viewer));

	/* Emit an additional view changed signal at the end of the scrolling
	   in order to support redrawing only when the panning is done.
	*/
	gtk_signal_emit(GTK_OBJECT(image_viewer), gtk_image_viewer_signals[VIEW_CHANGED]);
      }
    }

  return FALSE;
}

/*======================================================================
//  The motion callback should scroll the image. The current solution
//  unfortunately does not create smooth scrolling. A further
//  investigation of how e.g. gimp does it is needed.
//----------------------------------------------------------------------*/
static gint
gtk_image_viewer_motion_notify (GtkWidget      *widget,
				GdkEventMotion *event)
{
  GtkImageViewer *image_viewer;
  GdkModifierType state;
  gint x, y;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_IMAGE_VIEWER (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  image_viewer = GTK_IMAGE_VIEWER (widget);

  if (event->is_hint) {
     gdk_window_get_pointer(event->window, &x, &y, &state);
  } else {
     x = (int) event->x;
     y = (int) event->y;
  }
  image_viewer->last_x = x;
  image_viewer->last_y = y;

  if (image_viewer->is_mouse_button2_pressed)
    {
      if (image_viewer->last_pan_anchor_x>0 && image_viewer->last_pan_anchor_y > 0)
	zoom_translate(widget, image_viewer->last_pan_anchor_x-x,
		       image_viewer->last_pan_anchor_y-y);
      
      image_viewer->last_pan_anchor_x = x;
      image_viewer->last_pan_anchor_y = y;

      return TRUE;
    }
  
  return FALSE;
}

static gint
gtk_image_viewer_key_press (GtkWidget      *widget,
			    GdkEventKey *event)
{
  int ret = TRUE;
  GtkImageViewer *image_viewer;
  gint k;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_IMAGE_VIEWER (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  image_viewer = GTK_IMAGE_VIEWER (widget);

  k = event->keyval;
    
  if (k == '=' || k == '>' || k == '+')
    {
      zoom_in(widget, image_viewer->last_x, image_viewer->last_y);
    }
  else if (k == '<' || k == '-')
    zoom_out(widget, 0, 0);
  else if (k == '1' || k == 'n')
    zoom_reset(widget);
  else if (k == 'v')
    {
      image_viewer->do_flip_vertical = !image_viewer->do_flip_vertical;
      gtk_image_viewer_redraw(GTK_IMAGE_VIEWER(widget));
    }
  else if (k == 'h')
    {
      image_viewer->do_flip_horizontal = !image_viewer->do_flip_horizontal;
      gtk_image_viewer_redraw(GTK_IMAGE_VIEWER(widget));
    }
  else
    ret = FALSE;
  return ret;
}

static gint gtk_image_viewer_enter_notify(GtkWidget *widget,
					  GdkEventCrossing *event)
{
  if (!GTK_WIDGET_HAS_FOCUS (widget))
    gtk_widget_grab_focus (widget);

  return TRUE;
}

static gint gtk_image_viewer_leave_notify(GtkWidget *widget,
					  GdkEventCrossing *event)
{
  return TRUE;
}

static gint gtk_image_viewer_focus_in(GtkWidget *widget,
				      GdkEventFocus *event)
{
  return TRUE;
}

static gint gtk_image_viewer_focus_out(GtkWidget *widget,
				       GdkEventFocus *event)
{
  return TRUE;
}

/*======================================================================
//  Accessor related functions.
//----------------------------------------------------------------------*/
guint
gtk_image_viewer_set_image(GtkImageViewer *widget,
			   GdkPixbuf *_pixbuf)
{
  GtkImageViewer *image_viewer;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_IMAGE_VIEWER (widget), FALSE);

  image_viewer = GTK_IMAGE_VIEWER (widget);

  if (widget->image)
    gdk_pixbuf_unref(widget->image);

  if (_pixbuf)
    image_viewer->image = gdk_pixbuf_ref(_pixbuf);
  else
    image_viewer->image = NULL;

  /* Call view_changed with the same parameters, but with a forced
     redraw. This will cause the newly drawn image to be centered
     even if it is a different size.
   */
  view_changed(image_viewer, TRUE,
	       image_viewer->current_scale_x,
	       image_viewer->current_scale_y,
	       image_viewer->current_x0,
	       image_viewer->current_y0);
  
  return 0;
}

guint
gtk_image_viewer_set_file(GtkImageViewer *widget,
			  const gchar *filename)
{
  GdkPixbuf *image = NULL;
  GError *error = NULL;

  image = gdk_pixbuf_new_from_file(filename, &error);
  gtk_image_viewer_set_image(widget, image);
  if (image)
    gdk_pixbuf_unref(image);

  return 0;
}

guint
gtk_image_viewer_set_zoom_range(GtkImageViewer *widget,
				double min, double max)
{
  GtkImageViewer *image_viewer;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_IMAGE_VIEWER (widget), FALSE);

  image_viewer = GTK_IMAGE_VIEWER (widget);

  image_viewer->min_zoom = min;
  image_viewer->max_zoom = max;

  return 1;
}

gboolean
gtk_image_viewer_get_is_panning(GtkImageViewer *widget)
{
  return widget->is_mouse_button2_pressed;
}

guint
gtk_image_viewer_get_canvas_width(GtkImageViewer *widget)
{
  return widget->canvas_width;
}

guint
gtk_image_viewer_get_canvas_height(GtkImageViewer *widget)
{
  return widget->canvas_height;
}

void
gtk_image_viewer_get_scale(GtkImageViewer *widget,
			   /* output */
			   double *scale_x,
			   double *scale_y
			   )
{
  *scale_x = widget->current_scale_x;
  *scale_y = widget->current_scale_y;
}

GdkPixbuf *
gtk_image_viewer_get_image(GtkImageViewer *widget)
{
  return widget->image;
}

guint
gtk_image_viewer_get_image_width(GtkImageViewer *widget)
{ 
  return gdk_pixbuf_get_width (widget->image);
}

guint
gtk_image_viewer_get_image_height(GtkImageViewer *widget)
{ 
  return gdk_pixbuf_get_height (widget->image);
}

/*======================================================================
// gtk object system support...
//----------------------------------------------------------------------*/
static void
gtk_image_viewer_set_property (GObject         *object,
			       guint            prop_id,
			       const GValue    *value,
			       GParamSpec      *pspec)
{
  GtkImageViewer *image_viewer;

  image_viewer = GTK_IMAGE_VIEWER(object);

  switch(prop_id)
  {
  case PROP_FILE:
    gtk_image_viewer_set_file(image_viewer, g_value_get_string(value));
    break;
  case PROP_USE_LINEAR_ZOOM_STEPS:
    gtk_image_viewer_set_use_linear_zoom_steps(image_viewer, g_value_get_int(value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
gtk_image_viewer_get_property (GObject         *object,
			       guint            prop_id,
			       GValue          *value,
			       GParamSpec      *pspec)
{
  GtkImageViewer *image_viewer;

  image_viewer = GTK_IMAGE_VIEWER(object);

  switch(prop_id)
  {
  case PROP_IMAGE_WIDTH:
    g_value_set_int(value, gtk_image_viewer_get_image_width(image_viewer));
    break;
  case PROP_IMAGE_HEIGHT:
    g_value_set_int(value, gtk_image_viewer_get_image_height(image_viewer));
    break;
  case PROP_USE_LINEAR_ZOOM_STEPS:
    g_value_set_int(value, gtk_image_viewer_get_use_linear_zoom_steps(image_viewer));
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

/*======================================================================
//  Zoom related functions.
//----------------------------------------------------------------------*/
static gint
zoom_reset(GtkWidget *widget)
{
    view_changed(GTK_IMAGE_VIEWER(widget), TRUE, 1, 1, 0, 0);
    return 1;
}

static gint
zoom_in(GtkWidget *widget, int x, int y)
{
  GtkImageViewer *image_viewer = GTK_IMAGE_VIEWER (widget);
  double zoom_factor[2];
  int zoom_idx;

  zoom_factor[0] = image_viewer->current_scale_x;
  zoom_factor[1] = image_viewer->current_scale_y;

  for (zoom_idx = 0; zoom_idx < 2; zoom_idx++)
    {
      double z = zoom_factor[zoom_idx];  /* A shortcut */
      if (image_viewer->do_linear_zoom_steps) {
	if (z < 0.5)
	  z = 1.0 / (1.0/z-1);
	else
	  z = z+1;
      }
      else 
	z *= 2;
      zoom_factor[zoom_idx] = z;
    }

  gtk_image_viewer_zoom_around_fixed_point(image_viewer,
					   zoom_factor[0],
					   zoom_factor[1],
					   x, y,
					   image_viewer->canvas_width/2,
					   image_viewer->canvas_height/2);

  return 1;
}

static gint
zoom_out(GtkWidget *widget, int x, int y)
{
  GtkImageViewer *image_viewer = GTK_IMAGE_VIEWER (widget);

  double zoom_factor[2];
  int zoom_idx;

  zoom_factor[0] = image_viewer->current_scale_x;
  zoom_factor[1] = image_viewer->current_scale_y;

  for (zoom_idx = 0; zoom_idx < 2; zoom_idx++)
    {
      double z = zoom_factor[zoom_idx];  /* A shortcut */
      if (image_viewer->do_linear_zoom_steps) {
	if (z <= 1.0)
	  z = 1.0 / (1.0/z+1);
	else
	  z = z-1;
      }
      else 
	z /= 2.0;
      zoom_factor[zoom_idx] = z;
    }

  gtk_image_viewer_zoom_around_fixed_point(image_viewer,
					   zoom_factor[0],
					   zoom_factor[1],
					   image_viewer->canvas_width/2,
					   image_viewer->canvas_height/2,
					   image_viewer->canvas_width/2,
					   image_viewer->canvas_height/2);
  return 1;
}

static gint
zoom_translate(GtkWidget *widget, int dx, int dy)
{
  GtkImageViewer *image_viewer = GTK_IMAGE_VIEWER (widget);
  
  view_changed(image_viewer,
	       FALSE,
	       image_viewer->current_scale_x,
	       image_viewer->current_scale_y,
	       image_viewer->current_x0+dx,
	       image_viewer->current_y0+dy);
  return 1;
}

/*======================================================================
//  Expose an area. x0 y0 x1 and y1 are in pixel coordinates.
//----------------------------------------------------------------------*/
void
gtk_image_viewer_expose_area(GtkImageViewer *image_viewer,
			     int exp_x0, int exp_y0,
			     int w, int h)
{
  GtkWidget *widget = GTK_WIDGET(image_viewer); /* In order to access window */
  GdkPixbuf *img_scaled;
  double scale_x = image_viewer->current_scale_x;
  double scale_y = image_viewer->current_scale_y;
  int exp_x1 = exp_x0 + w;
  int exp_y1 = exp_y0 + h;

  DBG(fprintf(stderr, "expose: %d %d %d %d\n", exp_x0, exp_y0, w, h));
  if (image_viewer->image)
    {
				/* Canvas size */
      int cnv_w = image_viewer->canvas_width;
      int cnv_h = image_viewer->canvas_height;
      int copy_w, copy_h;
      int img_w, img_h;		/* Image's size (pixels) */
      int offs_x, offs_y;
      int dst_x, dst_y;
				/* Canvas coordinates in which drawing
				   is done. */
      int img_x0 = 0;
      int img_x1 = cnv_w;
      int img_y0 = 0;
      int img_y1 = cnv_h;
      dst_x = dst_y = 0;
      copy_w = w;
      copy_h = h;
      img_w = gdk_pixbuf_get_width(image_viewer->image);
      img_h = gdk_pixbuf_get_height(image_viewer->image);
      offs_x = -image_viewer->current_x0-exp_x0;
      offs_y = -image_viewer->current_y0-exp_y0;

      /* If img fits in canvas horizontally, need a smaller drawing zone */
      if (img_w * scale_x < cnv_w) 
	{
	  img_x0 = (cnv_w - img_w*scale_x)/2;
	  img_x1 = img_x0 + img_w*scale_x;
	}

      /* If img fits in canvas vertically : need a smaller drawing zone */
      if (img_h * scale_y < cnv_h)
	{
	  img_y0 = (cnv_h - img_h*scale_y)/2;
	  img_y1 = img_y0 + img_h*scale_y;
	}

      if (exp_x1 < img_x0)
	{
	  DBG(fprintf(stderr, "Case 1X\n"));
	  copy_w = 0;
	  offs_x = 0;
	  dst_x = img_x0;
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     exp_x0,exp_y0,w, h);
	}
      else if (exp_x0 < img_x0 && exp_x1 > img_x1)
	{
	  DBG(fprintf(stderr, "Case 2X\n"));
	  copy_w = w - (img_x0 - exp_x0) - (exp_x1 - img_x1);
	  dst_x = img_x0;
	  offs_x = 0;
	  
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     exp_x0,exp_y0,(img_x0-exp_x0), h);
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     img_x1,exp_y0,(exp_x1-img_x1), h);
	}
      else if (exp_x0 < img_x0)
	{
	  DBG(fprintf(stderr, "Case 3X\n"));
	  offs_x = 0;
	  copy_w = w - (img_x0 - exp_x0);
	  dst_x = img_x0;
	  
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     exp_x0,exp_y0,(img_x0-exp_x0), h);
	}
      else if (exp_x0 > img_x1)
	{
	  DBG(fprintf(stderr, "Case 4X\n"));
	  copy_w = 0;
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     exp_x0,exp_y0,w, h);
	  
	}
      else if (exp_x1 > img_x1)
	{
	  DBG(fprintf(stderr, "Case 5X\n"));
	  copy_w = w - (exp_x1 - img_x1);
	  dst_x = exp_x0;
	  offs_x = -(exp_x0-img_x0);
	  
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     img_x1,exp_y0,exp_x1-img_x1, h);
	}
      else
	{
	  DBG(fprintf(stderr, "Case 6X\n"));
	  dst_x = exp_x0;
	  copy_w = w;
	}

      /* Y dir */
      if (exp_y1 < img_y0)
	{
	  DBG(fprintf(stderr, "Case 1Y\n"));
	  copy_h = 0;
	  offs_y = 0;
	  dst_x = img_y0;
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     exp_x0,exp_y0,w, h);
	}
      else if (exp_y0 < img_y0 && exp_y1 > img_y1)
	{
	  DBG(fprintf(stderr, "Case 2Y\n"));
	  copy_h = h - (img_y0 - exp_y0) - (exp_y1 - img_y1);
	  dst_y = img_y0;
	  offs_y = 0;
	  
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     exp_x0,exp_y0,w, img_y0-exp_y0);
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     exp_x0, img_y1,w, exp_y1-img_y1);
	}
      else if (exp_y0 < img_y0)
	{
	  DBG(fprintf(stderr, "Case 3Y\n"));
	  offs_y = 0;
	  copy_h = h - (img_y0 - exp_y0);
	  dst_y = img_y0;
	  
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     exp_x0,exp_y0,w,(img_y0-exp_y0));
	}
      else if (exp_y0 > img_y1)
	{
	  DBG(fprintf(stderr, "Case 4Y\n"));
	  copy_h = 0;
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     exp_x0,exp_y0,w, h);
	  
	}
      else if (exp_y1 > img_y1)
	{
	  DBG(fprintf(stderr, "Case 5Y\n"));
	  copy_h = h - (exp_y1 - img_y1);
	  dst_y = exp_y0;
	  offs_y = -(exp_y0-img_y0);
	  DBG(fprintf(stderr, "copy_h dst_y = %d %d\n", copy_h, dst_y));
	  gdk_draw_rectangle(widget->window,
			     widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			     TRUE,
			     exp_x0,img_y1,w,exp_y1-img_y1);
	}
      else
	{
	  DBG(fprintf(stderr, "Case 6Y\n"));
	  dst_y = exp_y0;
	  copy_h = h;
	}
      

      DBG(fprintf(stderr, "dst_x dst_y copy_w copy_h offs_x offs_y = %d %d %d %d %f %f\n",
	      dst_x, dst_y, copy_w, copy_h,
	      image_viewer->current_x0, image_viewer->current_y0));
      /* Scale and copy the image */
      if (copy_w > 0 && copy_h > 0)
	{
	  img_scaled = gdk_pixbuf_new(gdk_pixbuf_get_colorspace(image_viewer->image),
				      FALSE,
				      gdk_pixbuf_get_bits_per_sample(image_viewer->image),
				      copy_w, copy_h);

          if (image_viewer->do_flip_vertical) 
            offs_y = -(gdk_pixbuf_get_height(image_viewer->image) * scale_y + offs_y - copy_h);

          if (image_viewer->do_flip_horizontal) 
            offs_x = -(gdk_pixbuf_get_width(image_viewer->image) * scale_x + offs_x - copy_w);

	  /* Check legality */
	  DBG(fprintf(stderr, "offs_y copy_h scale_y h = %d %d %f %d\n",
		      offs_y, copy_h, scale_y, img_h));
	  gdk_pixbuf_scale(image_viewer->image,
			   img_scaled,
			   0,0,
			   copy_w,
			   copy_h,
			   offs_x,
			   offs_y,
			   scale_x, scale_y,
			   image_viewer->interp_type);
	  
	  if (image_viewer->do_use_transfer_map)
	    {
	      /* Assume we are doing 8-bit... */
	      guint8 *buf_scaled = gdk_pixbuf_get_pixels(img_scaled);
	      gint w = gdk_pixbuf_get_width(img_scaled);
	      gint h = gdk_pixbuf_get_height(img_scaled);
	      gint rs = gdk_pixbuf_get_rowstride(img_scaled);
	      gint row_idx, col_idx;
	      guint8 *rmap = image_viewer->transfer_map[0];
	      guint8 *gmap = image_viewer->transfer_map[1];
	      guint8 *bmap = image_viewer->transfer_map[2];
	      
	      for (row_idx=0; row_idx<h; row_idx++)
		{
		  guint8 *p = &buf_scaled[rs*row_idx];
		  
		  for (col_idx=0; col_idx<w; col_idx++)
		    {
		      *p = rmap[*p]; p++;
		      *p = gmap[*p]; p++;
		      *p = bmap[*p]; p++;
		    }
		}
	    }

          if (image_viewer->do_flip_vertical)
            {
	      /* Assume we are doing 8-bit... */
	      guint8 *buf_scaled = gdk_pixbuf_get_pixels(img_scaled);
	      gint w = gdk_pixbuf_get_width(img_scaled);
	      gint h = gdk_pixbuf_get_height(img_scaled);
	      gint rs = gdk_pixbuf_get_rowstride(img_scaled);
	      gint row_idx, col_idx;
	      
	      for (row_idx=0; row_idx<h/2; row_idx++)
		{
                  guint8 *ptr1 = buf_scaled+rs * row_idx;
                  guint8 *ptr2 = buf_scaled+rs * (h-row_idx-1);
		  
		  for (col_idx=0; col_idx<w; col_idx++)
		    {
                      guint8 tmp_r = *ptr1;
                      guint8 tmp_g = *(ptr1+1);
                      guint8 tmp_b = *(ptr1+2);
                      *ptr1++ = *ptr2;
                      *ptr1++ = *(ptr2+1);
                      *ptr1++ = *(ptr2+2);
                      *ptr2++ = tmp_r;
                      *ptr2++ = tmp_g;
                      *ptr2++ = tmp_b;
		    }
		}
            }

          if (image_viewer->do_flip_horizontal)
            {
	      /* Assume we are doing 8-bit... */
	      guint8 *buf_scaled = gdk_pixbuf_get_pixels(img_scaled);
	      gint w = gdk_pixbuf_get_width(img_scaled);
	      gint h = gdk_pixbuf_get_height(img_scaled);
	      gint rs = gdk_pixbuf_get_rowstride(img_scaled);
	      gint row_idx, col_idx, clr_idx;
	      
              for (col_idx = 0; col_idx < w/2; col_idx++)
                {
                  for (row_idx = 0; row_idx <h; row_idx++)
                    {
                      int l_idx = row_idx * rs + col_idx*3;
                      int r_idx = row_idx * rs + (w - col_idx - 1)*3;
                      
                      for (clr_idx=0; clr_idx<3; clr_idx++)
                        {
                          guint8 tmp = buf_scaled[l_idx+clr_idx];
                          buf_scaled[l_idx+clr_idx] = buf_scaled[r_idx+clr_idx];
                          buf_scaled[r_idx+clr_idx] = tmp;
                        }
                    }
                }
            }

	  gdk_pixbuf_render_to_drawable (img_scaled,
					 widget->window,
					 widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
					 0, 0,
					 dst_x, dst_y,
					 copy_w, copy_h,
					 GDK_RGB_DITHER_NORMAL,
					 image_viewer->current_x0,
					 image_viewer->current_y0);
	  
	  gdk_pixbuf_unref(img_scaled);
	}
    }
  else
    {
      gdk_draw_rectangle(widget->window,
			 widget->style->bg_gc[GTK_WIDGET_STATE (widget)],
			 TRUE,
			 exp_x0,exp_y0,w, h);
    }

  gtk_signal_emit(GTK_OBJECT(image_viewer), gtk_image_viewer_signals[VIEW_CHANGED]);
  update_adjustments(image_viewer);
}


/*======================================================================
//  gtk_image_viewer_zoom_around_fixed_point is a preprocessor
//  to view_changed that allows a zooming while moving an old pixel
//  coordinate to a given new position. E.g. moving the x,y coordinate
//  clicked by the mouse to the center of the zoomed up image.
//----------------------------------------------------------------------*/
gint
gtk_image_viewer_zoom_around_fixed_point(GtkImageViewer *image_viewer,
					 double new_scale_x,
					 double new_scale_y,
					 double old_x,
					 double old_y,
					 double new_x,
					 double new_y)
{
  double old_scale_x, old_scale_y, old_x0, old_y0, new_x0, new_y0;

  g_return_val_if_fail (image_viewer != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_IMAGE_VIEWER (image_viewer), FALSE);

  /* Short cuts */
  old_x0 = image_viewer->current_x0;
  old_y0 = image_viewer->current_y0;
  old_scale_x = image_viewer->current_scale_x;
  old_scale_y = image_viewer->current_scale_y;

  new_x0 = new_scale_x/old_scale_x * (old_x + old_x0) - new_x;
  new_y0 = new_scale_y/old_scale_y * (old_y + old_y0) - new_y;

  view_changed(image_viewer, FALSE, new_scale_x, new_scale_y, new_x0, new_y0);

  return 0;
}

/*======================================================================
//  view_changed does clipping, scrolling and scaling.
//----------------------------------------------------------------------
*/
static gint
view_changed(GtkImageViewer *image_viewer,
	     int do_force,
	     double scale_x, double scale_y, double x0, double y0)
{
  GtkWidget *widget = GTK_WIDGET(image_viewer);
  GdkPixbuf *im;
  int render_width, render_height;
  GdkRectangle expose_rect;

  if (!widget || !widget->window)
    return 0;
  
  g_return_val_if_fail (image_viewer != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_IMAGE_VIEWER (image_viewer), FALSE);

  widget = GTK_WIDGET(image_viewer);

  im = image_viewer->image; /* A short cut */

  DBG2(g_print("force sx sy x0 y0 = %d %f %f %f %f\n",
	       do_force, scale_x, scale_y, x0, y0));

  /* This should certainly be configurable */
  if (scale_x < 1.0)
    image_viewer->interp_type = GDK_INTERP_BILINEAR;
  else
    image_viewer->interp_type = GDK_INTERP_NEAREST;

  /* Clip the scale */
  if (scale_x>image_viewer->max_zoom && scale_y > image_viewer->max_zoom)
    return 0;
  else if (scale_x < image_viewer->min_zoom && scale_y < image_viewer->min_zoom)
    return 0;

  /* Clip the request */
  if (im
      || (image_viewer->scroll_width > -1 && image_viewer->scroll_height > -1)
      || (image_viewer->scroll_max_x - image_viewer->scroll_min_x > 0
          && image_viewer->scroll_max_y - image_viewer->scroll_min_y > 0)
      )
    {
      int cwidth = image_viewer->canvas_width;
      int cheight = image_viewer->canvas_height;
      int height, width;

      if (image_viewer->scroll_width > -1) {
	  width = image_viewer->scroll_width;
	  height = image_viewer->scroll_height;
      }
      else if (image_viewer->scroll_max_x - image_viewer->scroll_min_x > 0)
        {
          width = image_viewer->scroll_max_x - image_viewer->scroll_min_x;
          height = image_viewer->scroll_max_y - image_viewer->scroll_min_y;
        }
      else {
	  width = gdk_pixbuf_get_width(im);
	  height = gdk_pixbuf_get_height(im);
      }

      if (im && width*scale_x > cwidth)
	  render_width = cwidth;
      else
	  render_width = width * scale_x;
      
      if (im && height*scale_y > cheight)
	  render_height = cheight;
      else
	  render_height = height * scale_y;

      // Clip only for images
      if (im)
        {
          if (render_width < cwidth)
            x0 = -(cwidth - render_width)/2;
          else if (x0 + render_width > width*scale_x)
            x0 = width*scale_x - render_width;
          else if (x0<0)
            x0 = 0;
          
          if (im && render_height < cheight)
            y0 = -(cheight - render_height)/2;
          else if (y0 + render_height > height*scale_y)
            y0 = height*scale_y - render_height;
          else if (y0<0)
            y0 = 0;
        }
    }

  /* If scale is the same, then the image has only been scrolled,
     and we only need to update the exposed areas.
   */
  if (!do_force
      && widget->window
      && scale_x == image_viewer->current_scale_x
      && scale_y == image_viewer->current_scale_y
      )
    {
      int dx = (x0 - image_viewer->current_x0);
      int dy = (y0 - image_viewer->current_y0);
      int src_x = (dx < 0) ? 0 : dx;
      int src_y = (dy < 0) ? 0 : dy;
      int dst_x = (dx < 0) ? -dx : 0;
      int dst_y = (dy < 0) ? -dy : 0;

      /* Construct the gc the first time it is used */
      if (!image_viewer->gc)
        {
	  image_viewer->gc = gdk_gc_new(widget->window);
	  gdk_gc_copy(image_viewer->gc, widget->style->white_gc);
	  gdk_gc_set_exposures(image_viewer->gc, TRUE);
	}

      image_viewer->current_x0 = x0;
      image_viewer->current_y0 = y0;

      /* Scroll visible region */
      gdk_draw_pixmap (widget->window,
		       image_viewer->gc,
		       widget->window,
		       src_x, src_y,
		       dst_x, dst_y,
		       image_viewer->canvas_width - abs (dx),
		       image_viewer->canvas_height - abs (dy));

      DBG2(g_print("Filling in: dx dy = %d %d\n", dx, dy));
      /* And fill in the new areas */
      if (dx)
	{
	    expose_rect.x = (dx < 0) ? 0 : image_viewer->canvas_width - dx;
	    expose_rect.y = 0;
	    expose_rect.width = abs(dx);
	    expose_rect.height = image_viewer->canvas_height;

	    gdk_window_invalidate_rect(widget->window,
				       &expose_rect,
				       FALSE);
	    gdk_window_process_updates(widget->window,
				       FALSE);

	}
      if (dy)
	{
	    expose_rect.x = 0;
	    expose_rect.y = (dy < 0) ? 0 : image_viewer->canvas_height - dy;;
	    expose_rect.width = image_viewer->canvas_width;
	    expose_rect.height = abs(dy);

	    gdk_window_invalidate_rect(widget->window,
				       &expose_rect,
				       FALSE);
	    gdk_window_process_updates(widget->window,
				       FALSE);

	}
    }
  
  else if (widget->window &&
	   (do_force
      || scale_x != image_viewer->current_scale_x
      || scale_y != image_viewer->current_scale_y
      || x0 != image_viewer->current_x0
      || y0 != image_viewer->current_y0))
    {
      /* Remember the current transform */
      image_viewer->current_scale_x = scale_x;
      image_viewer->current_scale_y = scale_y;
      image_viewer->current_x0 = x0;
      image_viewer->current_y0 = y0;

      expose_rect.x = 0;
      expose_rect.y = 0;
      expose_rect.width = image_viewer->canvas_width;
      expose_rect.height = image_viewer->canvas_height;

      /* gdk_window_ref (expose_event.window); */
      gdk_window_invalidate_rect(widget->window,
				 &expose_rect,
				 TRUE);
      gdk_window_process_updates(widget->window,
				 TRUE
				 );
    }

  return 1;
}

/*======================================================================
//  Utility functions for converting between the image coordinates
//  and the canvas coordinate.
//----------------------------------------------------------------------*/
void
gtk_image_viewer_img_coord_to_canv_coord(GtkImageViewer *image_viewer,
					 double imgx, double imgy,
					 /* output */
					 double *cx, double *cy)
{
  double x0 = image_viewer->current_x0;
  double y0 = image_viewer->current_y0;
  double sx = image_viewer->current_scale_x;
  double sy = image_viewer->current_scale_y;

  if (image_viewer->do_flip_horizontal)
    {
      if (image_viewer->image)
        imgx = gdk_pixbuf_get_width(image_viewer->image) - imgx;
      else
        imgx = image_viewer->scroll_max_x - (imgx - image_viewer->scroll_min_x);
    }

  if (image_viewer->do_flip_vertical)
    {
      // TBD - Set scroll_max_y so that it may always be used even
      // if there is an image.
      if (image_viewer->image)
        imgy = gdk_pixbuf_get_height(image_viewer->image) - imgy;
      else
        imgy = image_viewer->scroll_max_y - (imgy - image_viewer->scroll_min_y);
    }

  *cx = imgx * sx - x0;
  *cy = imgy * sy - y0;
}

void
gtk_image_viewer_canv_coord_to_img_coord(GtkImageViewer *image_viewer,
					 double cx, double cy,
					 /* output */
					 double *imgx, double *imgy)
{
  double x0 = image_viewer->current_x0;
  double y0 = image_viewer->current_y0;
  double sx = image_viewer->current_scale_x;
  double sy = image_viewer->current_scale_y;

  *imgx  = (cx + x0) / sx;
  *imgy  = (cy + y0) / sy;

  if (image_viewer->do_flip_horizontal)
    {
      if (image_viewer->image)
        *imgx = gdk_pixbuf_get_width(image_viewer->image) - *imgx;
      else
        *imgx = image_viewer->scroll_max_x - (*imgx - image_viewer->scroll_min_x);
    }

  if (image_viewer->do_flip_vertical)
    {
      if (image_viewer->image)
        *imgy = gdk_pixbuf_get_height(image_viewer->image) - *imgy;
      else
        *imgy = image_viewer->scroll_max_y - (*imgy - image_viewer->scroll_min_y);
    }
}

/*======================================================================
//  Set the transfer function for the display. Note that the widget
//  does not own or replicate the memory of the maps.
//----------------------------------------------------------------------*/
void
gtk_image_viewer_set_transfer_map(GtkImageViewer *widget,
				  guint8 *rmap,
				  guint8 *gmap,
				  guint8 *bmap
				  )
{
  GtkImageViewer *image_viewer = GTK_IMAGE_VIEWER(widget);

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (widget));
  
  image_viewer->transfer_map[0] = rmap;
  image_viewer->transfer_map[1] = gmap;
  image_viewer->transfer_map[2] = bmap;
  image_viewer->do_use_transfer_map = TRUE;

  gtk_image_viewer_redraw(widget);
}

void
gtk_image_viewer_set_flip(GtkImageViewer *image_viewer,
                          gboolean do_flip_horizontal,
                          gboolean do_flip_vertical)
{
  gboolean need_redraw =
    do_flip_horizontal != image_viewer->do_flip_horizontal
    || do_flip_horizontal != image_viewer->do_flip_vertical;

  image_viewer->do_flip_horizontal = do_flip_horizontal;
  image_viewer->do_flip_vertical = do_flip_vertical;

  if (need_redraw)
    gtk_image_viewer_redraw(image_viewer);
}

gboolean
gtk_image_viewer_get_use_linear_zoom_steps(GtkImageViewer *widget)
{
  GtkImageViewer *image_viewer = GTK_IMAGE_VIEWER(widget);

  return image_viewer->do_linear_zoom_steps;
}

void
gtk_image_viewer_set_use_linear_zoom_steps(GtkImageViewer *widget,
					   gboolean do_use_linear_zoom_steps)
{
  GtkImageViewer *image_viewer = GTK_IMAGE_VIEWER(widget);

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (widget));

  image_viewer->do_linear_zoom_steps = do_use_linear_zoom_steps;
}

void           gtk_image_viewer_get_transform(GtkImageViewer *widget,
					      /* output */
					      double *x,
					      double *y,
					      double *scale_x,
					      double *scale_y)
{
    *x = widget->current_x0;
    *y = widget->current_y0;
    *scale_x = widget->current_scale_x;
    *scale_y = widget->current_scale_y;
}

void           gtk_image_viewer_set_transform(GtkImageViewer *widget,
					      double x,
					      double y,
					      double scale_x,
					      double scale_y)
{
    view_changed(widget, FALSE, scale_x, scale_y, x,y);
}

void           gtk_image_viewer_set_max_zoom(GtkImageViewer *widget,
					     double max_zoom)
{
  widget->max_zoom = max_zoom;
}

void           gtk_image_viewer_set_min_zoom(GtkImageViewer *widget,
					     double min_zoom)
{
  widget->min_zoom = min_zoom;
}

double         gtk_image_viewer_get_min_zoom(GtkImageViewer *widget)
{
  return widget->min_zoom;
}

double         gtk_image_viewer_get_max_zoom(GtkImageViewer *widget)
{
  return widget->max_zoom;
}

int         gtk_image_viewer_get_scroll_width(GtkImageViewer *widget)
{
  return widget->scroll_width;
}

int         gtk_image_viewer_get_scroll_height(GtkImageViewer *widget)
{
  return widget->scroll_height;
}

void         gtk_image_viewer_set_scroll_height(GtkImageViewer *widget,
						int scroll_height
						)
{
  widget->scroll_height = scroll_height;
}

void         gtk_image_viewer_set_scroll_width(GtkImageViewer *widget,
						int scroll_width
						)
{
  widget->scroll_width = scroll_width;
}

int            gtk_image_viewer_set_scroll_region(GtkImageViewer *widget,
                                                  double x0, double y0,
                                                  double x1, double y1)
{
  widget->scroll_min_x = x0;
  widget->scroll_min_y = y0;
  widget->scroll_max_x = x1;
  widget->scroll_max_y = y1;

  return 0;
}


void           gtk_image_viewer_zoom_rel(GtkImageViewer *widget,
					 double rel_zoomfactor)
{
  double width = gtk_image_viewer_get_canvas_width(widget);
  double height = gtk_image_viewer_get_canvas_height(widget);

  gtk_image_viewer_zoom_around_fixed_point(widget,
					   widget->current_scale_x * rel_zoomfactor,
					   widget->current_scale_y * rel_zoomfactor,
					   width/2, height/2,
					   width/2, height/2);
}

void           gtk_image_viewer_zoom_fit(GtkImageViewer *widget,
					 gboolean do_keep_aspect_ratio)
{
  double x_canvas_img_ratio, y_canvas_img_ratio;
  double x_scale, y_scale;

  x_canvas_img_ratio = 1.0 * widget->canvas_width / gdk_pixbuf_get_width(widget->image);
  y_canvas_img_ratio = 1.0 * widget->canvas_height / gdk_pixbuf_get_height(widget->image);

  if (do_keep_aspect_ratio)
    {
      if (x_canvas_img_ratio < y_canvas_img_ratio)
	x_scale = y_scale = x_canvas_img_ratio;
      else
	x_scale = y_scale = y_canvas_img_ratio;
    }
  else
    {
      x_scale = x_canvas_img_ratio;
      y_scale = y_canvas_img_ratio;
    }

  gtk_image_viewer_zoom_around_fixed_point(widget,
					   x_scale,
					   y_scale,
					   widget->canvas_width/2,
					   widget->canvas_height/2,
					   widget->canvas_width/2,
					   widget->canvas_height/2);
  
}

void           gtk_image_viewer_zoom_100(GtkImageViewer *widget)
{
  double width = gtk_image_viewer_get_canvas_width(widget);
  double height = gtk_image_viewer_get_canvas_height(widget);

  gtk_image_viewer_zoom_around_fixed_point(widget,
					   1.0,
					   1.0,
					   width/2, height/2,
					   width/2, height/2);
}

/**
 * gtk_image_viewer_set_vadjustment:
 * @image_viewer: a #GtkImage_Viewer.
 * @adjustment: a #GtkAdjustment.
 * 
 * Sets the vertical adjustment of the image_viewer.
 **/
void
gtk_image_viewer_set_vadjustment (GtkImageViewer   *image_viewer,
				  GtkAdjustment *adjustment)
{
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (image_viewer));
  if (adjustment)
    g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  if (image_viewer->vadjustment && image_viewer->vadjustment != adjustment)
    {
      gtk_signal_disconnect_by_data (GTK_OBJECT (image_viewer->vadjustment), image_viewer);
      gtk_object_unref (GTK_OBJECT (image_viewer->vadjustment));
      image_viewer->vadjustment = NULL;
    }

  if (!adjustment)
    adjustment = GTK_ADJUSTMENT (gtk_adjustment_new (0.0, 0.0, 0.0,
						     0.0, 0.0, 0.0));

  if (image_viewer->vadjustment != adjustment)
    {
      image_viewer->vadjustment = adjustment;
      gtk_object_ref (GTK_OBJECT (image_viewer->vadjustment));
      gtk_object_sink (GTK_OBJECT (image_viewer->vadjustment));
      
      gtk_signal_connect (GTK_OBJECT (adjustment), "changed",
			  (GtkSignalFunc) gtk_image_viewer_adjustment_changed,
			  (gpointer) image_viewer);
      gtk_signal_connect (GTK_OBJECT (adjustment), "value_changed",
			  (GtkSignalFunc) gtk_image_viewer_vadjustment_value_changed,
			  (gpointer) image_viewer);

      gtk_image_viewer_adjustment_changed (adjustment, image_viewer);
    }

    g_object_notify (G_OBJECT (image_viewer), "vadjustment");
    update_adjustments(image_viewer);
}


/**
 * gtk_image_viewer_set_hadjustment:
 * @image_viewer: a #GtkImage_Viewer.
 * @adjustment: a #GtkAdjustment.
 * 
 * Sets the horizontal adjustment of the image_viewer.
 **/
void
gtk_image_viewer_set_hadjustment (GtkImageViewer   *image_viewer,
				  GtkAdjustment *adjustment)
{
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (image_viewer));
  if (adjustment)
    g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  if (image_viewer->hadjustment && image_viewer->hadjustment != adjustment)
    {
      gtk_signal_disconnect_by_data (GTK_OBJECT (image_viewer->hadjustment), image_viewer);
      gtk_object_unref (GTK_OBJECT (image_viewer->hadjustment));
      image_viewer->hadjustment = NULL;
    }

  if (!adjustment)
    adjustment = GTK_ADJUSTMENT (gtk_adjustment_new (0.0, 0.0, 0.0,
						     0.0, 0.0, 0.0));

  if (image_viewer->hadjustment != adjustment)
    {
      image_viewer->hadjustment = adjustment;
      gtk_object_ref (GTK_OBJECT (image_viewer->hadjustment));
      gtk_object_sink (GTK_OBJECT (image_viewer->hadjustment));
      
      gtk_signal_connect (GTK_OBJECT (adjustment), "changed",
			  G_CALLBACK(gtk_image_viewer_adjustment_changed),
			  (gpointer) image_viewer);
      gtk_signal_connect (GTK_OBJECT (adjustment), "value_changed",
			  G_CALLBACK(gtk_image_viewer_hadjustment_value_changed),
			  (gpointer) image_viewer);

      gtk_image_viewer_adjustment_changed (adjustment, image_viewer);
    }

  g_object_notify (G_OBJECT (image_viewer), "hadjustment");

  update_adjustments(image_viewer);
}

static void
gtk_image_viewer_set_scroll_adjustments (GtkImageViewer      *image_viewer,
				     GtkAdjustment    *hadjustment,
				     GtkAdjustment    *vadjustment)
{
  if (image_viewer->hadjustment != hadjustment)
    gtk_image_viewer_set_hadjustment (image_viewer, hadjustment);
  if (image_viewer->vadjustment != vadjustment)
    gtk_image_viewer_set_vadjustment (image_viewer, vadjustment);
}


static void
gtk_image_viewer_adjustment_changed (GtkAdjustment *adjustment,
                                     gpointer       data)
{
  GtkImageViewer *image_viewer;

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (data));

  image_viewer = GTK_IMAGE_VIEWER (data);
}

static void
gtk_image_viewer_hadjustment_value_changed (GtkAdjustment *hadjustment,
					    gpointer       data)
{
  GtkImageViewer *image_viewer;
  double min = 0;
  double new_x0, dx;
  int width;

  g_return_if_fail (GTK_IS_ADJUSTMENT (hadjustment));
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (data));

  image_viewer = GTK_IMAGE_VIEWER (data);

  if (image_viewer->scroll_max_x - image_viewer->scroll_min_x > 0)
    {
      min = image_viewer->scroll_min_x;
      width = image_viewer->scroll_max_x - min;
    }
  else if (image_viewer->scroll_width > 0)
    width = image_viewer->scroll_width;
  else if (image_viewer->image)
    width = gtk_image_viewer_get_image_width(image_viewer);
  else
    width = image_viewer->canvas_width;

  new_x0 = (hadjustment->value * width + min) * image_viewer->current_scale_x;
  dx = new_x0 - image_viewer->current_x0;

  if (dx!=0)
    zoom_translate(GTK_WIDGET(data), dx, 0);
}

static void
gtk_image_viewer_vadjustment_value_changed (GtkAdjustment *vadjustment,
					    gpointer       data)
{
  GtkImageViewer *image_viewer;
  double min = 0;
  double new_y0, dy;
  int height;

  g_return_if_fail (GTK_IS_ADJUSTMENT (vadjustment));
  g_return_if_fail (GTK_IS_IMAGE_VIEWER (data));

  image_viewer = GTK_IMAGE_VIEWER (data);

  if (image_viewer->scroll_max_y - image_viewer->scroll_min_y > 0)
    {
      min = image_viewer->scroll_min_y;
      height = image_viewer->scroll_max_y - min;
    }
  else if (image_viewer->scroll_height > 0)
    height = image_viewer->scroll_height;
  else if (image_viewer->image)
    height = gtk_image_viewer_get_image_height(image_viewer);
  else
    height = image_viewer->canvas_height;

  new_y0 = (vadjustment->value * height+min) * image_viewer->current_scale_y;
  dy = new_y0 - image_viewer->current_y0;

  if (dy!=0)
    zoom_translate(GTK_WIDGET(data), 0, dy);
}

// This function should update the adjustment so that they reflect
// the scrolling of the widget as it was decided somewhere else.
static void update_adjustments(GtkImageViewer *image_viewer)
{
  double imgx, imgy;
  double min_x = 0, min_y = 0;
  gint width, height;

  if (!image_viewer->hadjustment
      || !image_viewer->vadjustment
      )
    return;

  if (image_viewer->scroll_max_y - image_viewer->scroll_min_y > 0
      && image_viewer->scroll_max_x - image_viewer->scroll_min_x > 0) 
    {
      width = image_viewer->scroll_max_x - image_viewer->scroll_min_x;
      height = image_viewer->scroll_max_y - image_viewer->scroll_min_y;
      min_x = image_viewer->scroll_min_x;
      min_y = image_viewer->scroll_min_y;
    }
  else if (image_viewer->scroll_width > 0
           && image_viewer->scroll_height > 0)
    {
      width = image_viewer->scroll_width;
      height = image_viewer->scroll_height;
    }
  else if (image_viewer->image)
    {
      width = gtk_image_viewer_get_image_width(image_viewer);
      height = gtk_image_viewer_get_image_height(image_viewer);
    }
  else
    {
      width = image_viewer->canvas_width;
      height = image_viewer->canvas_height;
    }

  imgx = image_viewer->current_x0 / image_viewer->current_scale_x;
  imgy = image_viewer->current_y0 / image_viewer->current_scale_y;

  if (image_viewer->hadjustment)
    {
      image_viewer->hadjustment->lower = 0;
      image_viewer->hadjustment->upper = 1.0;
      image_viewer->hadjustment->step_increment = 0.01;
      image_viewer->hadjustment->page_size = image_viewer->canvas_width / (width * image_viewer->current_scale_x);
      image_viewer->hadjustment->value = (imgx - min_x)/width;
      gtk_signal_emit_by_name (GTK_OBJECT (image_viewer->hadjustment), "changed");
      gtk_signal_emit_by_name (GTK_OBJECT (image_viewer->hadjustment), "value_changed");

    }

  if (image_viewer->vadjustment)
    {
      image_viewer->vadjustment->lower = 0;
      image_viewer->vadjustment->upper = 1.0;

      /* The following should be five pixel */
      image_viewer->vadjustment->step_increment = 0.1;
      
      /* Should be coord of center of image compared to image width */
      //image_viewer->vadjustment->value = 0.5;
      image_viewer->vadjustment->value = (imgy - min_y)/height;
      image_viewer->vadjustment->step_increment = 0.01;
      image_viewer->vadjustment->page_size = image_viewer->canvas_height / (height * image_viewer->current_scale_y);

      gtk_signal_emit_by_name (GTK_OBJECT (image_viewer->vadjustment), "changed");
      gtk_signal_emit_by_name (GTK_OBJECT (image_viewer->vadjustment), "value_changed");
    }
}
