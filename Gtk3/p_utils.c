/*
 * p_utils.c
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
#include "t_view.h"

extern const int nDigits;
extern const int GroupSize;

//----------------------------------------------------------------------
void draw_tocta(char ** primes_array, int i, struct surface_data *p) {


  cairo_set_source_rgb(p->cr,0.9, 0.9, 0.9);
  cairo_paint(p->cr);
  
  cairo_set_source_rgb(p->cr, 0.2, 0.4, 0.6 );
  
  // d->a->b->c
  cairo_set_line_width(p->cr, 1.0);
  cairo_move_to(p->cr, node_xy[3][0], node_xy[3][1]);
  cairo_line_to(p->cr, node_xy[0][0], node_xy[0][1]);
  cairo_line_to(p->cr, node_xy[1][0], node_xy[1][1]);
  cairo_stroke(p->cr);
  // perspective 
  cairo_set_line_width(p->cr, 3.0);
  cairo_move_to(p->cr, node_xy[1][0], node_xy[1][1]);
  cairo_line_to(p->cr, node_xy[2][0], node_xy[2][1]);
  cairo_line_to(p->cr, node_xy[3][0], node_xy[3][1]);
  cairo_stroke(p->cr);
  
  // f->a->e->c
  cairo_set_line_width(p->cr, 1.0);
  cairo_move_to(p->cr, node_xy[5][0], node_xy[5][1]);
  cairo_line_to(p->cr, node_xy[0][0], node_xy[0][1]);
  cairo_line_to(p->cr, node_xy[4][0], node_xy[4][1]);
  cairo_stroke(p->cr);
  // perspective
  cairo_set_line_width(p->cr, 3.0);
  cairo_move_to(p->cr, node_xy[4][0], node_xy[4][1]);
  cairo_line_to(p->cr, node_xy[2][0], node_xy[2][1]);
  cairo_line_to(p->cr, node_xy[5][0], node_xy[5][1]);
  cairo_stroke(p->cr);
  
   // d->e->b-f
   cairo_set_line_width(p->cr, 3.0);
  cairo_move_to(p->cr, node_xy[3][0], node_xy[3][1]);
  cairo_line_to(p->cr, node_xy[4][0], node_xy[4][1]);
  cairo_line_to(p->cr, node_xy[1][0], node_xy[1][1]);
  cairo_line_to(p->cr, node_xy[5][0], node_xy[5][1]);
  cairo_close_path(p->cr); 
  cairo_stroke(p->cr);
  
  cairo_select_font_face(p->cr, "monospace", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD);
  
  cairo_set_font_size(p->cr,18);
  cairo_set_source_rgb(p->cr, 0.1, 0.1, 0.1 );
  int j;
  for(j=0;j<12;j++) {
	  cairo_move_to(p->cr, prime_xy[j][0], prime_xy[j][1]);
	  cairo_show_text(p->cr, *(primes_array + i*GroupSize + j));
	  cairo_stroke(p->cr);
  }  	
}
//----------------------------------------------------------------------
char **extract_base_toctas(const char *fname, int *nGroups) {
	const int nDigits = 16;
	FILE *fp;
	char buffer[256];
	char **Head = NULL;
	char **pStr = NULL;
	char *pToken;
	int skip,token_idx,i,j;
	char working[256];
	
	*nGroups=0;
	fp = fopen(fname,"r");
	if(fp==NULL) return;
	
	while(fgets(buffer,255,fp) != NULL) {
		if(strstr(buffer,"Primes>") == NULL) continue;
		*nGroups += 1;
		// ........................
		Head = (char**)realloc(Head, (*nGroups) * GroupSize * sizeof(char*));
		pStr = Head + ((*nGroups - 1) * GroupSize);
		pToken = strtok(buffer," ,");
		token_idx = 0;
		skip = 2;
		while(1){
			pToken = strtok(NULL," ,");
			if(pToken == NULL) break;
			token_idx += 1;
			if(token_idx == skip) {
				skip += 4;
				continue;
			}
			*pStr = (char*)malloc(sizeof(char)*(nDigits+1));
			strncpy(*pStr, pToken, nDigits);
			pStr += 1;
		}
		//.........................
	}
	fclose(fp);
	return Head;
}

//----------------------------------------------------------------------
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer p)
{
  cairo_set_source_surface(cr, ((struct surface_data*)p)->image, 0, 0);
  cairo_paint(cr);
  return FALSE;
}
//----------------------------------------------------------------------
