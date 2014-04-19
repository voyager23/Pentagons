# Makefile for Pentagons

# setup for 32/64 bits
arch = $(shell uname -m)

ifeq ($(arch), x86_64)
	CFLAGS = -Wall -g -m64 -std=c99
	
	INC = -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include
	
	LIB = -L/usr/lib64 -lglib-2.0 -Wl,-rpath=/usr/lib64
	
	RPATH = 
else
	CFLAGS = -Wall -g -m32 -std=c99 -O2
	
	INC = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include	
	
	LIB = -L/usr/local/lib -lglib-2.0  -Wl,-rpath=/usr/local/lib
	
	RPATH = 
endif

# ----------------------------------

DEPS = penta.h

OBJS = searchPenta.o setupPrimeLinkedList.o searchNodesLinkedList.o \
	utilities.o searchPentagonArray.o searchPentagonLinkedList.o

#----------default target-----------

penta: penta.c $(OBJS) $(DEPS)
	gcc $< $(CFLAGS) $(INC) $(LIB) $(OBJS) -o $@
	mv *.o ./objs
	
#-----------object files------------

%.o : %.c
	gcc -c $< $(CFLAGS) $(INC) $(LIB) -o $@	
