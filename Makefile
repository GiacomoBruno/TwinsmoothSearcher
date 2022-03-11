OPT=-O3
CFLAGS=$(OPT) -Wall -fopenmp -std=c++17
CC=gcc
CXX=g++


all: main.cpp file_manager.o tree.o twinsmooth.o twinsmooth_full.o
	$(CXX) $(CFLAGS) $< -lgmp -o nb

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm nb
