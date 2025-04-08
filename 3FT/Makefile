# Dependency rules for non-file targets 
all: ft

# Dependency rules for file targets 
ft: ft.o ft_client.o  
	gcc217 ft.o ft_client.o -o ft
ft_client.o: ft_client.c ft.h a4def.h
	gcc217 -c ft_client.c
ft.o: ft.c ft.h a4def.h nodeFT.o
	gcc217 -c ft.c
nodeFT.o: nodeFT.c nodeFT.h
	gcc217 -c nodeFt.c