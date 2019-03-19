#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
// #include <sys/time.h>
// #include <unistd.h> // close()
// #include <fcntl.h>
// #include <algorithm> // max_element()
// #include <set>
#include <iostream>
//#include <cstring> // std::memcpy()
//#include <stdio.h>
//using namespace std;

#include "game.h"
#include "server.h"



int main()
{
   

    Game gm;
    Server srv;

    gm.initGame();

    srv.initServer();

    srv.mainLoop(gm);

   
    
    std::cout << "end" << std::endl;
    return 0;
}