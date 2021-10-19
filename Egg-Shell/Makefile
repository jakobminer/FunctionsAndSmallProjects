#C for object files, E for executables, A for all, L for lib flags
CFLAGS=-c -Wall -Werror -pedantic
EFLAGS=-Wall -Werror -pedantic -l
AFLAGS=-o esh -Wall -Werror -pedantic -l

esh: esh.o
	cc -o esh $(EFLAGS) esh.o

esh.o: esh.c
	cc $(CFLAGS) esh.c

all: esh.c
	cc $(AFLAGS) esh.c

clean:
	rm -f esh.o && rm -f esh

run: esh
	./esh
