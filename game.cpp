//#include <vector>
#include <cstring> // std::memcpy()
#include <iostream>
#include <algorithm> // std::fill()


#include "game.h"



Game::Game()
{

	unitsFrameNum = 1;
	pwrPointsFrameNum = 1;

	pwrPoints.resize(3);

	pwrPoints[0].skin = "(1)";
	pwrPoints[0].x = 7;
	pwrPoints[0].y = 20;
	pwrPoints[0].pwr = 1;

	pwrPoints[1].skin = "(2)";
	pwrPoints[1].x = 10;
	pwrPoints[1].y = 45;
	pwrPoints[1].pwr = 2;

	pwrPoints[2].skin = "((((0))))";
	pwrPoints[2].x = 10;
	pwrPoints[2].y = 55;
	pwrPoints[2].pwr = 30;

}




void Game::recvData(char *recvBuff, int clientid)
{
	recvBuffPtr = recvBuff;
	clientidBuff = clientid;

//-------------------------------------------
	if(unitsFrameNum > 100) // цикл счетчика
		unitsFrameNum = 1;
	if(pwrPointsFrameNum > 100)
		pwrPointsFrameNum = 1;


	// if(unitsFrameNum == 100) // временная зарядка pwrPoints
	// 	for(int i = 0; i < pwrPoints.size(); ++i)
	// 	{
	// 		pwrPoints[i].skin = '1';
	// 		pwrPointsFrameNum ++;
	// 	}
//-------------------------------------------

	if (recvBuffPtr[0] == 3) // Стандартный режим
	{
		if(recvBuffPtr[2] != 0) // Обработка команды управления персонажем
			if(recvBuffPtr[2] == 5)
				takePWR();
			else
				movePlayer();
	}
}



void Game::sendData(char *sendPreBuff, int &sendSize)
{
	sendPreBuffPtr = sendPreBuff;

	if (recvBuffPtr[0] == 2) // создать игрока
		createPlayer(sendSize);


	if(recvBuffPtr[0] == 3)
	{
		if(unitsFrameNum == recvBuffPtr[3] 
			&& pwrPointsFrameNum == recvBuffPtr[4])
			sendZero(sendSize);
		else
		{
			std::fill(printObjectsSize, printObjectsSize + 3, 0); // Обнулить размеры векторов
			sendSize = sizeof(int) * 3 + 4;

			if(unitsFrameNum != recvBuffPtr[3])
			{
				sendUnits(sendSize);
			}
			if(pwrPointsFrameNum != recvBuffPtr[4])
			{
				sendPwrPoints(sendSize);
				sendStatus(sendSize);
			}
			sendPreBuffPtr[0] = 4; // тип пакета
			sendPreBuffPtr[1] = unitsFrameNum;
			sendPreBuffPtr[2] = pwrPointsFrameNum;
			sendPreBuffPtr[3] = 0; // резерв
			std::memcpy(&sendPreBuffPtr[4], &printObjectsSize, sizeof(int) * 3);
		}
	}
}


void Game::sendUnits(int &sendSize)
{
	printObjects.clear();
	printObjects.reserve(units.size());

	for(int i = 0; i < units.size(); ++i)
	{
		printObject.skin = units[i].skin;
		printObject.id = units[i].id;
		printObject.x = units[i].x;
		printObject.y = units[i].y;

		printObjects.push_back(printObject);
	}

	printObjectsSize[0] = printObjects.size();

	if(sendSize + printObjects.size() * sizeof(PrintData) > SEND_BUFF_WORK_SIZE)
		std::cout << "SendBuffer overload !\n";

	std::memcpy(&sendPreBuffPtr[sendSize], printObjects.data(), printObjects.size() * sizeof(PrintData));
	sendSize += printObjects.size() * sizeof(PrintData);
}



