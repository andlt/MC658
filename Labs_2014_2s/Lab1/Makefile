
CC=gcc
CPPC=g++
CFLAGS= -Wall -lm
DEBUGFLAGS= -g -O0 -fno-inline
OPTFLAGS= -O2

c: clean
c: COMPILER=$(CC)
c: CFLAGS += $(OPTFLAGS)
c: tsp

c-debug: clean
c-debug: COMPILER=$(CC)
c-debug: CFLAGS += $(DEBUGFLAGS)
c-debug: tsp

cpp: clean
cpp: COMPILER=$(CPPC)
cpp: CFLAGS += $(OPTFLAGS)
cpp: tsp

cpp-debug: clean
cpp-debug: COMPILER=$(CPPC)
cpp-debug: CFLAGS += $(DEBUGFLAGS)
cpp-debug: tsp

tsp: main.o tsp.o
	$(COMPILER) $(CFLAGS) -o tsp main.o tsp.o
	
main.o:
	$(COMPILER) $(CFLAGS) -o main.o -c main.c 

tsp.o: 
	$(COMPILER) $(CFLAGS) -o tsp.o -c tsp.c 

clean: 
	rm -rf *.o
