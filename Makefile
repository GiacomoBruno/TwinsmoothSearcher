OPT=-O0
CFLAGS=$(OPT) -g -Wall -fopenmp -std=c++17
CC=gcc
CXX=g++


all: main.cpp nb.o file_manager.o tree.o twinsmooth.o twinsmooth_full.o list.o twinsmooth_range.o twinsmooth_kopt.o twinsmooth_growing_k.o
	$(CXX) $(CFLAGS) main.cpp nb.o file_manager.o list.o tree.o twinsmooth.o twinsmooth_full.o twinsmooth_range.o twinsmooth_kopt.o twinsmooth_growing_k.o -lgmp -o nb

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o
	rm nb
