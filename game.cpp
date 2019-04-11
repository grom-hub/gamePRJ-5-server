//#include <vector>
#include <cstring> // std::memcpy()
#include <iostream>
#include <algorithm> // std::fill()


#include "game.h"



Game::Game()
{

	unitsFrameNum = 1;
	pwrPointsFrameNum = 1;
	starsFrameNum = 1;

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

//-------------------------------------------
	if(unitsFrameNum > 100) // цикл счетчика
		unitsFrameNum = 1;
	if(pwrPointsFrameNum > 100)
		pwrPointsFrameNum = 1;
	if(starsFrameNum > 100)
		starsFrameNum = 1;

	if(unitsFrameNum == 100) // временная зарядка pwrPoints
		for(int i = 0; i < pwrPoints.size(); ++i)
		{
			pwrPoints[i].skin = '1';
			pwrPointsFrameNum ++;
		}
//-------------------------------------------

	if (recvBuffPtr[0] == 3) // Стандартный режим
	{
		if(recvBuffPtr[2] != 0) // Обработка команды управления персонажем
			if(recvBuffPtr[2] == 5)
				createPlanet();
			else
				movePlayer();
	}
}



void Game::sendData(char *sendBuff, int &sendSize)
{
	sendBuffPtr = sendBuff;

	if (recvBuffPtr[0] == 2) // создать игрока
		createPlayer(sendSize);


	if(recvBuffPtr[0] == 3)
	{
		std::fill(vectorSize, vectorSize + 3, 0); // Обнулить размеры векторов

		sendSize = sizeof(int) * 3 + 4;
		bool upToDate = true;
		if(unitsFrameNum != recvBuffPtr[3])
		{
			upToDate = false;
			sendUnits(sendSize);
		}
		if(pwrPointsFrameNum != recvBuffPtr[4])
		{
			upToDate = false;
			sendPwrPoints(sendSize);
			sendStatus(sendSize);
		}
		if(starsFrameNum != recvBuffPtr[5])
		{
			upToDate = false;
			sendStars(sendSize);
		}
		if(!upToDate)
		{
			sendBuffPtr[0] = 4; // тип пакета
			sendBuffPtr[1] = unitsFrameNum;
			sendBuffPtr[2] = pwrPointsFrameNum;
			sendBuffPtr[3] = starsFrameNum;
			std::memcpy(&sendBuffPtr[4], &vectorSize, sizeof(int) * 3);
		}
		if(upToDate)
			sendZero(sendSize);
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

	vectorSize[0] = printObjects.size();

	std::memcpy(&sendBuffPtr[sendSize], printObjects.data(), printObjects.size() * sizeof(PrintData));
	sendSize += printObjects.size() * sizeof(PrintData);
}



void Game::sendPwrPoints(int &sendSize)
{
	printObjects.clear();
	printObjects.reserve(pwrPoints.size());

	for(int i = 0; i < pwrPoints.size(); ++i)
	{
		printObject.skin = pwrPoints[i].skin;
		printObject.id = 0; // без 0 будет конфлик с units.id
		printObject.x = pwrPoints[i].x;
		printObject.y = pwrPoints[i].y;

		printObjects.push_back(printObject);
	}

	vectorSize[1] = printObjects.size();

	std::memcpy(&sendBuffPtr[sendSize], printObjects.data(), printObjects.size() * sizeof(PrintData));
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

	std::memcpy(&sendBuffPtr[sendSize], &printStatus, sizeof(PrintStatusData));
	sendSize += sizeof(PrintStatusData);
}



void Game::sendStars(int &sendSize)
{
	printObjects.clear();

	for(int i = 0; i < stars.size(); ++i)
	{
		for(int j = 0; j < stars[i].skinMap.size(); ++j)
		{
			printObject.skin = stars[i].skinMap[j];
			printObject.id = 0; // без 0 будет конфлик с units.id
			printObject.x = stars[i].x;
			printObject.y = stars[i].y + j;

			printObjects.push_back(printObject);
		}
	}

	vectorSize[2] = printObjects.size();

	std::memcpy(&sendBuffPtr[sendSize], printObjects.data(), printObjects.size() * sizeof(PrintData));
	sendSize += printObjects.size() * sizeof(PrintData);
}



void Game::sendZero(int &sendSize)
{
	sendBuffPtr[0] = 5;
	sendSize = 1;
}



void Game::createPlayer(int &sendSize)
{
	createData.planet.resize(recvBuffPtr[1]);
	std::memcpy((void*) createData.planet.data(), &recvBuffPtr[3], recvBuffPtr[1]);

	unit.id = clientidBuff;
    unit.skin = recvBuffPtr[2];
    unit.planet = createData.planet;
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

				checkPointCollision(i);
				unitsFrameNum ++;
			}
			break;
		}
	}
}



