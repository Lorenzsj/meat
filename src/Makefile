IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

_DEPS = meat.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o meat.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -shared

meat: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) -L/usr/local/lib -lconfuse

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
