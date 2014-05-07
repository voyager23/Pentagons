/* 
 * p_view.h
 * Copyright 2014 Michael <michael.tate@wanadoo.fr>
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo.h>
#include <gtk/gtk.h>

 #ifndef __PVIEW_H__
 #define __PVIEW_H__ 
 
extern double node_xy[5][2];
extern double prime_xy[10][2];
 
typedef struct surface_data {
	cairo_surface_t *image;
	cairo_t *cr;
	GtkWidget *penta_image;
}Surface_Data;
 
void draw_penta(char **, int i, Surface_Data *p);
void file_penta(int Target, int nUnique, Surface_Data *p, int i);
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);
char **extract_base_pentas(FILE *fp, int *nUnique);

#endif
