/*
 * dev3.c
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

#include <stdio.h>
#include <stdlib.h>

#define SWAPINT(a,b) {int t=*a;	*a=*b; *b=t;}
	
#define REFLECT(node,pivot) {SWAPINT(&node[((pivot+4)%5)],&node[((pivot+6)%5)]);\
	SWAPINT(&node[((pivot+3)%5)],&node[((pivot+7)%5)]);}

#define ROTATE(node) {SWAPINT(&node[0],&node[4]);SWAPINT(&node[4],&node[3]);\
	SWAPINT(&node[3],&node[2]);SWAPINT(&node[2],&node[1]);}


int main(int argc, char **argv)
{
	int nodes[5] = {0,1,2,3,4};
	int i,j,k;
	
	//for(i=0;i<5;i++) printf("%02d ", nodes[i]); printf("\n\n");
	 
	for(j=0;j<5;j++) {
		// print current config
		for(k=0;k<5;k++) printf("%02d ", nodes[k]); 
		printf("\n");
		// reflect and print
		for(i=0;i<5;i++) {
			REFLECT(nodes,i);
			for(k=0;k<5;k++) printf("%02d ", nodes[k]); printf("\n");
			// reflect restore
			REFLECT(nodes,i);			
		}
		// rotate
		ROTATE(nodes);	
		printf("\n");	
	} // for j
	
	for(i=0;i<5;i++) printf("%02d ", nodes[i]); printf("\n\n");
	
	return 0;
}

