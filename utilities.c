/*
 * utilities.c
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
	
int adjacentNode4(Node4 *a, Node4 *b) {
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

int diagonalNode4(Node4 *a, Node4 *b) {
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

void printNode4(Node4 *np) {
	if(np != NULL) {
			printf("  p0 = %4d  p1 = %4d  p2 = %4d  p3 = %4d\n", 
				np->primes[0],np->primes[1],np->primes[2],np->primes[3] );
		} else {
			printf("NULL node pointer.\n");
		}
}

//------------------------------------------------------------------//

void printRing5_compact(Ring5 *rp) {
	int i;
	for(i=0; i<5; i++) printNode4(rp->nodes[i]);
	
	
}
//------------------------------------------------------------------//

int add_Pentagon_to_list(GSList**Pentagons, GSList **WorkingList ,Ring5 *working) {

	Ring5 *new_rot, *new_ref;
	int i;
	if(find_Pentagon(Pentagons, working) == 0) {
		//printf("\n===================New Configuration===================\n");	
		//printRing5_compact(working);
		
		// TODO: add deep copy of working to FinalList
		*WorkingList = g_slist_prepend(*WorkingList, deep_copy_ring5(working));
		
		// all rotations & reflections go to Pentagons
		for(i=0;i<5;i++) {
			new_rot = deep_copy_ring5(working);
			new_ref = deep_copy_ring5(working);
			rotate(new_rot,i);
			mirror(new_ref,i);
			*Pentagons = g_slist_prepend(*Pentagons, new_rot);
			*Pentagons = g_slist_prepend(*Pentagons, new_ref);
		}		
		
	return 1;	// added to list
	}
	return 0;	// not added
}

//------------------------------------------------------------------//

int find_Pentagon(GSList **Pentagons, Ring5 *working) {
	// Pentagons are found by matching 5 pairs of primes p0 & p2
	GSList *target = *Pentagons;
	int i,found;
	while(target != NULL) {
		found = 1;
		for(i=0;i<5;i++) {
			if(	(working->nodes[i]->primes[0] != RPTR(target)->nodes[i]->primes[0])||
				(working->nodes[i]->primes[2] != RPTR(target)->nodes[i]->primes[2]))
			{
				found=0;
				break;
			}
		}
		if(found==1) return 1;
		target = target->next;
	}
	return 0;
}

//------------------------------------------------------------------//

void rotate(Ring5 *p,int n){
	// 0 <= n <= 4
	Node4 *temp = NULL;
	int i;
	// Sanity Checks
	if(p==NULL) {printf("\nrotate: p is NULL pointer.\n");exit(1);}
	if((n<0)||(n>4)) {printf("rotate: n %d out of bounds.\n",n);exit(1);}
	while(n>0) {
		temp = p->nodes[0];
		for(i=1;i<5;i++) p->nodes[i-1] = p->nodes[i];
		p->nodes[4] = temp;
		n--;
	}
}

//------------------------------------------------------------------//

void mirror(Ring5 *p,int axis){	
	Node4 *temp = NULL;
	int i,tmp;
			
	// Sanity Checks
	if(p==NULL) {printf("\nmirror: p is NULL pointer.\n");exit(1);}
	if((axis<0)||(axis>4)) {printf("mirror: axis %d out of bounds.\n",axis);exit(1);}
	
	// swap 'inner pair'
	temp = p->nodes[ (axis+1)%5 ];
	p->nodes[ (axis+1)%5 ] = p->nodes[ (axis+4)%5 ];
	p->nodes[ (axis+4)%5 ] = temp;
	
	// swap 'outer pair'
	temp = p->nodes[ (axis+2)%5 ];
	p->nodes[ (axis+2)%5 ] = p->nodes[ (axis+3)%5 ];
	p->nodes[ (axis+3)%5 ] = temp;
	
	// for each node swap p0/p1 and p2/p3
	for(i=0;i<5;i++) {
		tmp = p->nodes[i]->primes[0];
		p->nodes[i]->primes[0] = p->nodes[i]->primes[1];	// working corruption?
		p->nodes[i]->primes[1] = tmp;
		tmp = p->nodes[i]->primes[2];
		p->nodes[i]->primes[2] = p->nodes[i]->primes[3];
		p->nodes[i]->primes[3] = tmp;
	}		
}

//------------------------------------------------------------------//

Ring5* deep_copy_ring5(const Ring5 *p) {
	// make a complete independent copy of Ring5
	int i,j;
	Ring5 *q = (Ring5*)malloc(sizeof(Ring5));
	for(i=0;i<5;i++) {
		q->nodes[i] = (Node4*)malloc(sizeof(Node4));
		for(j=0;j<4;j++) q->nodes[i]->primes[j] = p->nodes[i]->primes[j];
	}
	return q;
}

//======================================================================
