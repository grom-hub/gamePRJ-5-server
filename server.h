#pragma once

#include <set>

#include "game.h"



class Server
{
public:
    void initServer();
    void mainLoop(Game &gm);




private:
    int listener;
    struct sockaddr_in addr;
    char recvBuf[1024];
    char sendBuf[1024];
    int bytes_read;
    int sdSize;

    std::set<int> clients;




};