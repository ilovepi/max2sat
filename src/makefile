CC=clang++
INCLUDE=~/boost_1_57_0
#SOURCE=src
FLAGS=-std=c++11 -stdlib=libc++ -O2 #-g -fsanitize=address


all: solver.cpp solver.hpp
	$(CC) solver.cpp -I $(INCLUDE) $(FLAGS) 

clean:
	rm a.out
