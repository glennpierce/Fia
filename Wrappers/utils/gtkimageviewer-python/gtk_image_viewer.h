/* 
 * Copyright (c) 2001 by Dov Grobgeld <dov@imagic.weizmann.ac.il>
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

#ifndef __GTK_IMAGE_VIEWER_H__
#define __GTK_IMAGE_VIEWER_H__

#include <gdk/gdk.h>
#include <gtk/gtkwidget.h>
#include <gtk/gtkmisc.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtkadjustment.h>

#ifdef __cplusplus
extern "C" {
#endif
  
#define GTK_TYPE_IMAGE_VIEWER          (gtk_image_viewer_get_type())
#define GTK_IMAGE_VIEWER(obj)          GTK_CHECK_CAST (obj, GTK_TYPE_IMAGE_VIEWER, GtkImageViewer)
#define GTK_IMAGE_VIEWER_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, GTK_TYPE_IMAGE_VIEWER, GtkImageViewerClass)
#define GTK_IS_IMAGE_VIEWER(obj)       GTK_CHECK_TYPE (obj, GTK_TYPE_IMAGE_VIEWER)


typedef struct _GtkImageViewer        GtkImageViewer;
typedef struct _GtkImageViewerClass   GtkImageViewerClass;

struct _GtkImageViewer
{
  GtkWidget widget;
  GdkPixbuf *image;
  GtkAdjustment *hadjustment;
  GtkAdjustment *vadjustment;
  double current_scale_x, current_scale_y, current_x0, current_y0;
  gint canvas_width, canvas_height;
  gint scroll_width, scroll_height;
  gint scroll_min_x, scroll_max_x;
  gint scroll_min_y, scroll_max_y;
  gboolean do_use_transfer_map;
  gboolean do_flip_horizontal, do_flip_vertical;
  guint8 *transfer_map[3];

  /* Motion related functions */
  gboolean is_mouse_button2_pressed;
  gint last_pan_anchor_x, last_pan_anchor_y;
  gint last_x, last_y;
  gint button;
  
  /* Settings */
  GdkInterpType interp_type;
  gboolean do_linear_zoom_steps;

  /* Configuration */
  gdouble min_zoom;
  gdouble max_zoom;

  /* Reusable data */
  GdkGC *gc;
};

struct _GtkImageViewerClass
{
  GtkWidgetClass parent_class;

  void (*view_changed) (GtkImageViewer *image_viewer);
  void	(*set_scroll_adjustments)	(GtkImageViewer	*image_viewer,
					 GtkAdjustment	*hadjustment,
					 GtkAdjustment	*vadjustment);

};

GtkWidget*     gtk_image_viewer_new          (GdkPixbuf *pixbuf);
GtkWidget*     gtk_image_viewer_new_from_file(const gchar *filename);
guint          gtk_image_viewer_get_type     (void);
guint	       gtk_image_viewer_set_image    (GtkImageViewer *widget,
					      GdkPixbuf *_img);
guint	       gtk_image_viewer_set_file     (GtkImageViewer *widget,
					      const gchar *filename);
GdkPixbuf*     gtk_image_viewer_get_image    (GtkImageViewer *widget);
guint          gtk_image_viewer_get_image_width(GtkImageViewer *widget);
guint          gtk_image_viewer_get_image_height(GtkImageViewer *widget);
gboolean       gtk_image_viewer_get_is_panning(GtkImageViewer *widget);
guint          gtk_image_viewer_get_canvas_width(GtkImageViewer *widget);
guint          gtk_image_viewer_get_canvas_height(GtkImageViewer *widget);
void           gtk_image_viewer_get_scale    (GtkImageViewer *widget,
					      /* output */
					      double *scale_x,
					      double *scale_y
					      );
void           gtk_image_viewer_get_transform(GtkImageViewer *widget,
					      /* output */
					      double *x,
					      double *y,
					      double *scale_x,
					      double *scale_y);
void           gtk_image_viewer_set_transform(GtkImageViewer *widget,
					      double x,
					      double y,
					      double scale_x,
					      double scale_y);
void           gtk_image_viewer_set_min_zoom (GtkImageViewer *widget,
					      double min_zoom);
void           gtk_image_viewer_set_max_zoom (GtkImageViewer *widget,
					      double min_zoom);
double         gtk_image_viewer_get_min_zoom (GtkImageViewer *widget);
double         gtk_image_viewer_get_max_zoom (GtkImageViewer *widget);
void           gtk_image_viewer_img_coord_to_canv_coord(
                                              GtkImageViewer *image_viewer,
				              double imgx,
					      double imgy,
					      /* output */
					      double *cx,
					      double *cy);
void           gtk_image_viewer_canv_coord_to_img_coord(
                                              GtkImageViewer *image_viewer,
					      double cx, double cy,
					      /* output */
					      double *imgx, double *imgy);

void           gtk_image_viewer_set_transfer_map(GtkImageViewer *widget,
						 guint8 *rmap,
						 guint8 *gmap,
						 guint8 *bmap);
gint           gtk_image_viewer_redraw(GtkImageViewer *image_viewer);
gint           gtk_image_viewer_zoom_around_fixed_point(
                                       GtkImageViewer *image_viewer,
				       double new_scale_x,
				       double new_scale_y,
				       double old_x,
				       double old_y,
				       double new_x,
				       double new_y);
void           gtk_image_viewer_expose_area(GtkImageViewer *image_viewer,
					    int x0, int y0,
					    int w, int h);
int            gtk_image_viewer_get_scroll_width(GtkImageViewer *widget);
int            gtk_image_viewer_get_scroll_height(GtkImageViewer *widget);
void           gtk_image_viewer_set_scroll_height(GtkImageViewer *widget,
		 				  int scroll_height);
int            gtk_image_viewer_set_scroll_region(GtkImageViewer *widget,
                                                  double x0, double y0,
                                                  double x1, double y1);

void           gtk_image_viewer_set_scroll_width(GtkImageViewer *widget,
					         int scroll_width);
gboolean       gtk_image_viewer_get_use_linear_zoom_steps(GtkImageViewer *widget);
void
gtk_image_viewer_set_use_linear_zoom_steps(GtkImageViewer *widget,
					   gboolean do_use_linear_steps);

/* Convenience functions for tying to image display buttons */
void           gtk_image_viewer_zoom_rel(GtkImageViewer *widget,
					 double rel_zoomfactor);
void           gtk_image_viewer_zoom_fit(GtkImageViewer *widget,
					 gboolean do_keep_aspect_ratio);
void           gtk_image_viewer_zoom_100(GtkImageViewer *widget);

GtkAdjustment* gtk_image_viewer_get_hadjustment (GtkImageViewer   *image_viewer);
GtkAdjustment* gtk_image_viewer_get_vadjustment (GtkImageViewer   *image_viewer);
void           gtk_image_viewer_set_hadjustment (GtkImageViewer   *image_viewer,
						 GtkAdjustment *adjustment);
void           gtk_image_viewer_set_vadjustment (GtkImageViewer   *image_viewer,
						 GtkAdjustment *adjustment);

void           gtk_image_viewer_set_flip(GtkImageViewer *image_viewer,
                                         gboolean do_vflip,
                                         gboolean do_hflip);

#ifdef __cplusplus
}
#endif
    

#endif
