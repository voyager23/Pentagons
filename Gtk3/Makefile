#Makefile

p_view: p_view.c p_utils.o p_view.h
	gcc -g `pkg-config --cflags gtk+-3.0` p_view.c p_utils.o -o p_view `pkg-config --libs gtk+-3.0`
	

p_utils.o: p_utils.c
	gcc -g `pkg-config --cflags gtk+-3.0` -c p_utils.c
	
#-----------------------------------------------------------------------

scaledpng: scaledpng.c
	gcc `pkg-config --cflags gtk+-3.0` scaledpng.c -o scaledpng `pkg-config --libs gtk+-3.0`


