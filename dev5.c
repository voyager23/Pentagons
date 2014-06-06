/*
 * dev5.c
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
 * It is sufficient to write each rotation [5] plus 1 reflection for each rotation [5].
 * Total of 10 unique pentagons.
 * ABCDE  EABCD  DEABC  CDEAB  BCDEA  
 *
 * aedcb  EDCBA  DCBAE  CBAED  BAEDC  
 *
 * CBAED  BAEDC  aedcb  EDCBA  DCBAE  
 *
 * EDCBA  DCBAE  CBAED  BAEDC  aedcb  
 *
 * BAEDC  aedcb  EDCBA  DCBAE  CBAED  
 *
 * DCBAE  CBAED  BAEDC  aedcb  EDCBA 
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SWAPCHAR(a,b) {char t=*((char*)a);*((char*)a)=*((char*)b);*((char*)b)=t;}

#define ROTATE(config) {SWAPCHAR(config+0,config+4);SWAPCHAR(config+4,config+3);SWAPCHAR(config+3,config+2);SWAPCHAR(config+2,config+1);}

#define REFLECT(config,pivot) {SWAPCHAR(config+((pivot+4)%5), config+((pivot+6)%5));  SWAPCHAR(config+((pivot+3)%5), config+((pivot+7)%5));}




const int length=16;

int main(int argc, char **argv)
{
	char *config[6][5];
	int r,c;
	char *initial = "ABCDE";
	
	for(r=0;r<6;r++) {
		for(c=0;c<5;c++) {
			config[r][c] = (char*)malloc(sizeof(char)*length);
		}
	}
	
	strcpy(config[0][0],initial);	// initial string
	for(c=1;c<5;c++) {
		strcpy(config[0][(c)], config[0][c-1]);
		ROTATE(config[0][c]);
	}

	for(r=1;r<6;r++) {
		for(c=0;c<5;c++) {
			strcpy(config[r][c], config[0][c]);
			REFLECT(config[r][c], r-1);
		}
	}
	
	printf("\n");
	for(r=0;r<6;r++) {
		for(c=0;c<5;c++) printf("%s  ", config[r][c]);
		printf("\n\n");
	}
	
	return 0;
}

