/*
 * penta.c
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


#include "./penta.h"

int main(int argc, char **argv) {
	
	// None threaded version
	
	long Target = 84;	
	
	if(argc==2) {
		Target = strtol(argv[1],NULL,10);
		if((Target % 2) == 1) Target -= 1;
		printf("Target value is set to %ld\n",Target);
	} else {
		printf("Using default Target %ld.\n",Target);
	}
	
	searchPenta(Target);
	
	return 0;
}

