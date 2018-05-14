CXX = g++
CPPFLAGS = -std=c++98 -Wall -pedantic-errors -g

TESTS_DIR = ./tests
GTEST_FLAGS = -lpthread -lgtest -I.

all: tp1 diff_complex test_all

diff_complex: diff_complex.o cmdline.o vector.hpp complex.hpp
	$(CXX) $(CPPFLAGS) $^ -o $@

diff_complex.o: diff_complex.cpp vector.hpp complex.hpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

tp1: main.o cmdline.o vector.hpp complex.hpp dft.hpp
	$(CXX) $(CPPFLAGS) $^ -o $@

cmdline.o: cmdline.cpp
	$(CXX) -c $(CPPFLAGS) $^ -o $@

main.o: main.cpp complex.hpp vector.hpp dft.hpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

clean :
	rm -f *.o

$(TESTS_DIR)/test_vector.o: $(TESTS_DIR)/test_vector.cpp vector.hpp
	$(CXX) $(CPPFLAGS) $(GTEST_FLAGS) -c $< -o $@

$(TESTS_DIR)/test_main.o: $(TESTS_DIR)/test_main.cpp
	$(CXX) $(CPPFLAGS) $(GTEST_FLAGS) -c $< -o $@

test_all: $(TESTS_DIR)/test_vector.o $(TESTS_DIR)/test_main.o
	$(CXX) $(CPPFLAGS) $(GTEST_FLAGS) $^ -o $@
