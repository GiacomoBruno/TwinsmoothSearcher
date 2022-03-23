OPT=-O3
CFLAGS=$(OPT) -Wall -fopenmp -std=c++17
CC=gcc
CXX=g++


all: main.cpp nb.o logger.o file_manager.o tree.o twinsmooth.o twinsmooth_full.o list.o twinsmooth_range.o twinsmooth_kopt.o twinsmooth_k.o twinsmooth_k_growing.o
	$(CXX) $(CFLAGS) main.cpp nb.o logger.o file_manager.o list.o tree.o twinsmooth.o twinsmooth_full.o twinsmooth_range.o twinsmooth_kopt.o twinsmooth_k.o twinsmooth_k_growing.o -lgmp -o nb

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o
	rm nb
