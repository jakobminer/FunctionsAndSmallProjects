main: main.o fileread.o sjf.o bubbleswap.o fifo.o iosched.o rr.o pr.o
	gcc -o main main.o fileread.o bubbleswap.o sjf.o fifo.o iosched.o rr.o pr.o -pthread

bubbleswap.o: bubbleswap.c mylib.h
	gcc -c bubbleswap.c -pthread

sjf.o: sjf.c mylib.h
	gcc -c sjf.c -pthread

main.o: main.c mylib.h
	gcc -c main.c -pthread

fileread.o: fileread.c mylib.h
	gcc -c fileread.c -pthread


fifo.o: fifo.c mylib.h
	gcc -c fifo.c -pthread

iosched.o: iosched.c mylib.h
	gcc -c iosched.c -pthread

rr.o: rr.c mylib.h
	gcc -c rr.c -pthread

pr.o: pr.c mylib.h
	gcc -c pr.c -pthread

