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
	GSList *Pentagons = NULL;
	
	int n_primes, n_nodes, n_pentagons;
	int HiPrime = Target + 2;
	
	printf("Starting setupPrimeLinkedList\n");
	n_primes = setupPrimeLinkedList(&Primes, HiPrime);
	printf("n_primes: %d \n", n_primes);
	
	n_nodes = searchNodesLinkedList(&Primes, &Nodes, Target);
	printf("n_nodes: %d \n", n_nodes);
#if(1)	
	time_t t1,t2;
	printf("Array search\n");
	time(&t1);
	n_pentagons = searchPentagonArray(&Nodes, &Pentagons, Target);
	time(&t2);
	printf("n_pentagons = %d in %.f seconds\n", n_pentagons,difftime(t2,t1));
	printf("Length of Pentagons list %d\n", g_slist_length(Pentagons));
#endif
}
