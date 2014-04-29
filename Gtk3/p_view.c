/*
 * view.c
 * 
 * Copyright 2014 Michael <mike@voyager>
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

#include "t_view.h"

// define width and height of individual tocta images
const int width=400;
const int height=400;

// used by print function
const int GroupSize = 12;

// scaled x-y co-ords for nodes a->f
const double 
node_scale[6][2] = {{0.4833,0.4267},	// a
					{0.9900,0.6267},	// b
					{0.5633,0.6417},	// c
					{0.0100,0.3867},	// d
					{0.6433,0.0067},	// e
					{0.3900,0.9800}};	// f
							
// scaled x-y co-ords for primes {a_p0,a_p2,a_p3}->{d_p0,d_p2,d_p3}
const double
prime_scale[12][2] = {	{0.2767,0.4000},	// a_p0
						{0.4800,0.2500},	// a_p2
						{0.3700,0.7189},	// a_p3
						
						{0.7000,0.4966},
						{0.8400,0.3400},
						{0.7000,0.8330},
						
						{0.7317,0.6195},
						{0.6200,0.3500},
						{0.5000,0.8000},
						
						{0.3183,0.5084},	// d_p0
						{0.2200,0.2189},	// d_p2
						{0.1300,0.7189}};	// d_p3
						
struct surface_data *surfaces;			

double
node_xy[6][2];		// scaled co-ordinates for nodes a->f

double
prime_xy[12][2];	// scaled co-ordinates for prime values

// ==================== Main Code =======================

int main(int argc, char **argv)
{

	int i;
	
	int nGroups;
	char **primes_array;
	char **pStr;
	int j,k;
	FILE *fp;
	char filename[256];
	char title[256];
	
	// look for .dat files in Parent Directory
	sprintf(filename,"../DatFiles/Tocta_%s.dat", argv[1]);
	printf("Looking for %s ... ", filename);
	fp = fopen(filename,"r");
	if(fp != NULL) {
		printf("Found.\n");
	} else {
		printf("not found --- Add valid target value to cmd line! --- Stopping.\n");
		exit(1);
	}
	
	fclose(fp);
	
	primes_array = extract_base_toctas(filename, &nGroups);
	sprintf(title,"- Showing %d Base Toctas for Target %s -", nGroups, argv[1]);
	printf("nGroups = %d\n", nGroups);
	
#if(0)	
	pStr = primes_array;
	for(j=0; j<nGroups; j++) {
		for(i=0; i<GroupSize; i++) printf("%s,", *(pStr + j*GroupSize + i));
		printf("\n");
	}
#endif
//----------------------------------------------------------------------	
		
	// Calculate the node co-ordinates
	for(i=0;i<6;i++) {
		node_xy[i][0] = width * node_scale[i][0];
		node_xy[i][1] = height * node_scale[i][1];
	}
	
	// Calculate the number co-ordinates
	for(i=0;i<12;i++) {
		prime_xy[i][0] = width * prime_scale[i][0];
		prime_xy[i][1] = height * prime_scale[i][1];
	}
	
	// Create the surfaces
	
	const int nCols = 4;
	const int nRows = (nGroups-1)/nCols + 1;
	GtkWidget *window, *scrolled, *grid;
	GdkPixbuf *pixbuf;
	
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	scrolled = gtk_scrolled_window_new(NULL,NULL);
	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid),5);
	gtk_grid_set_column_spacing(GTK_GRID(grid),5);
	
	// allocate space for surfaces array
	surfaces = (struct surface_data*)malloc(sizeof(struct surface_data) * nGroups);
	
	for(i=0; i<nGroups; i++) {
		surfaces[i].image = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
		surfaces[i].cr = cairo_create(surfaces[i].image);
		
		draw_tocta(primes_array, i, &surfaces[i]);
		
		pixbuf = gdk_pixbuf_get_from_surface( surfaces[i].image, 0, 0, width, height);
		surfaces[i].tocta_image = gtk_image_new_from_pixbuf(pixbuf);
		
		gtk_grid_attach(GTK_GRID(grid), surfaces[i].tocta_image, (i%nCols), (i/nCols), 1, 1);
		
	}
	
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),GTK_POLICY_ALWAYS,GTK_POLICY_ALWAYS);
	
	gtk_container_add(GTK_CONTAINER(scrolled), grid);
	
	gtk_container_add(GTK_CONTAINER(window), scrolled);
	
	g_signal_connect(window, "destroy",G_CALLBACK (gtk_main_quit), NULL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), (width*4)+30, (height*2)+30); 
	gtk_window_set_title(GTK_WINDOW(window), title);
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}

