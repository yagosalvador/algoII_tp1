CXX = g++
CPPFLAGS = -std=c++98 -Wall -pedantic-errors -g -O3

all: tp0 diff_complex

diff_complex: diff_complex.o cmdline.o vector.hpp complex.hpp
	$(CXX) $^ -o $@

diff_complex.o: diff_complex.cpp vector.hpp complex.hpp
	$(CXX) -c $< -o $@

tp0: main.o cmdline.o vector.hpp complex.hpp dft.hpp
	$(CXX) $^ -o $@

cmdline.o: cmdline.cpp
	$(CXX) -c $(CPPFLAGS) $^ -o $@

main.o: main.cpp complex.hpp vector.hpp dft.hpp
	$(CXX) -c $(CPPFLAGS) $< -o $@
