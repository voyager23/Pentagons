/* 
 * view.h
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo.h>
#include <gtk/gtk.h>

 #ifndef __VIEW_H__
 #define __VIEW_H__ 
 
extern double node_xy[6][2];
extern double prime_xy[12][2];
 
struct surface_data {
	cairo_surface_t *image;
	cairo_t *cr;
	GtkWidget *tocta_image;
};
 
void draw_tocta(char **, int i, struct surface_data *p);
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);
char **extract_base_toctas(const char *fname, int *nGroups);

#endif
