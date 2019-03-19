serverMy: main.cpp
	g++ -o serverMy main.cpp server.cpp game.cpp -std=c++11

clean:
	rm serverMy