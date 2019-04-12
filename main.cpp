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

#include "game.h"
#include "server.h"



int main()
{
    Game gm;
    Server srv;

    srv.initServer();

    srv.mainLoop(gm);

    std::cout << "end??" << std::endl;
    return 0;
}