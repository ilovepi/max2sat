CC=clang++
INCLUDE=~/boost_1_57_0
FLAGS=-std=c++11 -stdlib=libc++


all: solver.cpp solver.hpp
	$(CC) solver.cpp -I $(INCLUDE) $(FLAGS)

clean:
	rm a.out