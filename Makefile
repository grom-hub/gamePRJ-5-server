start: Server
	./Server

Server: main.cpp
	g++ -o Server main.cpp game.cpp -lncurses -std=c++11

clean:
	rm Server
