test: main.o
	g++ -std=c++11 -g -Wall main.o -o test
main.o: main.cpp
	g++ -std=c++11 -g -Wall -c main.cpp
clean:
	rm test *.o
