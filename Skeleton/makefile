# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

all: link

link: compile
	@echo "linking"
	g++ -o bin/main bin/main.o bin/Auxiliary.o

compile: src/main.cpp src/Auxiliary.cpp
	@echo: "compiling files"
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/main.o src/main.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp

clean:
	@echo: "cleaning bin directory"
	rm -f bin/* 
