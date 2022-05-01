OPT=-O3
CFLAGS=$(OPT) -Wall -fopenmp -std=c++17
CC=gcc
CXX=g++


all: src/main.cpp nb.o logger.o file_manager.o tree.o static_twinsmooth.o twinsmooth.o prime_calculator.o prime_calculator_top_n.o twinsmooth_range.o twinsmooth_k.o twinsmooth_k_growing.o twinsmooth_range_growing.o twinsmooth_test.o
	$(CXX) $(CFLAGS) main.cpp nb.o logger.o file_manager.o tree.o static_twinsmooth.o twinsmooth.o prime_calculator.o prime_calculator_top_n.o twinsmooth_range.o twinsmooth_k.o twinsmooth_k_growing.o twinsmooth_range_growing.o twinsmooth_test.o -lgmp -o nb

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o
	rm nb
