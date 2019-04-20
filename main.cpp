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



void inputHeandler(Server &srv)
{
    char a;
    while(true)
    {
        a = getchar();
        if(a == 'q')
        {
            srv.closeConnection();
            return;
        }
    }
}


int main()
{
    Game gm;
    Server srv;

    std::thread thr1(inputHeandler, std::ref(srv));

    srv.initServer();

    srv.mainLoop(gm);

    std::cout << "End?" << std::endl;
    thr1.join();
    return 0;
}