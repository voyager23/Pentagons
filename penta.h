/*
 * penta.h
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
#ifndef __PENTA__
	#define __PENTA__

	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <time.h>
	#include <glib.h>

	#define PRIMES "./Primes.int"
	#define NPTR(gl) ((Node4 *)((gl)->data))
	#define RPTR(gl) ((Ring5 *)((gl)->data))

	typedef struct node4 {
		int node4_id;
		int primes[4];
	}Node4;

	typedef struct ring5 {
		Node4 *nodes[5];
	}Ring5;
	

	
	// -----functions-----
	void searchPenta(int);
	int setupPrimeLinkedList(GSList **, int);
	int searchNodesLinkedList(GSList **, GSList **, int);
	int searchPentagonArray(GSList **, GSList **, int Target);
	void printNode4(Node4 *np);
	void printRing5_compact(struct ring5 *rp);
	int searchPentagonLinkedList(GSList **, GSList **, GSList **, int Target);

	// -----utilities.c-----
	int adjacentNode4(Node4 *, Node4 *);
	int diagonalNode4(Node4 *, Node4 *);
	int add_Pentagon_to_list(GSList**Pentagons, GSList **WorkingList, struct ring5 *working);
	int find_Pentagon(GSList **Pentagons, struct ring5 *working);
	void rotate(struct ring5 *p,int n);
	void mirror(struct ring5 *p,int axis);
	struct ring5* deep_copy_ring5(const struct ring5 *p);
	
	// -----testcode-----
	int search4gonLinkedList(GSList **Nodes, GSList ** FourGons, int Target);
	int match4gons(GSList **FourGons, GSList **Pentagons);
	
#endif
