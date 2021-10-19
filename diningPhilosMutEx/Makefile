assignment6: assignment6.o
	gcc -o assignment6 assignment6.o -lm -pthread

assignment6.o: assignment6.c
	gcc -c assignment6.c

.PHONY : clean
clean :
	-rm assignment6 *.o

all:
	gcc -c assignment6.c
	gcc -o assignment6 assignment6.o -lm -pthread

run:
	./assignment6