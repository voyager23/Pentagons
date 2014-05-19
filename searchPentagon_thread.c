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
	GSList **nodes;			// Head of Nodes
	GSList **basepentas;	// thread-local list of unique pentas
	GSList **pentagons;		// thread-local list of all pentas
} ThreadData;

void* worker(void *);
//----------------------------------------------------------------------
void* worker(void *p) {
	ThreadData *ptr = p;
	
	ptr->found = 0;
	ptr->basepentas = NULL;
	ptr->pentagons = NULL;

	pthread_exit(ptr);
}
//----------------------------------------------------------------------
int searchPentagon_thread(GSList **Nodes, GSList **BasePentas, GSList **Pentagons, int Target) {
	
	const int nNodes = g_slist_length(*Nodes);
	
	ThreadData *ptr;
	int rc;
	pthread_t id_array[NTHREADS];
	void *status;
	int stride, extra, i;
	GSList *start_block_a;
	
	stride = nNodes / NTHREADS;
	extra  = nNodes % NTHREADS;
	start_block_a = *Nodes;
	
	for(i=0; i<NTHREADS; i++) {
		ptr = (ThreadData*)malloc(sizeof(ThreadData));
		// Thread data block init section
		ptr->nodes = Nodes;
		ptr->index = i;
		ptr->a = start_block_a;
		
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
		printf("Thread index = %02d	",((ThreadData*)status)->index );
		printf("Thread count = %02d	",((ThreadData*)status)->block_count);
		printf("Thread found = %02d\n", ((ThreadData*)status)->found);
	}
	
	// All threads recovered
	return 0;
}
