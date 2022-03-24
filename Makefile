CC=g++
CFLAGS=-fdiagnostics-color=always -g

all: 
	$(CC) $(CFLAGS) ./src/main.cpp -o ./bin/main -lncurses