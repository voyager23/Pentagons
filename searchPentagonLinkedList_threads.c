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
	int found;
	GSList *a,*b,*c,*d,*e;	// read only
	GSList **nodes;			// read only
	GSList **basepentas;	// read-write
	GSList **pentagons;		// read-write
} ThreadData;

void* worker_e(void *);
void* worker_e(void *p) {	
	return NULL;
}

int searchPentagonLinkedList_threads(GSList **Nodes, GSList **BasePentas, GSList **Pentagons, int Target) {

	GSList *a,*b,*c,*d,*e;
	int i,n_pentagons = 0;
	int fail;
	
	int t;
	ThreadData *Head_tdb, *p_tdb;
	
	Head_tdb = (ThreadData*)malloc(sizeof(ThreadData)*g_slist_length(*Nodes));
	
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
					//==================================================
					e = *Nodes;
					p_tdb = Head_tdb;
					while(e != NULL) {
						// launch threads in blocks of NTHREADS
						count = 0;
						while(count < NTHREADS) {
							p_tdb->a = a;
							p_tdb->b = b;
							p_tdb->c = c;
							p_tdb->d = d;
							p_tdb->e = e;
							pthread_create(&(p_tdb->thread_id), NULL, worker_e, p_tdb);
							e = e->next;
							if(e == NULL) break;
							p_tdb += 1;
							count++;
						}
						// recover threads in blocks of NTHREADS
						
						p_tdb -= NTHREADS;
						while(count < NTHREADS) {
							pthread_join(p_tdb->thread_id, NULL)
					
					//==================================================
					
				} // d loop
			} // c loop			
		} // b loop
	} // a loop		
	return n_pentagons;
}
