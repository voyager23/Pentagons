/*
 * searchNodesLinkedList.c
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

// permutations of 4 objects 0,1,2,3
const int perm[24][4] =
{{ 0, 1, 2, 3 },
 { 0, 1, 3, 2 },
 { 0, 2, 1, 3 },
 { 0, 2, 3, 1 },
 { 0, 3, 1, 2 },
 { 0, 3, 2, 1 },
 { 1, 0, 2, 3 },
 { 1, 0, 3, 2 },
 { 1, 2, 0, 3 },
 { 1, 2, 3, 0 },
 { 1, 3, 0, 2 },
 { 1, 3, 2, 0 },
 { 2, 0, 1, 3 },
 { 2, 0, 3, 1 },
 { 2, 1, 0, 3 },
 { 2, 1, 3, 0 },
 { 2, 3, 0, 1 },
 { 2, 3, 1, 0 },
 { 3, 0, 1, 2 },
 { 3, 0, 2, 1 },
 { 3, 1, 0, 2 },
 { 3, 1, 2, 0 },
 { 3, 2, 0, 1 },
 { 3, 2, 1, 0 } };


int searchNodesLinkedList(GSList **Primes, GSList **Nodes, int Target) {
	// Scan the list of primes for QUADS which sum to Target
	// Save in Nodes list

	GSList *p0, *p1, *p2, *p3;
	Node4 *newnode;
	int i,j,sum;
	int found = 0;
	int buffer[4];
	int index = 0;
	
	p0 = *Primes;
	while(p0 != NULL) {
		buffer[0] = GPOINTER_TO_INT(p0->data);
		if(buffer[0] > Target) break;
		p1 = g_slist_next(p0);
		while(p1 != NULL) {
			buffer[1] = GPOINTER_TO_INT(p1->data);
			if((buffer[0]+buffer[1])>Target) break;
			p2 = g_slist_next(p1);
			while(p2 != NULL) {
				buffer[2] = GPOINTER_TO_INT(p2->data);
				if((buffer[0]+buffer[1]+buffer[2])>Target) break;
				p3 = g_slist_next(p2);
				while(p3 != NULL) {
					buffer[3] = GPOINTER_TO_INT(p3->data);
					sum = (buffer[0]+buffer[1]+buffer[2]+buffer[3]);
					if(sum > Target) break;
					// if sum == target save to list
					if(sum == Target) {											
						for(i=0; i<24; i++) {
							// create a Node structure
							newnode = malloc(sizeof(Node4));
							for(j=0; j<4; j++) newnode->primes[j] = buffer[ perm[i][j] ];
							newnode->node4_id = index++;
							// add permutation to list
							*Nodes = g_slist_append(*Nodes, newnode);	
						}
						found+=24;				
					}
				p3 = g_slist_next(p3);				
				}
			p2 = g_slist_next(p2);
			}
		p1 = g_slist_next(p1);
		}
	p0 = g_slist_next(p0);
	}	
	return found;
}