void Game::createPlanet()
{
	for(int i = 0; i < units.size(); ++i)
	{
		if(units[i].id == recvBuffPtr[1])
		{
			star.skinMap = units[i].planet;
		    star.x = units[i].x;
		    star.y = units[i].y + 1;

		    stars.push_back(star);
		    starsFrameNum ++;
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



void Game::checkPointCollision(int unitIndex)
{
	for (int i = 0; i < pwrPoints.size(); ++i)
	{
		if(units[unitIndex].x == pwrPoints[i].x && units[unitIndex].y == pwrPoints[i].y && pwrPoints[i].skin == '1')
		{
			units[unitIndex].pwr ++;
			pwrPoints[i].skin = '0';
			pwrPointsFrameNum ++;
			break;
		}
	}
}



void Game::createStars()
{
	stars.clear();

	star.skinMap = "(*)";
    star.x = 0;
    star.y = 15;

    stars.push_back(star);

    star.skinMap = "((*))";
    star.x = 8;
    star.y = 85;

	stars.push_back(star);

    star.skinMap = "(((*)))";
    star.x = -10;
    star.y = 45;

    stars.push_back(star);

    star.skinMap = "(-^-)";
    star.x = 7;
    star.y = 35;

    stars.push_back(star);
}















// if (answerType == 4) // отправить картинку и статус
// { 
// 	sendScreen(sendSize);
// 	addStatus(sendSize);
// }




// void Game::updatePrintData()
// {
// 	printObjects.clear();
// 	printObjects.reserve(units.size() + pwrPoints.size() + stars.size() * 7);

// 	for(int i = 0; i < stars.size(); ++i)
// 	{
// 		for(int j = 0; j < stars[i].skinMap.size(); ++j)
// 		{
// 			printObject.skin = stars[i].skinMap[j];
// 			printObject.id = 0; // без 0 будет конфлик с units.id
// 			printObject.x = stars[i].x;
// 			printObject.y = stars[i].y + j;

// 			printObjects.push_back(printObject);
// 		}
// 	}
	
// 	for (int i = 0; i < pwrPoints.size(); ++i)
// 	{
// 		printObject.skin = pwrPoints[i].skin;
// 		printObject.id = 0; // без 0 будет конфлик с units.id
// 		printObject.x = pwrPoints[i].x;
// 		printObject.y = pwrPoints[i].y;
		
// 		printObjects.push_back(printObject);
// 	}

// 	for (int i = 0; i < units.size(); ++i)
// 	{
// 		printObject.skin = units[i].skin;
// 		printObject.id = units[i].id;
// 		printObject.x = units[i].x;
// 		printObject.y = units[i].y;

// 		printObjects.push_back(printObject);
// 	}
// }






// void Game::sendScreen(int &sendSize)
// {
// 	updatePrintData();


// 	for(int i = 0; i < printObjects.size(); ++i)
// 	{
// 		for(int j = 0; clientObjectsUid.size(); ++j)
// 		{
// 			if(clientObjectsUid == printObjects[i].uid)
// 				break;
// 			else
// 				// отправить клиенту
// 		}
// 	}



// 	sendSize = printObjects.size() * sizeof(PrintObjectData);
// 	sendBuffPtr[0] = 4; // тип пакетиа
// 	sendBuffPtr[1] = printObjects.size();
// 	std::memcpy(&sendBuffPtr[3], printObjects.data(), sendSize);
// 	std::cout << printObjects.size() << std::endl;
// }



// void Game::addStatus(int &sendSize)
// {

// 	for(int i = 0; i < units.size(); ++i)
// 	{
// 		if(units[i].id == recvBuffPtr[1])
// 		{
// 			printStatus.pwr = units[i].pwr;
// 			break;
// 		}
// 	}

// 	sendBuffPtr[2] = serverFrameNum;


// 	std::memcpy(&sendBuffPtr[sendSize + 3], &printStatus, sizeof(PrintStatusData));
// 	sendSize += sizeof(PrintStatusData) + 3;

// }







// void Game::updatePrintData()
// {

// 	for(int i = 0; i < stars.size(); ++i)
// 	{
// 		if(stars[i].changed == true)
// 		{
// 			for(int j = 0; j < stars[i].skinMap.size(); ++j)
// 			{
// 				printObject.skin = stars[i].skinMap[j];
// 				printObject.id = 0; // без 0 будет конфлик с units.id
// 				printObject.x = stars[i].x;
// 				printObject.y = stars[i].y + j;

// 				printObjects.push_back(printObject);
// 			}
// 			stars[i].changed = false;
// 		}
// 	}


// 	for (int i = 0; i < pwrPoints.size(); ++i)
// 	{
// 		if(pwrPoints[i].changed == true)
// 		{
// 			for(int j = 0; j < printObjects.size(); ++j)
// 			{
// 				if(pwrPoints[i].uid == printObjects[j].uid)
// 				{
// 					printObjects[j].skin = pwrPoints[i].skin;
// 					break;
// 				}
// 				else
// 				{
// 					printObject.skin = pwrPoints[i].skin;
// 					printObject.id = 0; // без 0 будет конфлик с units.id
// 					printObject.x = pwrPoints[i].x;
// 					printObject.y = pwrPoints[i].y;
					
// 					printObjects.push_back(printObject);
// 				}
// 			}
// 			pwrPoints[i].changed = false;
// 		}
// 	}


// 	for (int i = 0; i < units.size(); ++i)
// 	{
// 		if(units[i].changed == true)
// 		{
// 			for(int j = 0; j < printObjects.size(); ++j)
// 			{
// 				if(units[i].uid == printObjects[j].uid)
// 				{
// 					printObjects[j].x = units[i].x;
// 					printObjects[j].y = units[i].y;
// 					break;
// 				}
// 				else
// 				{
// 					printObject.skin = units[i].skin;
// 					printObject.id = units[i].id;
// 					printObject.x = units[i].x;
// 					printObject.y = units[i].y;

// 					printObjects.push_back(printObject);
// 				}
// 			}
// 			units[i].changed = false;
// 		}
// 	}
// }