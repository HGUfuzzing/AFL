JSON_C_DIR=/usr/local
CFLAGS += -Wall -I$(JSON_C_DIR)/include
LDFLAGS+= -L$(JSON_C_DIR)/lib -ljson-c -static


server : server.o socket_wrapper.o
	gcc -o server server.o socket_wrapper.o $(LDFLAGS)

server.o : server.c socket_wrapper.h
	gcc -c server.c $(CFLAGS)

socket_wrapper.o : socket_wrapper.c socket_wrapper.h
	gcc -c socket_wrapper.c $(CFLAGS)

test : test.c
	gcc -o test test.c $(CFLAGS) $(LDFLAGS) 

clean : 
	rm -f server server.o socket_wrapper.o test
