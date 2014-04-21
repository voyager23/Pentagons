/*
 * searchPentagonLinkedList.c
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

//========================== Dev Code ==================================

int add_Pentagon_to_list(GSList**Pentagons, struct ring5 *working);
int find_Pentagon(GSList **Pentagons, struct ring5 *working);
void rotate(struct ring5 *p,int n);
void mirror(struct ring5 *p,int axis);
struct ring5* deep_copy_ring5(const struct ring5 *p);

int add_Pentagon_to_list(GSList**Pentagons, struct ring5 *working) {

	struct ring5 *new_rot, *new_ref;
	int i;
	if(find_Pentagon(Pentagons, working) == 0) {
		
		printRing5_compact(working);
		
		// rotations & reflections
		for(i=0;i<5;i++) {
			
			new_rot = deep_copy_ring5(working);
			new_ref = deep_copy_ring5(working);
			
			//printf("\n%d:working rotate pointer %p\n",i,working);printRing5_compact(working);
			//printf("new_rot in: i=%d\n",i);printRing5_compact(new_rot);
			rotate(new_rot,i);
			//printf("new_rot out\n");printRing5_compact(new_rot);
			
			//printf("\n%d:working mirror pointer %p\n",i, working);printRing5_compact(working);
			//printf("new_ref in pointer %p\n", new_ref);printRing5_compact(new_ref);
			mirror(new_ref,i);
			//printf("new_ref out\n");printRing5_compact(new_ref);			
			
			*Pentagons = g_slist_prepend(*Pentagons, new_rot);
			*Pentagons = g_slist_prepend(*Pentagons, new_ref);
		}
		
	printf("\n==================================================\n");	
	return 1;	// added to list
	}
	return 0;	// not added
}

int find_Pentagon(GSList **Pentagons, struct ring5 *working) {
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

void rotate(struct ring5 *p,int n){
	// 0 <= n <= 4
	struct node4 *temp = NULL;
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
void mirror(struct ring5 *p,int axis){	
	struct node4 *temp = NULL;
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

struct ring5* deep_copy_ring5(const struct ring5 *p) {
	// make a complete independent copy of struct ring5
	int i,j;
	struct ring5 *q = (struct ring5*)malloc(sizeof(struct ring5));
	for(i=0;i<5;i++) {
		q->nodes[i] = (struct node4*)malloc(sizeof(struct node4));
		for(j=0;j<4;j++) q->nodes[i]->primes[j] = p->nodes[i]->primes[j];
	}
	return q;
}
		

//======================================================================

int searchPentagonLinkedList(GSList **Nodes, GSList **Pentagons, int Target) {

	GSList *a,*b,*c,*d,*e;
	int i,n_pentagons = 0;
	int fail;
	
	
	
	for(a=*Nodes; a != NULL; a = a->next) {		
		for(b=*Nodes; b != NULL; b = b->next) {
			if((b==a)||(NPTR(a)->primes[1] != NPTR(b)->primes[0])) continue;
			fail=0;
			for(i=1; i<4; i++) {
				if(	(NPTR(b)->primes[i] == NPTR(a)->primes[0])||
					(NPTR(b)->primes[i] == NPTR(a)->primes[2])||
					(NPTR(b)->primes[i] == NPTR(a)->primes[3])) {
					fail=1;
					break;
				}
			}
			if(fail==1) continue;
			for(c=*Nodes; c != NULL; c = c->next) {
				if((c==b)||(c==a)||(NPTR(b)->primes[1] != NPTR(c)->primes[0])) continue;
				if((NPTR(c)->primes[3] != NPTR(a)->primes[2])) continue;
				fail=0;
				for(i=1; i<3; i++) {
					if(	(NPTR(c)->primes[i] == NPTR(a)->primes[0])||
						(NPTR(c)->primes[i] == NPTR(a)->primes[1])||
						(NPTR(c)->primes[i] == NPTR(a)->primes[3])||
						(NPTR(c)->primes[i] == NPTR(b)->primes[2])||
						(NPTR(c)->primes[i] == NPTR(b)->primes[3])) {
						fail=1;
						break;
					}
				}
				if(fail==1) continue;
				for(d=*Nodes; d != NULL; d = d->next) {
					if((d==c)||(d==b)||(d==a)) continue;
					if(	// testing for link values
						(NPTR(d)->primes[0] != NPTR(c)->primes[1])||
						(NPTR(d)->primes[2] != NPTR(a)->primes[3])||
						(NPTR(d)->primes[3] != NPTR(b)->primes[2])||
						// testing for unique values
						(NPTR(d)->primes[1] == NPTR(a)->primes[0])||
						(NPTR(d)->primes[1] == NPTR(a)->primes[1])||
						(NPTR(d)->primes[1] == NPTR(a)->primes[2])||
						(NPTR(d)->primes[1] == NPTR(b)->primes[1])||
						(NPTR(d)->primes[1] == NPTR(b)->primes[3])||
						(NPTR(d)->primes[1] == NPTR(c)->primes[2])) continue;
					for(e=*Nodes; e != NULL; e = e->next) {
						if((e==d)||(e==c)||(e==b)||(e==a)) continue;
						if(	// testing for link values
							(NPTR(e)->primes[0] != NPTR(d)->primes[1])||
							(NPTR(e)->primes[1] != NPTR(a)->primes[0])||
							(NPTR(e)->primes[2] != NPTR(b)->primes[3])||
							(NPTR(e)->primes[3] != NPTR(c)->primes[2])) continue;
						// a-b-c-d-e is a pentagon
						// found a Pentagon
						struct ring5 *working = malloc(sizeof(struct ring5));					
						working->nodes[0] = NPTR(a);							
						working->nodes[1] = NPTR(b);
						working->nodes[2] = NPTR(c);
						working->nodes[3] = NPTR(d);
						working->nodes[4] = NPTR(e);
						
#if(1)						
						// if this pentagon is already in list - ignore
						if(add_Pentagon_to_list(Pentagons,working) == 0) {
							//printf("Skipped\n");
						} else {
							//printf("Added 10\n");
							n_pentagons+=10;							
						}
						free(working);
#endif

#if(0)
						*Pentagons = g_slist_prepend(*Pentagons, working);
						n_pentagons++;
#endif						

					} // e loop
				} // d loop
			} // c loop			
		} // b loop
	} // a loop
		
	return n_pentagons;
}
