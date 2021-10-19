assignment5: assignment5.o random.o random.h
	gcc -o assignment5 assignment5.o random.o random.h -lm

assignment5.o: assignment5.c random.h
	gcc -c assignment5.c -lm

random.o: random.c random.h
	gcc -c random.c -lm

.PHONY : clean
clean :
	-rm assignment5 *.o

all:
	gcc -c random.c -lm
	gcc -c assignment5.c -lm
	gcc -o assignment5 assignment5.o random.o random.h -lm

run:
	./assignment5