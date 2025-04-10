all: ft

# Dependency rules for file targets 
ft: ft.o dynarray.o path.o nodeFT.o ft_client.o checkerFT.o 
	gcc217 ft.o ft_client.o nodeFT.o path.o checkerFT.o dynarray.o -o ft
ft.o: ft.c ft.h dynarray.h path.h nodeFT.h checkerFT.h a4def.h  
	gcc217 -c ft.c
checkerFT.o: checkerFT.c checkerFT.h nodeFT.h
	gcc217 -c checkerFT.c
ft_client.o: ft_client.c ft.h a4def.h
	gcc217 -c ft_client.c
nodeFT.o: nodeFT.c nodeFT.h checkerFT.h a4def.h
	gcc217 -c nodeFT.c
path.o: path.c path.h
	gcc217 -c path.c
dynarray.o: dynarray.c dynarray.h
	gcc217 -c dynarray.c