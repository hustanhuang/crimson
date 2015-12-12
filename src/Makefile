IDIR= include
CC= gcc-5
CFAGS= -Wall -Wextra -std=c99 -I$(IDIR)

ODIR= obj

_DEPS= kq.h sock.h parse.h \
	   timed.h cmd.h funcs.h db.h obj.h \
	   list.h sds.h hash.h table.h set.h \
	   c_string.h c_list.h c_hash.h c_set.h
DEPS= $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJS= srv.o kq.o sock.o parse.o \
	   timed.o cmd.o funcs.o db.o obj.o \
	   list.o sds.o hash.o table.o set.o \
	   c_string.o c_list.o c_hash.o c_set.o
OBJS= $(patsubst %, $(ODIR)/%, $(_OBJS))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f server *~ $(IDIR)/*~ $(ODIR)/*.o
