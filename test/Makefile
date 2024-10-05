all:
	../main test.hpp testClass
	g++ -c test_c.cpp -o test_c.o
	gcc -c main.c -o main.o
	g++ test_c.o main.o -o main

clean:
	rm -f test_c.o main.o test_c.cpp test_c.h main
