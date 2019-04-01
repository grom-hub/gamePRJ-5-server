#include <cstring> // std::memcpy()
#include <iostream>
#include <vector>


#include "game.h"



Game::Game()
{

	serverFrameNum = 1;


	pwrPoints.resize(3);

	pwrPoints[0].skin = '1';
	pwrPoints[0].x = 10;
	pwrPoints[0].y = 10;

	pwrPoints[1].skin = '1';
	pwrPoints[1].x = 15;
	pwrPoints[1].y = 25;

	pwrPoints[2].skin = '1';
	pwrPoints[2].x = 17;
	pwrPoints[2].y = 31;
}



void Game::recvData(char *recvBuff, int clientid)
{
	recvBuffPtr = recvBuff;
	clientidBuff = clientid;

	if(serverFrameNum > 100) // цикл счетчика
		serverFrameNum = 1;


	if (recvBuffPtr[0] == 2) // Создать нового игрока
	{
		answerType = 2;
	}

	if (recvBuffPtr[0] == 3) // Стандартный режим
	{
		movePlayer();

		if(recvBuffPtr[3] != serverFrameNum) // Проверить номер кадра клиента
			answerType = 4;
		else	
			answerType = 5;
	}

}



void Game::sendData(char *sendBuff, int &sendSize)
{
	sendBuffPtr = sendBuff;

	if (answerType == 2) // создать игрока
		createPlayer(sendSize);

	if (answerType == 4) // отправить картинку и статус
	{ 
		sendScreen(sendSize);
		addStatus(sendSize);
	}

	if (answerType == 5)
	{
		sendZero(sendSize);
	}

}



void Game::createPlayer(int &sendSize)
{

    unit.id = clientidBuff;
    unit.skin = recvBuffPtr[1];
    unit.x = 5;
    unit.y = 5 + clientidBuff;
    unit.pwr = 10 + clientidBuff;

    units.push_back(unit);
    serverFrameNum ++;

	sendBuffPtr[0] = 2;
	sendBuffPtr[1] = clientidBuff;
	sendSize = 2;

	std::cout << "Create person id = " << clientidBuff << "\n";
}


void Game::movePlayer()
{
	for(int i = 0; i < units.size(); ++i)
	{
		if(units[i].id == recvBuffPtr[1])
		{
			if(recvBuffPtr[2] == 1)
				units[i].x++;
			if(recvBuffPtr[2] == 2)
				units[i].x--;
			if(recvBuffPtr[2] == 3)
				units[i].y++;
			if(recvBuffPtr[2] == 4)
				units[i].y--;
			if(recvBuffPtr[2] != 0)
				serverFrameNum ++;
			//checkPointCollision(units[i].id);


			break;
		}
	}
}



void Game::sendScreen(int &sendSize)
{
	printObjects.clear();
	printObjects.reserve(units.size() + pwrPoints.size());

	for (int i = 0; i < pwrPoints.size(); ++i)
	{
		printObject.skin = pwrPoints[i].skin;
		printObject.x = pwrPoints[i].x;
		printObject.y = pwrPoints[i].y;
		
		printObjects.push_back(printObject);
	}

	for (int i = 0; i < units.size(); ++i)
	{
		printObject.skin = units[i].skin;
		printObject.x = units[i].x;
		printObject.y = units[i].y;

		printObjects.push_back(printObject);
	}


	sendSize = printObjects.size() * sizeof(PrintObjectData);
	sendBuffPtr[0] = 4; // тип пакетиа
	sendBuffPtr[1] = printObjects.size();
	std::memcpy(&sendBuffPtr[3], printObjects.data(), sendSize);
}



void Game::addStatus(int &sendSize)
{

	for(int i = 0; i < units.size(); ++i)
	{
		if(units[i].id == recvBuffPtr[1])
		{
			printStatus.pwr = units[i].pwr;
			break;
		}
	}

	sendBuffPtr[2] = serverFrameNum;

	std::memcpy(&sendBuffPtr[sendSize + 3], &printStatus, sizeof(PrintStatusData));
	sendSize += sizeof(PrintStatusData) + 3;

}



void Game::sendZero(int &sendSize)
{
	sendBuffPtr[0] = 5;
	sendSize = 1;
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
	serverFrameNum ++;
	std::cout << "deletePlayer - " << clientid << "\nunits size = " << units.size() << "\n";
}




// void checkPointCollision(int unitid)
// {
// 	for (int i = 0; i < pwrPoints.size(); ++i)
// 	{
// 		if(units[unitid].x == pwrPoints[i].x && units[unitid].y == pwrPoints[i].y)
// 		{
// 			units[unitid].pwr ++;
// 			break;
// 		}
// 	}
// }




