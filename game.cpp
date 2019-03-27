#include <cstring> // std::memcpy()
#include <iostream>
#include <vector>


#include "game.h"




void Game::createPwrPoints()
{
	pwrPoints.resize(10);

	for (int i = 0; i < 10; ++i)
	{
		pwrPoints[i].skin = '1';
		pwrPoints[i].x = 10;
		pwrPoints[i].y = 10 + i;
	}

}


void Game::recvData(char *recvBuff, int clientid)
{
	recvBuffPtr = recvBuff;
	clientidBuff = clientid;

	if (recvBuffPtr[0] == 1)
		answerType = 4;

	if (recvBuffPtr[0] == 2)
		answerType = 2;

	if (recvBuffPtr[0] == 3)
		movePlayer();

}



void Game::sendData(char *sendBuff, int &sSize)
{
	sendBuffPtr = sendBuff;

	if (answerType == 2) // создать игрока
		createPlayer(sSize);

	if (answerType == 4) // отправить картинку и статусы
		sendScreen(sSize);

}



void Game::createPlayer(int &sSize)
{
	std::memcpy(&createData, &recvBuffPtr[2], sizeof(crtData)); 

    unit.id = clientidBuff;
    unit.skin = createData.skin;
    unit.x = 5;
    unit.y = 5 + clientidBuff;

    units.push_back(unit);

	createData.id = clientidBuff;
//------------------------------------

	sSize = sizeof(crtData);
	sendBuffPtr[0] = 2; // тип пакетиа
	sendBuffPtr[1] = sSize; // размер
	std::memcpy(&sendBuffPtr[2], &createData, sSize);

	std::cout << "Create person id = " << clientidBuff << "\n";
}


void Game::movePlayer()
{
	for(int i = 0; i < units.size(); ++i)
	{
		if(units[i].id == recvBuffPtr[2])
		{
			if(recvBuffPtr[3] == 1)
				units[i].x++;
			if(recvBuffPtr[3] == 2)
				units[i].x--;
			if(recvBuffPtr[3] == 3)
				units[i].y++;
			if(recvBuffPtr[3] == 4)
				units[i].y--;
			break;
		}
	}
}



void Game::sendScreen(int &sSize)
{

	printObjects.clear();
	printObjects.reserve(units.size() + pwrPoints.size());

	for (int i = 0; i < units.size(); ++i)
	{
		printObject.skin = units[i].skin;
		printObject.x = units[i].x;
		printObject.y = units[i].y;

		printObjects.push_back(printObject);
	}

	for (int i = 0; i < pwrPoints.size(); ++i)
	{
		printObject.skin = pwrPoints[i].skin;
		printObject.x = pwrPoints[i].x;
		printObject.y = pwrPoints[i].y;
		
		printObjects.push_back(printObject);
	}



	sSize = printObjects.size();
	sendBuffPtr[0] = 4; // тип пакетиа
	sendBuffPtr[1] = sSize; // размер
	std::memcpy(&sendBuffPtr[2], printObjects.data(), sSize * sizeof(printData));



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
