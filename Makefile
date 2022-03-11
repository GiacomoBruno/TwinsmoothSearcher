OPT=-O0
CFLAGS=$(OPT) -g -Wall -fopenmp -std=c++17 -fsanitize=leak
CC=clang
CXX=clang++


all: main.cpp file_manager.o tree.o twinsmooth.o twinsmooth_full.o list.o
	$(CXX) $(CFLAGS) main.cpp file_manager.o list.o tree.o twinsmooth.o twinsmooth_full.o -lgmp -o nb

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm nb
	rm *.o
