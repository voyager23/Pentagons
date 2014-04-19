/*
 * utilities.c
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
	
int adjacentNode4(struct node4 *a, struct node4 *b) {
	// The order of the paramters is important.
	// By convention, node a/p1 => node b/p0.
	// If the adjacent nodes comply then return 1
	// else return 0 failure.
	int i;
	if(a->primes[1] != b->primes[0]) return 0;
	for(i=1; i<4; i++) {
		if( (a->primes[0] == b->primes[i])||
			(a->primes[2] == b->primes[i])||
			(a->primes[3] == b->primes[i]) ) 
		return 0;
	}	
	return 1;
}
//------------------------------------------------------------------//

int diagonalNode4(struct node4 *a, struct node4 *b) {
	// The order of the paramters is important.
	// By convention, node a comes before node b
	// For diagonal nodes, all prime weights must be unique
	// If the diagonal nodes comply then return 1
	// else return 0 failure.
	int i;
	for(i=0; i<4; i++) {
		if( (a->primes[0] == b->primes[i])||
			(a->primes[1] == b->primes[i])||
			(a->primes[2] == b->primes[i])||
			(a->primes[3] == b->primes[i]) )
		return 0;
	}	
	return 1;
}
//------------------------------------------------------------------//

void printNode4(struct node4 *np) {
	if(np != NULL) {
			printf("  node_id = %03d    p0 = %03d  p1 = %03d  p2 = %03d  p3 = %03d\n", 
				np->node4_id,np->primes[0],np->primes[1],np->primes[2],np->primes[3] );
		} else {
			printf("NULL node pointer.\n");
		}
}

//------------------------------------------------------------------//

void printRing5_compact(struct ring5 *rp) {
	int i;
	for(i=0; i<5; i++) printNode4(rp->nodes[i]);
	
	
}
//------------------------------------------------------------------//
