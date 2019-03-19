#include <cstring> // std::memcpy()


#include "game.h"



void Game::initGame()
{
	for (int i = 0; i < 5; ++i)
    {
        serverData[i].id = i;
        serverData[i].uX = 5;
        serverData[i].uY = 5 + i;
        serverData[i].uSkin = 'X';
        serverData[i].command = 0;
    }
}


void Game::recvData(char *buf)
{
	if (buf[0] == 2)
	{
	    std::memcpy(&clientData, &buf[1], sizeof(sData)); 

	    serverData[clientData.id].uSkin = clientData.uSkin;

	    if(clientData.command == 1) serverData[clientData.id].uX ++;
	    if(clientData.command == 2) serverData[clientData.id].uX --;
	    if(clientData.command == 3) serverData[clientData.id].uY ++;
	    if(clientData.command == 4) serverData[clientData.id].uY --;
	}
}


void Game::sendData(char *buf)
{
	buf[0] = 2;
	std::memcpy(&buf[1], &serverData, sizeof(sData) * 5);
}