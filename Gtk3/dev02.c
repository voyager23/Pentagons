/*
 * dev02.c
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
 * Primes> 31,41,07,05,41,11,19,13,11,43,23,07,43,17,05,19,17,31,13,23,
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int GroupSize = 10;	// this will be extern in final version

char **extract_base_pentas(FILE *fp, int *nUnique);
 
char **extract_base_pentas(FILE *fp, int *nUnique) {
		
	char **Head = NULL;
	char **pStr = NULL;
	char *pToken;
	char buffer[256];
	int i,j;
	
	// set up and sanity checks
	*nUnique=0;
	if(fp==NULL) return;
	// parse file
	while(fgets(buffer,255,fp) != NULL) {
		if(strstr(buffer,"Primes>") == NULL) continue;
		*nUnique += 1;
		Head = (char**)realloc(Head, GroupSize*sizeof(char*)*(*nUnique) );
		pStr = Head + ((*nUnique - 1) * GroupSize);
		pToken = strtok(buffer," ,");	// capture and discard 'Primes>'
		
		for(i=0;i<5;i++) {
			for(j=0;j<4;j++) {
				pToken = strtok(NULL," ,");
				if((j==0)||(j==3)) continue;
				*pStr = (char*)malloc(sizeof(char)*(strlen(pToken)+1));
				strcpy(*pStr,pToken);
				pStr += 1;
			}
		}
	}
	return Head;
}

int main(int argc, char **argv) {
	char **Head = NULL;
	char **pStr = NULL;
	int nUnique = 0;	
	FILE *fp;
	
	fp = fopen("../DatFiles/Penta_84.dat","r");
	if(fp==NULL) {
		printf("Unable to open .dat file\n");
		exit(1);
	}
	printf("calling extract_base_pentas\n");
	Head = extract_base_pentas(fp, &nUnique);
	int i,j;
	pStr = Head;
	for(i=0;i<nUnique;i++) {
		for(j=0;j<GroupSize;j++) {
			printf("%s ", *pStr );
			pStr += 1;
		}
		printf("\n");
	}
	fclose(fp);
	return 0;
}
	
	
