/*
 * penta.h
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
#ifndef __PENTA__
#define __PENTA__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <glib.h>

#define PRIMES "./Primes.int"
#define NPTR(gl) ((struct node4 *)((gl)->data))
#define RPTR(gl) ((struct ring5 *)((gl)->data))

struct node4 {
	int node4_id;
	int primes[4];
};

struct ring5 {
	struct node4 *nodes[5];
};

// -----functions-----
void searchPenta(int);
int setupPrimeLinkedList(GSList **, int);
int searchNodesLinkedList(GSList **, GSList **, int);
int searchPentagonArray(GSList **, GSList **, int Target);
void printNode4(struct node4 *np);
void printRing5_compact(struct ring5 *rp);

int searchPentagonLinkedList(GSList **, GSList **, int Target);

// -----utilities.c-----
int adjacentNode4(struct node4 *, struct node4 *);
int diagonalNode4(struct node4 *, struct node4 *);

#endif
