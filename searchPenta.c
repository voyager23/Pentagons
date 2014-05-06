/*
 * searchPenta.c
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

void searchPenta(int Target) {
	
	GSList *Primes = NULL;
	GSList *Nodes  = NULL;	
	GSList *Pentagons = NULL;
	GSList *BasePentas = NULL;
	FILE *fp;
	char filename[128];
	int n,pr;
	int n_primes, n_nodes, n_pentagons;
	int HiPrime = Target + 2;
	
	n_primes = setupPrimeLinkedList(&Primes, HiPrime);
	//printf("n_primes: %d \n", n_primes);
	
	n_nodes = searchNodesLinkedList(&Primes, &Nodes, Target);
	//printf("n_nodes: %d \n", n_nodes);
	
	n_pentagons = searchPentagonLinkedList(&Nodes, &BasePentas, &Pentagons, Target);
	//printf("n_pentagons: %d\n",n_pentagons);
	
	printf("Target: %3d  n_primes: %3d  n_nodes: %4d  n_unique: %4d\n", Target, n_primes,n_nodes, g_slist_length(BasePentas));
	
#if(1)
	// write data to file for use by graphics
	sprintf(filename,"./DatFiles/Penta_%02d.dat",Target);
	fp = fopen(filename,"w");
	if(fp==NULL) {
		printf("Failed to open %s - stopping\n",filename);
		exit(1);
	}
	fprintf(fp,"Target: %3d  n_primes: %3d  n_nodes: %4d  n_unique: %4d\n", Target, n_primes,n_nodes, g_slist_length(BasePentas));
	GSList *p = BasePentas;
	while(p != NULL) {
		printf("==========\n");
		printRing5_compact(RPTR(p));
		fprintf(fp,"Primes> ");
		for(n=0;n<5;n++) {
			for(pr=0;pr<4;pr++) {
				fprintf(fp,"%02d,", RPTR(p)->nodes[n]->primes[pr]);
			}
		}
		fprintf(fp,"\n");		
		p = p->next;
	}
	fclose(fp);
#endif
}
