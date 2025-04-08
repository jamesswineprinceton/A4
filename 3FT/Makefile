# Dependency rules for non-file targets 
all: ft

# Dependency rules for file targets 
ft: ft.o ft_client.o  
	gcc217 ft.o ft_client.o -o dt
ft_client.o: ft_client.c ft.h
	gcc217 -c ft_client.c
ft.o: ft.c ft.h
	gcc217 -c ft.c
