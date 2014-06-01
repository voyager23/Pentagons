 /*
 * dev2.c
 * 
 * Copyright 2014 Michael <michael.tate@wanadoo.fr>
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
 * Scan the entire list of pentagons looking for a pentagon
 * which matches the "internal-inverted" pentagon.
 * Note: No matches found. mjt-26/05/14
 * 
 * Complete revision using a table of indexes to set up candidates
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <glib.h>
#include <pthread.h>

#define PPTR(gl) ((int *)((gl)->data))

// declarations
void parse_all_file(FILE *fp, GSList **Head);

// definitions
void parse_all_file(FILE *fp, GSList **Head) {
	
	char buffer[256];
	int i,j,*pArray;
	
	// set up and sanity checks
	if((fp==NULL)||(*Head != NULL)) {
		printf("Error: sanity check parse_all_file failed. \n");
		return;
	}
	
	// parse file
	while(fgets(buffer,255,fp) != NULL) {
		if(strstr(buffer,"Primes>") == NULL) continue;		
		(void)strtok(buffer," ,");	// capture and discard 'Primes>'
		// malloc space for a 20 integer array
		pArray = (int*)malloc(sizeof(int)*20);
		for(i=0;i<5;i++) {
			for(j=0;j<4;j++) *(pArray+i*4+j) = atoi(strtok(NULL," ,"));
		}
		// prepend Array to GSList
		*Head = g_slist_prepend(*Head,(void*)pArray);
	}
}

//----------------------------------------------------------------------
int main(int argc, char **argv)
{
	const int Target=84;

	FILE *fp;
	char filename[80];
	GSList *Head=NULL, *gl;
	int i,*primes,match;
	
	GSList *target, *candidate;
	int targ[5], cand[5];
	
	// look for .all files in Parent Directory
	sprintf(filename,"./DatFiles/Penta_%d.all", Target);
	printf("Looking for %s ... ", filename);
	fp = fopen(filename,"r");
	if(fp != NULL) {
		printf("Found.\n");
	} else {
		printf("not found --- Add valid target value to cmd line! --- Stopping.\n");
		exit(1);
	}
	parse_all_file(fp, &Head);
	fclose(fp);
	
	// check gslist
	gl=Head;
	while(gl!=NULL) {
		primes = PPTR(gl);
		for(i=0;i<20;i++) printf("%02d ", primes[i]);
		printf("\n");
		gl = gl->next;
	}
	printf("scanning.....");
	target = Head;
	while(target != NULL) {
		// fill targ[]
		targ[0] = (*PPTR(target)+5);	//b2
		targ[1] = (*PPTR(target)+9);	//c2
		targ[2] = (*PPTR(target)+13);	//d2
		targ[3] = (*PPTR(target)+17);	//e2
		targ[4] = (*PPTR(target)+1);	//a2
		candidate = Head;
		while(candidate != NULL) {
			// fill cand[]
			cand[0] = (*PPTR(candidate)+0);		//a0
			cand[1] = (*PPTR(candidate)+4);		//b0
			cand[2] = (*PPTR(candidate)+8);		//c0
			cand[3] = (*PPTR(candidate)+12);	//d0
			cand[4] = (*PPTR(candidate)+16);	//e0
			match = 1;
			// if targ == cand print result
			for(i=0;i<5;i++) {
				if(targ[i] != cand[i]) {
					match = 0;
					break;
				}
			}
			if(match == 1) printf("Match found.\n");
			candidate = candidate->next;
		}
		target = target->next;
	}
	printf("complete.\n");
	return 0;
}

