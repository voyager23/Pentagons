/*
 * searchPentagonLinkedList_threads.c
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
 * Comment: This is a threaded version of the searchPentagonLinkedList function
 * 
 */

#include "./penta.h"

#define NTHREADS 8

typedef struct thread_data {
	pthread_t thread_id;
	int offset_e;
	GSList **nodes;			// read only
	GSList **basepentas;	// read-write
	GSList **pentagons;		// read-write
	GSList *a,*b,*c,*d,*e;	// read only
	int found;
} ThreadData;

void* worker_e(void *);
void* worker_e(void *p) {	
	pthread_exit(NULL);
}

int searchPentagonLinkedList_threads(GSList **Nodes, GSList **BasePentas, GSList **Pentagons, int Target) {

	GSList *a,*b,*c,*d,*e;
	int i,n_pentagons = 0;
	int fail;
	
	int t;
	ThreadData tdb[NTHREADS];
	ThreadData *p_tdb = tdb;
	// initialise some values
	for(t=0;t<NTHREADS;t++) {
		tdb[t].nodes = Nodes;				// RO
		tdb[t].basepentas = BasePentas;	// Protect with mutex - RW
		tdb[t].pentagons = Pentagons;		// Protect with mutex - RW
	}
	
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
					// at this level set up the thread-data-block
					for(t=0; t<NTHREADS; t++) {
						tdb[t].a = a;
						tdb[t].b = b;
						tdb[t].c = c;
						tdb[t].d = d;
					}
					// 
					for(e=*Nodes; e != NULL; e = e->next) {
						//--------------------------------
						printf("Launching threads...");
						for(t=0; t<NTHREADS; t++) {
							tdb[t].e = g_slist_nth(e,t);
							tdb[t].found=0;
							tdb[t].offset_e=t;
							pthread_create( &(tdb[t].thread_id), NULL, worker_e, p_tdb);
						}
						printf("completed\n");						
						//---------------------------------
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
						// if this pentagon is already in list - ignore
						if(add_Pentagon_to_list(Pentagons,BasePentas,working) == 1) n_pentagons+=10;							
						free(working);
					} // e loop
				} // d loop
			} // c loop			
		} // b loop
	} // a loop		
	return n_pentagons;
}
