all: game
	./play

game:
	g++ GH.cpp -o play -std=c++11 -lpthread -lncurses
