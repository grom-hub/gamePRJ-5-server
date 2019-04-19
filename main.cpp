// #include <sys/time.h>
// #include <unistd.h> // close()
// #include <fcntl.h>
// #include <algorithm> // max_element()
// #include <set>
//#include <cstring> // std::memcpy()
//#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include<thread>

#include "game.h"
#include "server.h"


#include <unistd.h> // usleep()



void threadFunction(char &a)
{
	while((a = getchar()) != 'q');
}


int main()
{
	char a;
    Game gm;
    Server srv;


    std::thread thr(threadFunction, std::ref(a));


    srv.initServer();

    srv.mainLoop(gm, a);

    std::cout << "Server - OFF" << std::endl;

    thr.join();
    return 0;
}