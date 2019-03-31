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
    char recvBuff[1024];
    char sendBuff[1024];
    int bytes_read;
    int sendSize;
    int clientid;

    std::set<int> clients;



};