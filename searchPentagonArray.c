/*
 * searchPentagonArray.c
 * 
 * Copyright 2014 Michael Tate <michael.tate@wanadoo.fr>
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

int searchPentagonArray(GSList **Nodes, GSList **Pentagons, int Target) {
	
	
	int *NodePairs = NULL;
	int idx_pairs, n_nodes, n_pentagons;
	int a, a_offset, b, b_offset, c, c_offset, d, d_offset, e, e_offset;
	GSList *pNodeL, *pNodeR;
	struct ring5 *newring;
	
	// setup a (very) large array of node pairs describing their
	// relationship. adj=2, diag=1, unpair=0	
	n_nodes = g_slist_length(*Nodes);
	NodePairs = (int *)malloc(sizeof(int)*n_nodes*n_nodes);	
	pNodeL = *Nodes;
		
	while(pNodeL != NULL) {
		idx_pairs = (NPTR(pNodeL)->node4_id)*n_nodes;
		pNodeR = *Nodes;
		while(pNodeR != NULL) {
			if(diagonalNode4(NPTR(pNodeL), NPTR(pNodeR)) == 1) {
				NodePairs[idx_pairs++] = 1;
			} else if (adjacentNode4(NPTR(pNodeL), NPTR(pNodeR)) == 1) {
				NodePairs[idx_pairs++] = 2;
				} else {
				NodePairs[idx_pairs++] = 0;
				}
			pNodeR = pNodeR->next;			
			}
		pNodeL = pNodeL->next;	
	}
	
#if(1)
	// -----debug check array for consistency-----
	int adj=0, diag=0, unpair=0, err=0;
	for(idx_pairs=0; idx_pairs < (n_nodes*n_nodes); idx_pairs++) {
		switch(NodePairs[idx_pairs]) {
			case 2:
				adj++;
				break;
			case 1:
				diag++;
				break;
			case 0:
				unpair++;
				break;
			default:
				err++;
				break;
		}
	}	
	printf("adj = %d diag = %d unpair = %d err = %d\n",adj,diag,unpair,err);
	printf("Input items %d	pairs = %d\n", (n_nodes*n_nodes), (adj+diag+unpair));
	// -----end debug consistency check-----
#endif
	// main search for Pentagons

	*Pentagons = NULL;
	// 4 index search of this array to find Rings
	n_pentagons = 0;
	for(a=0; a<n_nodes; a++) {
		a_offset = a*n_nodes;
		for(b=0; b<n_nodes; b++) {
			b_offset = b*n_nodes;
			if(NodePairs[a_offset + b] != 2) continue;	// a->b			
			for(c=0; c<n_nodes; c++) {
				c_offset = c*n_nodes;
				if(	(NodePairs[b_offset + c] != 2)||	// b->c & a->c
					(NodePairs[a_offset + c] != 1)) continue;
				for(d=0; d<n_nodes; d++) {
					d_offset = d*n_nodes;
					if(	(NodePairs[c_offset + d] != 2)||	// c->d &d->a & b->d
						(NodePairs[d_offset + a] != 1)||
						(NodePairs[b_offset + d] != 1)) continue;
					for(e=0; e<n_nodes; e++) {
						e_offset = e*n_nodes;
						if(	(NodePairs[e_offset + a] == 2)||
							(NodePairs[e_offset + d] == 2)||
							(NodePairs[e_offset + b] == 1)||
							(NodePairs[e_offset + c] == 1))
						{	// found a Pentagon
							newring = malloc(sizeof(Ring5));
							newring->nodes[0] = (Node4*)g_slist_nth_data(*Nodes, a);							
							newring->nodes[1] = (Node4*)g_slist_nth_data(*Nodes, b);
							newring->nodes[2] = (Node4*)g_slist_nth_data(*Nodes, c);
							newring->nodes[3] = (Node4*)g_slist_nth_data(*Nodes, d);
							newring->nodes[4] = (Node4*)g_slist_nth_data(*Nodes, e);
							*Pentagons = g_slist_prepend(*Pentagons, newring);
							n_pentagons++;
							printf("%d %d %d %d %d\n",a,b,c,d,e);
						}
					}
				}
			}
		}
	}
	return n_pentagons;
}

