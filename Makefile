all: msh 
msh: main.o Makefile
	g++ -g --std=c++11 -o msh main.o

main.o: main.cpp Makefile
	g++ -g --std=c++11 -c main.cpp

clean:
	./rd; rm -f msh *.o 