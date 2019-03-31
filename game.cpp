#include <cstring> // std::memcpy()
#include <iostream>
#include <vector>


#include "game.h"



Game::Game()
{

	frameNum = 0;


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

	if (recvBuffPtr[0] == 1) // Пустая команда
	{
		answerType = 4;
	}

	if (recvBuffPtr[0] == 2) // Создать нового игрока
	{
		answerType = 2;
	}

	if (recvBuffPtr[0] == 3) // Команда на перемещение
	{
		movePlayer();
		answerType = 4;
	}

}



void Game::sendData(char *sendBuff, int &sSize)
{
	sendBuffPtr = sendBuff;

	if (answerType == 2) // создать игрока
		createPlayer(sSize);

	if (answerType == 4) // отправить картинку и статус
	{ 
		sendScreen(sSize);
		sendStatus(sSize);
	}

}



void Game::createPlayer(int &sSize)
{
	std::memcpy(&createData, &recvBuffPtr[2], sizeof(CrtData)); 

    unit.id = clientidBuff;
    unit.skin = createData.skin;
    unit.x = 5;
    unit.y = 5 + clientidBuff;
    unit.pwr = 10 + clientidBuff;

    units.push_back(unit);
    frameNum ++;

	createData.id = clientidBuff;
//------------------------------------

	sSize = sizeof(CrtData);
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

			frameNum ++;
			//checkPointCollision(units[i].id);


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


	sSize = printObjects.size() * sizeof(PrintData);
	sendBuffPtr[0] = 4; // тип пакетиа
	sendBuffPtr[1] = printObjects.size();
	std::memcpy(&sendBuffPtr[2], printObjects.data(), sSize);
}



void Game::sendStatus(int &sSize)
{

	for(int i = 0; i < units.size(); ++i)
	{
		if(units[i].id == recvBuffPtr[2])
		{
			playerStatus.pwr = units[i].pwr;
			break;
		}
	}

	playerStatus.frameNum = frameNum;

	std::memcpy(&sendBuffPtr[sSize + 2], &playerStatus, sizeof(StatusData));
	sSize += sizeof(StatusData);

}

void Game::sendZero(int &sSize)
{
	sendBuffPtr[0] = 1;
	sSize = 0;
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



// void Game::createPwrPoints()
// {
// 	pwrPoints.resize(3);

// 	pwrPoints[0].skin = '1';
// 	pwrPoints[0].x = 10;
// 	pwrPoints[0].y = 10;

// 	pwrPoints[1].skin = '1';
// 	pwrPoints[1].x = 15;
// 	pwrPoints[1].y = 25;

// 	pwrPoints[2].skin = '1';
// 	pwrPoints[2].x = 17;
// 	pwrPoints[2].y = 31;

// }


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




