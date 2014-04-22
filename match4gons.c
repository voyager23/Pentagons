/*
 * match4gons.c
 * 
 * Copyright 2014 Michael Tate <mike@wingnut>
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


#include "./penta.h"

int match4gons(GSList **FourGons, GSList **Pentagons) {
	GSList *a, *b;
	int matches = 0;
	for(a=*FourGons; a != NULL; a = a->next) {		
		for(b=*FourGons; b != NULL; b = b->next) {
			if(a==b) continue;
			if(	(RPTR(a)->nodes[2]->node4_id == RPTR(b)->nodes[0]->node4_id)&&
				(RPTR(a)->nodes[3]->node4_id == RPTR(b)->nodes[1]->node4_id)&&
				(RPTR(a)->nodes[0]->node4_id == RPTR(b)->nodes[3]->node4_id)&&
				(RPTR(a)->nodes[1]->primes[3] == RPTR(b)->nodes[2]->primes[2]))
				{
					printf("Match!\n");
					matches++;
				}
		}
	}	
	return matches;
}

