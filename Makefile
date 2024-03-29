# File Makefile

all: bifid clean
	@echo "Compilation Completed!"
	@echo "To run type: ./bifid help"

bifid: main.o bifid.o
	g++ -std=c++11 -g -Wall main.o bifid.o -o bifid

main.o: main.cpp
	g++ -std=c++11 -c -g -Wall main.cpp -o main.o

bifid.o: bifid.cpp
	g++ -std=c++11 -c -g -Wall bifid.cpp -o bifid.o

clean:
	rm *.o

