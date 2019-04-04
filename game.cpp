#include <cstring> // std::memcpy()
#include <iostream>
#include <vector>


#include "game.h"



Game::Game()
{
	serverFrameNum = 1;

	createStars();

	pwrPoints.resize(3);

	pwrPoints[0].skin = '1';
	pwrPoints[0].x = 7;
	pwrPoints[0].y = 20;

	pwrPoints[1].skin = '1';
	pwrPoints[1].x = 10;
	pwrPoints[1].y = 45;

	pwrPoints[2].skin = '1';
	pwrPoints[2].x = 10;
	pwrPoints[2].y = 55;

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
		if(recvBuffPtr[2] != 0) // Проверить наличие команды парамещения
			movePlayer();

		if(recvBuffPtr[3] != serverFrameNum) // Сравнить номер кадра клиента и сервера
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
    unit.x = 6;
    unit.y = 34;
    unit.pwr = 0;

    bool checkFreeSpace;
    do
    {
    	checkFreeSpace = true;
	    for(int i = 0; i < units.size(); ++i)
		{
			if(unit.x == units[i].x && unit.y == units[i].y)
			{
				checkFreeSpace = false;
				unit.y ++;
				break;
			}
		}
	}
    while(!checkFreeSpace);

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
			if(checkObstacle(i)) // Проверка использует recvBuffPtr[2]
			{
				if(recvBuffPtr[2] == 1)
					units[i].x++;
				if(recvBuffPtr[2] == 2)
					units[i].x--;
				if(recvBuffPtr[2] == 3)
					units[i].y++;
				if(recvBuffPtr[2] == 4)
					units[i].y--;

				serverFrameNum ++;

				checkPointCollision(i);
			}
			break;
		}
	}
}



void Game::sendScreen(int &sendSize)
{
	printObjects.clear();
	printObjects.reserve(units.size() + pwrPoints.size() + stars.size() * 7);

	for(int i = 0; i < stars.size(); ++i)
	{
		for(int j = 0; j <= 6; ++j)
		{
			printObject.skin = stars[i].skin[j];
			printObject.id = 0; // без 0 будет конфлик с units.id
			printObject.x = stars[i].x;
			printObject.y = stars[i].y[j];

			printObjects.push_back(printObject);
		}
	}
	
	for (int i = 0; i < pwrPoints.size(); ++i)
	{
		printObject.skin = pwrPoints[i].skin;
		printObject.id = 0; // без 0 будет конфлик с units.id
		printObject.x = pwrPoints[i].x;
		printObject.y = pwrPoints[i].y;
		
		printObjects.push_back(printObject);
	}

	for (int i = 0; i < units.size(); ++i)
	{
		printObject.skin = units[i].skin;
		printObject.id = units[i].id;
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



bool Game::checkObstacle(int checkindex)
{
	int moveX = 0;
	int moveY = 0;
	if(recvBuffPtr[2] == 1) moveX = 1;
	if(recvBuffPtr[2] == 2) moveX = -1;
	if(recvBuffPtr[2] == 3) moveY = 1;
	if(recvBuffPtr[2] == 4) moveY = -1;

	for(int i = 0; i < units.size(); ++i)
	{
		if(units[checkindex].x + moveX == units[i].x && units[checkindex].y + moveY == units[i].y)
		{
			return false;
		}
	}

	return true;
}



void Game::checkPointCollision(int unitIndex)
{
	for (int i = 0; i < pwrPoints.size(); ++i)
	{
		if(units[unitIndex].x == pwrPoints[i].x && units[unitIndex].y == pwrPoints[i].y)
		{
			units[unitIndex].pwr ++;
			break;
		}
	}
}



void Game::createStars()
{
	stars.resize(4);


	stars[0].id = 0;
    stars[0].skin[0] = ' ';
    stars[0].skin[1] = ' ';
    stars[0].skin[2] = '(';
    stars[0].skin[3] = '*';
    stars[0].skin[4] = ')';
    stars[0].skin[5] = ' ';
    stars[0].skin[6] = ' ';
    stars[0].x = 0;
    stars[0].y[0] = 15;
    stars[0].y[1] = 16;
    stars[0].y[2] = 17;
    stars[0].y[3] = 18;
    stars[0].y[4] = 19;
    stars[0].y[5] = 20;
    stars[0].y[6] = 21;
    stars[0].pwr = 0;

    stars[1].id = 1;
    stars[1].skin[0] = ' ';
    stars[1].skin[1] = '(';
    stars[1].skin[2] = '(';
    stars[1].skin[3] = '*';
    stars[1].skin[4] = ')';
    stars[1].skin[5] = ')';
    stars[1].skin[6] = ' ';
    stars[1].x = 8;
    stars[1].y[0] = 85;
    stars[1].y[1] = 86;
    stars[1].y[2] = 87;
    stars[1].y[3] = 88;
    stars[1].y[4] = 89;
    stars[1].y[5] = 90;
    stars[1].y[6] = 91;
    stars[1].pwr = 0;

    stars[2].id = 2;
    stars[2].skin[0] = '(';
    stars[2].skin[1] = '(';
    stars[2].skin[2] = '(';
    stars[2].skin[3] = '*';
    stars[2].skin[4] = ')';
    stars[2].skin[5] = ')';
    stars[2].skin[6] = ')';
    stars[2].x = -10;
    stars[2].y[0] = 45;
    stars[2].y[1] = 46;
    stars[2].y[2] = 47;
    stars[2].y[3] = 48;
    stars[2].y[4] = 49;
    stars[2].y[5] = 50;
    stars[2].y[6] = 51;
    stars[2].pwr = 0;


    stars[3].id = 3;
    stars[3].skin[0] = ' ';
    stars[3].skin[1] = '(';
    stars[3].skin[2] = '-';
    stars[3].skin[3] = '^';
    stars[3].skin[4] = '-';
    stars[3].skin[5] = ')';
    stars[3].skin[6] = ' ';
    stars[3].x = 7;
    stars[3].y[0] = 35;
    stars[3].y[1] = 36;
    stars[3].y[2] = 37;
    stars[3].y[3] = 38;
    stars[3].y[4] = 39;
    stars[3].y[5] = 40;
    stars[3].y[6] = 41;
    stars[3].pwr = 0;
}




