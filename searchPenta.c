/*
 * searchPenta.c
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

void searchPenta(int Target) {
	
	GSList *Primes = NULL;
	GSList *Nodes  = NULL;
	GSList *WorkingList = NULL;
	GSList *Pentagons = NULL;
	
	
	int n_primes, n_nodes, n_pentagons;
	int HiPrime = Target + 2;
	
	//printf("Starting setupPrimeLinkedList\n");
	n_primes = setupPrimeLinkedList(&Primes, HiPrime);
	//printf("n_primes: %d \n", n_primes);
	
	n_nodes = searchNodesLinkedList(&Primes, &Nodes, Target);
	//printf("n_nodes: %d \n", n_nodes);
	
	n_pentagons = searchPentagonLinkedList(&Nodes, &WorkingList, &Pentagons, Target);
	//printf("n_pentagons: %d\n",n_pentagons);
	
	printf("Target: %3d  n_primes: %3d  n_nodes: %4d  n_unique: %4d\n", Target, n_primes,n_nodes, g_slist_length(WorkingList));
	
	//printf("\n%d items in WorkingList\n", g_slist_length(WorkingList));

#if(0)
	GSList *p = WorkingList;
	while(p != NULL) {
		printf("==========\n");
		printRing5_compact(RPTR(p));
		p = p->next;

	}
#endif

}
