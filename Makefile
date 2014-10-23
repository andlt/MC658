
CC=gcc
CPPC=g++
CFLAGS= -Wall -lm
DEBUGFLAGS= -g -O0 -fno-inline
OPTFLAGS= -O2
LIBGLPK=-lglpk 

c: clean
c: COMPILER=$(CC)
c: CFLAGS += $(OPTFLAGS)
c: stp

c-debug: clean
c-debug: COMPILER=$(CC)
c-debug: CFLAGS += $(DEBUGFLAGS)
c-debug: stp

cpp: clean
cpp: COMPILER=$(CPPC)
cpp: CFLAGS += $(OPTFLAGS)
cpp: stp

cpp-debug: clean
cpp-debug: COMPILER=$(CPPC)
cpp-debug: CFLAGS += $(DEBUGFLAGS)
cpp-debug: stp

stp: main.o stp.o
	$(COMPILER) $(CFLAGS) -o stp main.o stp.o $(LIBGLPK)
	
main.o:
	$(COMPILER) $(CFLAGS) -o main.o -c main.c 

stp.o: 
	$(COMPILER) $(CFLAGS) -o stp.o -c stp.c 

clean: 
	rm -rf *.o

