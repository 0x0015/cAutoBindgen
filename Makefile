all:
	g++ -std=c++20 -g main.cpp classDef.cpp -o main -lclang

clean:
	rm -f main
