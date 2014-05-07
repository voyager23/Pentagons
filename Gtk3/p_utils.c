/*
 * p_utils.c
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
#include "p_view.h"

extern const int nDigits;
extern const int GroupSize;

//----------------------------------------------------------------------
void file_penta(int Target, int nUnique, Surface_Data *p, int i){
	
	char fname[128];	
	sprintf(fname,".//PngFiles//Penta_%d_%d_%d.png",Target,nUnique,i);
	// more code
	cairo_surface_write_to_png( (p+i)->image, fname);
	// end
}
//----------------------------------------------------------------------
void draw_penta(char ** primes_array, int i, Surface_Data *p) {
	
	Surface_Data *q = (p+i);

	cairo_set_source_rgb(q->cr,0.9, 0.9, 0.9);
	cairo_paint(q->cr);
	  
	cairo_set_source_rgb(q->cr, 0.2, 0.4, 0.6 );
	  
	// a->b->c->d->e->a
	cairo_set_line_width(q->cr, 3.0);
	cairo_move_to(q->cr, node_xy[0][0], node_xy[0][1]);
	cairo_line_to(q->cr, node_xy[1][0], node_xy[1][1]);
	cairo_line_to(q->cr, node_xy[2][0], node_xy[2][1]);
	cairo_line_to(q->cr, node_xy[3][0], node_xy[3][1]);
	cairo_line_to(q->cr, node_xy[4][0], node_xy[4][1]);
	cairo_line_to(q->cr, node_xy[0][0], node_xy[0][1]);
	cairo_stroke(q->cr);
	cairo_set_line_width(q->cr, 1.0);
	cairo_move_to(q->cr, node_xy[0][0], node_xy[0][1]);
	cairo_line_to(q->cr, node_xy[2][0], node_xy[2][1]);
	cairo_line_to(q->cr, node_xy[4][0], node_xy[4][1]);
	cairo_line_to(q->cr, node_xy[1][0], node_xy[1][1]);
	cairo_line_to(q->cr, node_xy[3][0], node_xy[3][1]);
	cairo_line_to(q->cr, node_xy[3][0], node_xy[3][1]);  
	cairo_close_path(q->cr); 
	cairo_stroke(q->cr);
	cairo_select_font_face(q->cr, "monospace", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD);
	  
	cairo_set_font_size(q->cr,18);
	cairo_set_source_rgb(q->cr, 0.1, 0.1, 0.1 );
	int j;
	for(j=0;j<GroupSize;j++) {
		cairo_move_to(q->cr, prime_xy[j][0], prime_xy[j][1]);
		cairo_show_text(q->cr, *(primes_array + i*GroupSize + j));
		cairo_stroke(q->cr);
	}  	
}
//----------------------------------------------------------------------
char **extract_base_pentas(FILE *fp, int *nUnique) {
		
	char **Head = NULL;
	char **pStr = NULL;
	char *pToken;
	char buffer[256];
	int i,j;
	
	// set up and sanity checks
	*nUnique=0;
	if(fp==NULL) return;
	// parse file
	while(fgets(buffer,255,fp) != NULL) {
		if(strstr(buffer,"Primes>") == NULL) continue;
		*nUnique += 1;
		Head = (char**)realloc(Head, GroupSize*sizeof(char*)*(*nUnique) );
		pStr = Head + ((*nUnique - 1) * GroupSize);
		pToken = strtok(buffer," ,");	// capture and discard 'Primes>'
		
		for(i=0;i<5;i++) {
			for(j=0;j<4;j++) {
				pToken = strtok(NULL," ,");
				if((j==0)||(j==3)) continue;
				*pStr = (char*)malloc(sizeof(char)*8);
				strcpy(*pStr,pToken);
				pStr += 1;
			}
		}
	}
	return Head;
}

//----------------------------------------------------------------------
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer p)
{
  cairo_set_source_surface(cr, ((Surface_Data*)p)->image, 0, 0);
  cairo_paint(cr);
  return FALSE;
}
//----------------------------------------------------------------------
