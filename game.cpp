#include <cstring> // std::memcpy()
#include <iostream>
#include <vector>


#include "game.h"




void Game::recvData(char *recvBuf, int clientid)
{
	//std::cout << "---------\n";
	if (recvBuf[0] == 1)
	{
		answerType = 4;
	}


	if (recvBuf[0] == 2) // Создание персонажа
	{

	    std::memcpy(&createData, &recvBuf[2], sizeof(crtData)); 

	    int newid = units.size() + 1;

	    unit.id = clientid;
	    unit.skin = createData.skin;
	    unit.x = 5;
	    unit.y = 5 + clientid;

	    units.push_back(unit);

		std::cout << "Create person id = " << clientid << "\n";


		answerType = 2;
		createData.id = clientid;
	}


	if (recvBuf[0] == 3)
	{
		for(int i = 0; i < units.size(); ++i)
		{
			if(units[i].id == recvBuf[2])
			{
				if(recvBuf[3] == 1) units[i].x++;
				if(recvBuf[3] == 2) units[i].x--;
				if(recvBuf[3] == 3) units[i].y++;
				if(recvBuf[3] == 4) units[i].y--;
				break;
			}
		}
	}
}



void Game::sendData(char *sendBuf, int &sSize)
{
	//std::cout << "+++++++\n";
	if (answerType == 2)
	{
		sSize = sizeof(crtData);
		sendBuf[0] = 2; // тип пакетиа
		sendBuf[1] = sSize; // размер
		std::memcpy(&sendBuf[2], &createData, sSize);
	}


	if (answerType == 4)
	{
		sSize = units.size();
		sendBuf[0] = 4; // тип пакетиа
		sendBuf[1] = sSize; // размер
		std::memcpy(&sendBuf[2], units.data(), sSize * sizeof(unitBox));	
	}

}



void Game::deletePlayer(int clientid)
{
	for (int i = 0; i < units.size(); ++i)
	{
		if(units[i].id == clientid)
		{
			units.erase(units.begin() + i);
			break;
		}
	}
	std::cout << "deletePlayer - " << clientid << "\nunits size = " << units.size() << "\n";
}

// x ++
// x --
// y ++
// y --