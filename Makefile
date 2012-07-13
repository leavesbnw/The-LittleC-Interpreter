#C_FLAGS = -Wall -std=c99 -Ofast -march=native 
C_FLAGS = -Wall -std=c99 -g -O0 -march=native 
CC = gcc -c
LD = gcc
HEADER =

all: *.c *.h 
	gcc $(C_FLAGS) $(HEADER) *.c  -o lisp

clean:
	rm -rf lisp *.o
