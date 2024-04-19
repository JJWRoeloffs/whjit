SRCDIR=src
CC=gcc
CFLAGS=-I$(SRCDIR) -Wall -Wextra -Wswitch-enum -ggdb

ODIR=obj

_DEPS = utils.h parser.h interpreter.h
DEPS = $(patsubst %,$(SRCDIR)/%,$(_DEPS))

_OBJ = utils.o parser.o whjit.o interpreter.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

whjit: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR):
	mkdir -p $@

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS) $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rmdir $(ODIR)
