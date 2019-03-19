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
    char buf[1024];
    int bytes_read;

    std::set<int> clients;




};