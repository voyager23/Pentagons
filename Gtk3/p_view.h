/* 
 * p_view.h
 * 
 * Copyright 2014 Michael <michael.tate@wanadoo.fr>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
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
