#include <cstring> // std::memcpy()
#include <iostream>
#include <vector>


#include "game.h"




void Game::recvData(char *recvBuf)
{
	if (recvBuf[0] == 2)
	{
	    std::memcpy(&clientData, &recvBuf[1], sizeof(sData)); 

	    // serverData[clientData.id].skin = clientData.skin;

	    // if(clientData.command == 1) serverData[clientData.id].x ++;
	    // if(clientData.command == 2) serverData[clientData.id].x --;
	    // if(clientData.command == 3) serverData[clientData.id].y ++;
	    // if(clientData.command == 4) serverData[clientData.id].y --;
		std::cout << "test\n";
	    
	}
}











void Game::sendData(char *sendBuf, int &sdSize)
{

	serverData.resize(5);
	for (int i = 0; i < 5; ++i)
	{
		serverData[i].skin = 'X';
		serverData[i].x = 5;
		serverData[i].y = 5 + i;
	}

	sdSize = serverData.size();
	sendBuf[0] = 2; // тип пакетиа
	sendBuf[1] = sdSize; // размер
	std::memcpy(&sendBuf[2], serverData.data(), sdSize * sizeof(sData));

}