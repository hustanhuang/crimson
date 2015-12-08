IDIR= ../include
CC= gcc-5
CFAGS= -Wall -Wextra -std=c99 -I$(IDIR)

ODIR= obj

_DEPS= srv.h kq.h sock.h parse.h list.h sds.h hash.h table.h set.h
DEPS= $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJS= srv.o kq.o sock.o parse.o list.o sds.o hash.o table.o set.o
OBJS= $(patsubst %, $(ODIR)/%, $(_OBJS))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f server *~ $(IDIR)/*~ $(ODIR)/*.o
