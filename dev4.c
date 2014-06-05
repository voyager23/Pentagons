/*
 * dev4.c
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
 * 
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

// index table for base to working

int rot_ref [30][5] = {
	
	{00, 01, 02, 03, 04}, 
	{00, 04, 03, 02, 01}, 
	{02, 01, 00, 04, 03}, 
	{04, 03, 02, 01, 00}, 
	{01, 00, 04, 03, 02}, 
	{03, 02, 01, 00, 04},

	{04, 00, 01, 02, 03},
	{04, 03, 02, 01, 00},
	{01, 00, 04, 03, 02},
	{03, 02, 01, 00, 04},
	{00, 04, 03, 02, 01},
	{02, 01, 00, 04, 03},

	{03, 04, 00, 01, 02},
	{03, 02, 01, 00, 04},
	{00, 04, 03, 02, 01},
	{02, 01, 00, 04, 03},
	{04, 03, 02, 01, 00},
	{01, 00, 04, 03, 02},

	{02, 03, 04, 00, 01},
	{02, 01, 00, 04, 03},
	{04, 03, 02, 01, 00},
	{01, 00, 04, 03, 02},
	{03, 02, 01, 00, 04},
	{00, 04, 03, 02, 01},

	{01, 02, 03, 04, 00},
	{01, 00, 04, 03, 02},
	{03, 02, 01, 00, 04},
	{00, 04, 03, 02, 01},
	{02, 01, 00, 04, 03},
	{04, 03, 02, 01, 00}
};

//----------------------------------------------------------------------
int main(int argc, char **argv)
{
	const int Target=84;

	FILE *fp;
	char filename[80];
	GSList *Source=NULL, *Scan=NULL;
	int i,j,k,match,count;
	
	int base[5], working[5], query[5];
	
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
	parse_all_file(fp, &Source);
	fclose(fp);
	
	// main search loop
	count=0;
	while(Source != NULL) {
		
		// fill the base array
		for(i=0;i<5;i++) base[i] = *(PPTR(Source)+(i*4)+2);	//a2,b2,c2,c2,e2
		
		// use i to loop over rot_ref 
		for(i=0;i<30;i++) {
			for(j=0;j<5;j++) working[j] = base[ rot_ref[i][j] ];
			// scan list looking for match to working
			Scan = Source;
			while(Scan != NULL) {
				// setup the query array
				for(k=0;k<5;k++) query[k] = *(PPTR(Scan)+(i*4));	//a0,b0,c0,d0,e0
				
				// test base/query for equality
				match=1;
				for(k=0;k<5;k++) {
					if(working[k] != query[k]) {
						match=0;
						break;
					}
				}
				count+=1;
				if(match==1) {
					printf("+");
					// print "\n"
				} else {
					printf("-");
				}			
				// bump pointer
				Scan = Scan->next;
			} //while
		
		} //for i
		Source = Source->next;
	}
	printf("\nComplete - %d trials.\n",count);
}
		
		
		
		
		
	
