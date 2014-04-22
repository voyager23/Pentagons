/*
 * setupPrimeLinkedList.c
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

int setupPrimeLinkedList(GSList** refPrimes, int HiPrime) {
	int count, buffer;
	FILE *fp;
	
	fp = fopen(PRIMES, "r");
	if(fp == NULL) {
		printf("Error: failed to open %s\n", PRIMES);
		exit(1);
	}
	fread(&buffer, sizeof(int), 1, fp);
	count = 1;
	while(buffer <= HiPrime) {
		// append value and update list head
		*refPrimes = g_slist_append(*refPrimes, GINT_TO_POINTER(buffer));
		count += 1;
		fread(&buffer, sizeof(int), 1, fp);
	}
	fclose(fp);
	return count;
}