void Game::sendPwrPoints(int &sendSize)
{
	printObjects.clear();

	for(int i = 0; i < pwrPoints.size(); ++i)
	{
		int sSize = pwrPoints[i].skin.size();

		for(int j = 0; j < sSize; ++j)
		{
			printObject.skin = pwrPoints[i].skin[j];
			printObject.id = 0; // без 0 будет конфлик с units.id
			printObject.x = pwrPoints[i].x;
			printObject.y = pwrPoints[i].y - sSize / 2 + j;

			printObjects.push_back(printObject);
		}
	}

	printObjectsSize[1] = printObjects.size();

	if(sendSize + printObjects.size() * sizeof(PrintData) > SEND_BUFF_WORK_SIZE)
		std::cout << "SendBuffer overload !\n";

	std::memcpy(&sendPreBuffPtr[sendSize], printObjects.data(), printObjects.size() * sizeof(PrintData));
	sendSize += printObjects.size() * sizeof(PrintData);
}



void Game::sendStatus(int &sendSize)
{

	for(int i = 0; i < units.size(); ++i)
	{
		if(units[i].id == recvBuffPtr[1])
		{
			printStatus.pwr = units[i].pwr;
			break;
		}
	}

	if(sendSize + sizeof(PrintStatusData) > SEND_BUFF_WORK_SIZE)
		std::cout << "SendBuffer overload !\n";

	std::memcpy(&sendPreBuffPtr[sendSize], &printStatus, sizeof(PrintStatusData));
	sendSize += sizeof(PrintStatusData);
}



// 	printObjectsSize[2] = printObjects.size();



void Game::sendZero(int &sendSize)
{
	sendPreBuffPtr[0] = 5;
	sendSize = 1;
}



void Game::createPlayer(int &sendSize)
{
	createData.name.resize(recvBuffPtr[1]);
	std::memcpy((void*) createData.name.data(), &recvBuffPtr[3], recvBuffPtr[1]);

	unit.id = clientidBuff;
    unit.skin = recvBuffPtr[2];
    unit.name = createData.name;
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
    unitsFrameNum ++;

	sendPreBuffPtr[0] = 2;
	sendPreBuffPtr[1] = clientidBuff;
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

				// checkPointCollision(i);
				unitsFrameNum ++;
			}
			break;
		}
	}
}




void Game::deletePlayer(int clientid)
{
	for (int i = 0; i < units.size(); ++i)
	{
		if(units[i].id == clientid)
		{
			units.erase(units.begin() + i);
			unitsFrameNum ++;
			break;
		}
	}
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



// void Game::checkPointCollision(int unitIndex)
// {
// 	for (int i = 0; i < pwrPoints.size(); ++i)
// 	{
// 		if(units[unitIndex].x == pwrPoints[i].x && units[unitIndex].y == pwrPoints[i].y && pwrPoints[i].skin == '1')
// 		{
// 			units[unitIndex].pwr ++;
// 			pwrPoints[i].skin = '0';
// 			pwrPointsFrameNum ++;
// 			break;
// 		}
// 	}
// }



void Game::takePWR()
{
	for(int i = 0; i < units.size(); ++i)
	{
		if(units[i].id == recvBuffPtr[1])
		{
			for (int j = 0; j < pwrPoints.size(); ++j)
			{
				if(units[i].x == pwrPoints[j].x && units[i].y == pwrPoints[j].y && pwrPoints[j].pwr > 0)
				{
					units[i].pwr ++;
					pwrPoints[j].pwr --;
					setPwrPointSkin(j);
					pwrPointsFrameNum ++;
					return;
				}
			}
		}
	}
}


void Game::setPwrPointSkin(int id)
{
	pwrPoints[id].skin.clear();

	for (int i = 0; i < pwrPoints[id].pwr / 10 + 1; ++i)
		pwrPoints[id].skin += "(";

	pwrPoints[id].skin += std::to_string(pwrPoints[id].pwr % 10);

	for (int i = 0; i < pwrPoints[id].pwr / 10 + 1; ++i)
	pwrPoints[id].skin += ")";
}

