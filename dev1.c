/*
 * dev1.c
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


#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int node_id;	// May not be useful
	int primes[4];
} Node;

typedef struct {
	int pentagon_id;
	Node *nodes[5];
} Pentagon;

enum p_n {
	p0, p1, p2, p3
};

int data[20] = {2,3,5,7,3,11,13,17,11,19,23,5,19,29,7,13,29,2,17,23};

//------------------------Utility Functions-----------------------------
void prt_Node(Node*);
void prt_Pentagon(Pentagon*);
void rotate(Pentagon*,int);
void mirror(Pentagon*,int);

void prt_Node(Node *n) {
	int i;
	printf("%02d -- ", n->node_id);
	for(i=0;i<4;i++) printf("%02d ", n->primes[i]);
	printf("\n");	
};

void prt_Pentagon(Pentagon *p) {
	int i;
	for(i=0;i<5;i++) prt_Node(p->nodes[i]);
	printf("\n");
};

void rotate(Pentagon *p,int n){
	Node *temp = NULL;
	int i,tmp;
	// Sanity Checks
	if(p==NULL) {printf("\nrotate: p is NULL pointer.\n");exit(1);}
	if((n<0)||(n>4)) {printf("rotate: n %d out of bounds.\n",n);exit(1);}
		while(n>0) {
		temp = p->nodes[0];
		for(i=1;i<5;i++) p->nodes[i-1] = p->nodes[i];
		p->nodes[4] = temp;
		n--;
	}
}
void mirror(Pentagon *p,int axis){	
	// Note: This Function Invalidates The Node_Id
	Node *temp = NULL;
	int i,tmp;
			
	// Sanity Checks
	if(p==NULL) {printf("\nmirror: p is NULL pointer.\n");exit(1);}
	if((axis<0)||(axis>4)) {printf("mirror: axis %d out of bounds.\n",axis);exit(1);}
	
	// swap 'inner pair'
	temp = p->nodes[ (axis+1)%5 ];
	p->nodes[ (axis+1)%5 ] = p->nodes[ (axis+4)%5 ];
	p->nodes[ (axis+4)%5 ] = temp;
	// swap 'outer pair'
	temp = p->nodes[ (axis+2)%5 ];
	p->nodes[ (axis+2)%5 ] = p->nodes[ (axis+3)%5 ];
	p->nodes[ (axis+3)%5 ] = temp;
	// for each node swap p0/p1 and p2/p3
	for(i=0;i<5;i++) {
		
		// Note: This Function Invalidates The Node_Id
		p->nodes[i]->node_id = -1;
		// ...........................................
		
		tmp = p->nodes[i]->primes[0];
		p->nodes[i]->primes[0] = p->nodes[i]->primes[1];
		p->nodes[i]->primes[1] = tmp;
		tmp = p->nodes[i]->primes[2];
		p->nodes[i]->primes[2] = p->nodes[i]->primes[3];
		p->nodes[i]->primes[3] = tmp;
	}		
}

//----------------------------------------------------------------------

int main(int argc, char **argv)
{
	int i,j;	// general purpose index
	
	// For testing we need 5 Nodes and 1 Pentagon;
	Pentagon *pentagon = (Pentagon*)malloc(sizeof(Pentagon));
	pentagon->pentagon_id = 99;
	
	for(i=0;i<5;i++) {		
		pentagon->nodes[i] = (Node*)malloc(sizeof(Node));
		pentagon->nodes[i]->node_id = i;
		for(j=0;j<4;j++) pentagon->nodes[i]->primes[j] = data[(i*4)+j];
	};
	
	// Check configuration
	prt_Pentagon(pentagon);
	
	// call mirror
	rotate(pentagon, 2);
	
	// Check configuration
	prt_Pentagon(pentagon);
	
		
	// call mirror
	rotate(pentagon, 3);
	
	// Check configuration
	prt_Pentagon(pentagon);
	
		
	return 0;
}

