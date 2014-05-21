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
 * Comment: This is a threaded version of 
 * 	searchPentagonLinkedList()
 * 
 */

#include "./penta.h"

//----------------------------------------------------------------------
#define NTHREADS 8

typedef struct thread_data {
	int index,block_count,found;
	GSList *a,*b,*c,*d,*e;	// thread-local pointers into Nodes list
	GSList *Nodes;			// pointer to head of Nodes list - read_only
	GSList *basepentas;		// thread-local list of unique pentas
	GSList *pentagons;		// thread-local list of all pentas
} ThreadData;

void* worker(void *);
//----------------------------------------------------------------------
void* worker(void *p) {
	// On entry, p points to thread data block which is assumed
	// to be fully populated with data.	
	ThreadData *ptr = p;
	int fail,i;
	
	// a points to first node in block
	while(ptr->block_count > 0) {
		for(ptr->b=ptr->Nodes; ptr->b != NULL; ptr->b = ptr->b->next) {
		if((ptr->b==ptr->a)||(NPTR(ptr->a)->primes[1] != NPTR(ptr->b)->primes[0])) continue;
		fail=0;
		for(i=1; i<4; i++) {
			if(	(NPTR(ptr->b)->primes[i] == NPTR(ptr->a)->primes[0])||
				(NPTR(ptr->b)->primes[i] == NPTR(ptr->a)->primes[2])||
				(NPTR(ptr->b)->primes[i] == NPTR(ptr->a)->primes[3])) {
				fail=1;
				break;
			}
		}
		if(fail==1) continue;
		for(ptr->c=ptr->Nodes; ptr->c != NULL; ptr->c = ptr->c->next) {
				if((ptr->c==ptr->b)||(ptr->c==ptr->a)||(NPTR(ptr->b)->primes[1] != NPTR(ptr->c)->primes[0])) continue;
				if((NPTR(ptr->c)->primes[3] != NPTR(ptr->a)->primes[2])) continue;
				fail=0;
				for(i=1; i<3; i++) {
					if(	(NPTR(ptr->c)->primes[i] == NPTR(ptr->a)->primes[0])||
						(NPTR(ptr->c)->primes[i] == NPTR(ptr->a)->primes[1])||
						(NPTR(ptr->c)->primes[i] == NPTR(ptr->a)->primes[3])||
						(NPTR(ptr->c)->primes[i] == NPTR(ptr->b)->primes[2])||
						(NPTR(ptr->c)->primes[i] == NPTR(ptr->b)->primes[3])) {
						fail=1;
						break;
					}
				}
				if(fail==1) continue;
				for(ptr->d=ptr->Nodes; ptr->d != NULL; ptr->d = ptr->d->next) {
					if((ptr->d==ptr->c)||(ptr->d==ptr->b)||(ptr->d==ptr->a)) continue;
					if(	// testing for link values
						(NPTR(ptr->d)->primes[0] != NPTR(ptr->c)->primes[1])||
						(NPTR(ptr->d)->primes[2] != NPTR(ptr->a)->primes[3])||
						(NPTR(ptr->d)->primes[3] != NPTR(ptr->b)->primes[2])||
						// testing for unique values
						(NPTR(ptr->d)->primes[1] == NPTR(ptr->a)->primes[0])||
						(NPTR(ptr->d)->primes[1] == NPTR(ptr->a)->primes[1])||
						(NPTR(ptr->d)->primes[1] == NPTR(ptr->a)->primes[2])||
						(NPTR(ptr->d)->primes[1] == NPTR(ptr->b)->primes[1])||
						(NPTR(ptr->d)->primes[1] == NPTR(ptr->b)->primes[3])||
						(NPTR(ptr->d)->primes[1] == NPTR(ptr->c)->primes[2])) continue;
					for(ptr->e=ptr->Nodes; ptr->e != NULL; ptr->e = ptr->e->next) {
						if((ptr->e==ptr->d)||(ptr->e==ptr->c)||(ptr->e==ptr->b)||(ptr->e==ptr->a)) continue;
						if(	// testing for link values
							(NPTR(ptr->e)->primes[0] != NPTR(ptr->d)->primes[1])||
							(NPTR(ptr->e)->primes[1] != NPTR(ptr->a)->primes[0])||
							(NPTR(ptr->e)->primes[2] != NPTR(ptr->b)->primes[3])||
							(NPTR(ptr->e)->primes[3] != NPTR(ptr->c)->primes[2])) continue;
						// a-b-c-d-e is a pentagon
						// found a Pentagon
						struct ring5 *working = malloc(sizeof(struct ring5));					
						working->nodes[0] = NPTR(ptr->a);							
						working->nodes[1] = NPTR(ptr->b);
						working->nodes[2] = NPTR(ptr->c);
						working->nodes[3] = NPTR(ptr->d);
						working->nodes[4] = NPTR(ptr->e);
						// if this pentagon is already in list - ignore
						if(add_Pentagon_to_list(&(ptr->pentagons),&(ptr->basepentas),working) == 1) 
							ptr->found+=10;							
						free(working);
					} // e loop
				} // d loop
			} // c loop			
		} // b loop
		ptr->a = ptr->a->next;
		ptr->block_count -= 1;
	}
	pthread_exit(ptr);
}
//----------------------------------------------------------------------
int searchPentagon_thread(GSList **Nodes, GSList **BasePentas, GSList **Pentagons, int Target) {
	
	const int nNodes = g_slist_length(*Nodes);
	
	ThreadData *data_block[NTHREADS];
	ThreadData *ptr;
	int rc;
	pthread_t id_array[NTHREADS];
	void *status;
	int stride, extra, i, n_unique;
	GSList *start_block_a, *data_ptr;
	
	stride = nNodes / NTHREADS;
	extra  = nNodes % NTHREADS;
	start_block_a = *Nodes;
	
	for(i=0; i<NTHREADS; i++) {
		ptr = (ThreadData*)malloc(sizeof(ThreadData));
		data_block[i] = ptr;
		// Thread data block init section		
		ptr->index = i;
		ptr->found = 0;		
		ptr->a = start_block_a;						// thread local pointer
		ptr->b = ptr->c = ptr->d = ptr->e = NULL;	// thread local pointers
		ptr->Nodes = *Nodes;	// Head 0f Nodes list
		ptr->basepentas = ptr->pentagons = NULL;
				
		if(extra > 0) {
			ptr->block_count = stride + 1;
			extra -= 1;
			start_block_a = g_slist_nth(start_block_a, stride+1);
		} else {
			ptr->block_count = stride;
			start_block_a = g_slist_nth(start_block_a, stride);
		}
		
		// End init section - launch thread
		rc = pthread_create(&(id_array[i]), NULL, worker, (void*) ptr);
		if(rc != 0)
				printf("Warning: Thread launch %02d returned error code %02d.\n",i,rc);
		// next thread
	}	
	// All threads launched
	for(i=0; i<NTHREADS; i++) {
		rc = pthread_join(id_array[i], &status);
	}
	// All threads recovered
	
	*BasePentas = *Pentagons = NULL;
	n_unique = 0;
	for(i=0; i<NTHREADS; i++) {
		data_ptr = data_block[i]->basepentas;
		while(data_ptr != NULL) {
			n_unique += add_Pentagon_to_list(Pentagons, BasePentas, RPTR(data_ptr));
			data_ptr = data_ptr->next;
		}
	}

	return n_unique;	// number of unique pentagons
}
