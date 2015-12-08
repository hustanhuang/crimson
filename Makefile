CC= gcc-5
CFAGS= -Wall -Wextra -std=c99

all: server

server: srv kq sock parse list sds hash table
	$(CC) $(CFLAGS) -o srv obj/srv.o obj/kq.o obj/sock.o obj/parse.o obj/list.o obj/sds.o

srv:
	$(CC) $(CFLAGS) -o obj/srv.o -c src/srv.c

kq:
	$(CC) $(CFLAGS) -o obj/kq.o -c src/kq.c

sock:
	$(CC) $(CFLAGS) -o obj/sock.o -c src/sock.c

parse:
	$(CC) $(CFLAGS) -o obj/parse.o -c src/parse.c

list:
	$(CC) $(CFLAGS) -o obj/list.o -c src/list.c

sds:
	$(CC) $(CFLAGS) -o obj/sds.o -c src/sds.c

hash:
	$(CC) $(CFLAGS) -o obj/hash.o -c src/hash.c

table:
	$(CC) $(CFLAGS) -o obj/table.o -c src/table.c

clean:
	rm srv *~ obj/* src/*~
