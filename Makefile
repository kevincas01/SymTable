CC = gcc217

# Dependency rules for non-file targets
all: testsymtablelist testsymtablehash
clobber: clean
rm -f *~ \#*\#
clean:
rm -f testsymtablelist testsymtablehash *.o

# Dependency rules for file targets
testsymtablelist: testsymtable.o symtablelist.o
$(CC) testsymtable.o symtablelist.o -o testsymtablelist
testsymtablehash: testsymtable.o symtablehash.o
$(CC) testsymtable.o symtablehash.o -o testsymtablehash
testsymtable.o: testsymtable.c symtable.h
$(CC) -c testsymtable.c
symtablelist.o: symtablelist.c symtable.h
$(CC) -c symtablelist.c
symtablehash.o: symtablehash.c symtable.h
$(CC) -c symtablehash.c