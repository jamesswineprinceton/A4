CC=gcc217

all: ft

# Dependency rules for file targets 
ft: ft.o ft_client.o nodeFT.o path.o checkerFT.o dynarray.o
	gcc217 ft.o ft_client.o nodeFT.o path.o checkerFT.o dynarray.o -o ft
ft_client.o: ft_client.c ft.h a4def.h
	gcc217 -c ft_client.c
ft.o: ft.c ft.h a4def.h nodeFT.o
	gcc217 -c ft.c
nodeFT.o: nodeFT.c nodeFT.h
	gcc217 -c nodeFT.c
path.o: path.c path.h
	gcc217 -c path.c
checkerFT.o: checkerFT.c checkerFT.h
	gcc217 -c checkerFT.c
dynarray.o: dynarray.c dynarray.h
	gcc217 -c dynarray.c