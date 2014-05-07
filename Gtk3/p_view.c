/*
 * p_view.c
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

#include "p_view.h"

// define width and height of individual pentagon images
const int width=400;
const int height=400;

// used by print function
const int GroupSize = 10;

// scaled x-y co-ords for nodes a->e (5)
const double 
node_scale[5][2] = {{0.500,0.000},	// a
					{1.000,0.363 },	// b
					{0.809,0.951 },	// c
					{0.191,0.951 },	// d
					{0.000,0.363}};	// e
							
// scaled x-y co-ords for primes
// first five are the outer primes (nodes)
// next five are the inner primes (spokes)
const double
prime_scale[10][2] = {	{0.290,0.182},	//a_p1
						{0.470,0.350},	//a_p2
						{0.650,0.182},	//b_p1
						{0.655,0.476},	//b_p2
						{0.850,0.657},	//c_p1
						{0.610,0.690},	//c_p2		
						{0.500,0.951},	//d_p1
						{0.330,0.690},	//d_p2
						{0.095,0.657},	//e_p1
						{0.290,0.476}};	//e_p2

Surface_Data *surfaces;			

double
node_xy[5][2];		// scaled co-ordinates for nodes a->f

double
prime_xy[10][2];	// scaled co-ordinates for prime values

// ==================== Main Code =======================

int main(int argc, char **argv)
{

	int i;
	
	int nUnique=0;
	char **primes_array;
	char **pStr;
	int j,k;
	FILE *fp;
	char filename[256];
	char title[256];
	
	// look for .dat files in Parent Directory
	sprintf(filename,"../DatFiles/Penta_%s.dat", argv[1]);
	printf("Looking for %s ... ", filename);
	fp = fopen(filename,"r");
	if(fp != NULL) {
		printf("Found.\n");
	} else {
		printf("not found --- Add valid target value to cmd line! --- Stopping.\n");
		exit(1);
	}
	
	primes_array = extract_base_pentas(fp, &nUnique);
	if(nUnique < 1) {
		printf("No pentagons found for target value %s.\n", argv[1]);
		fclose(fp);
		return 0;
	}
	sprintf(title,"- Showing %d Base Pentas for Target %s -", nUnique, argv[1]);
	
#if(0)	
	pStr = primes_array;
	for(j=0; j<nUnique; j++) {
		for(i=0; i<GroupSize; i++) printf("%s,", *(pStr + j*GroupSize + i));
		printf("\n");
	}
#endif
	fclose(fp);

	
//----------------------------------------------------------------------	
		
	// Calculate the node co-ordinates
	for(i=0;i<5;i++) {
		node_xy[i][0] = width * node_scale[i][0];
		node_xy[i][1] = height * node_scale[i][1];
	}
	
	// Calculate the number co-ordinates
	for(i=0;i<10;i++) {
		prime_xy[i][0] = width * prime_scale[i][0];
		prime_xy[i][1] = height * prime_scale[i][1];
	}
	
	// Create the surfaces
	
	const int nCols = 4;
	const int nRows = (nUnique-1)/nCols + 1;
	GtkWidget *window, *scrolled, *grid;
	GdkPixbuf *pixbuf;
	
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	scrolled = gtk_scrolled_window_new(NULL,NULL);
	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid),5);
	gtk_grid_set_column_spacing(GTK_GRID(grid),5);
	
	// allocate space for surfaces array
	surfaces = (Surface_Data*)malloc(sizeof(Surface_Data) * nUnique);
	
	for(i=0; i<nUnique; i++) {
		surfaces[i].image = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
		surfaces[i].cr = cairo_create(surfaces[i].image);		
		draw_penta(primes_array, i, surfaces);		
		pixbuf = gdk_pixbuf_get_from_surface( surfaces[i].image, 0, 0, width, height);
		surfaces[i].penta_image = gtk_image_new_from_pixbuf(pixbuf);
		gtk_grid_attach(GTK_GRID(grid), surfaces[i].penta_image, (i%nCols), (i/nCols), 1, 1);
		// new code
		file_penta(84,nUnique,surfaces,i);		
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

