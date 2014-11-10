all: client server

client: client.c
	gcc -o client client.c

server: server.o lib/recvsend.o lib/user.o
	gcc -o server server.o lib/recvsend.o lib/user.o

server.o: server.c lib/header.h
	gcc -c -o server.o server.c

lib/recvsend.o: lib/recvsend.c lib/header.h
	gcc -c -o lib/recvsend.o lib/recvsend.c

lib/user.o: lib/user.c lib/header.h
	gcc -c -o lib/user.o lib/user.c

.PHONY: clean
clean:
	@rm -f client server server.o lib/recvsend.o lib/user.o
	@echo "Cleaned"
