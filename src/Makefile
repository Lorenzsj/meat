IDIR =../include
CC=gcc
CFLAGS=-Wall -pedantic -I$(IDIR) -L$(LDIR) -lconfuse -lzlog -lpthread


ODIR=obj
LDIR =/usr/local/lib

_DEPS = meat.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o meat.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -shared

meat: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) 
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
