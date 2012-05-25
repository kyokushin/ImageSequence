CC = gcc

LIB = -L/usr/local/lib -L/usr/lib -L../lib/ \
	  `pkg-config --libs opencv` -lm -lboost_program_options -lboost_filesystem
INCLUDE = `pkg-config --cflags opencv`
		  

.SUFFIXES: .cpp .o

all: main

.cpp.o:
	$(CC) -c $(CFLAGS) $<

main: main.cpp ImageSequence.h
	$(CC) -O2 -o $@ $@.cpp $(INCLUDE) $(LIB)

