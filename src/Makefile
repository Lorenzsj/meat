IDIR =../include
CC=gcc
CFLAGS=-Wall -pedantic -I$(IDIR) -L$(LDIR) -lconfuse -lzlog -lpthread -I$(PDIR) -L$(PDIR) -I$(PLDIR) -L$(PLDIR) -lperl -lm -Doff64_t=__off64_t

ODIR=obj
LDIR=/usr/local/lib
PDIR=/usr/lib/x86_64-linux-gnu/perl/5.22/CORE
PLDIR=/usr/lib/x86_64-linux-gnu/perl/5.24/CORE

_DEPS = meat.h camel.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o meat.o camel.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -shared

meat: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) 
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
