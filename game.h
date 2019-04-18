#pragma once

#include <vector>
#include <string>
//#include "server.h"


const int SEND_BUFF_WORK_SIZE = 100000;


struct CreateData
{
	char skin;
	std::string planet;
};

struct PrintData
{
	char skin;
	int id;
	int x;
	int y;
};

struct PrintStatusData
{
	int pwr;
};

struct UnitBox
{
	int id;
	char skin;
	int x;
	int y;
	int pwr;
	std::string planet;
};

struct StarBox
{
	std::string skinMap;
	int x;
	int y;
};

struct PwrPointBox
{
	char skin;
	int x;
	int y;
};


class Game
{
public:
	Game();
	void recvData(char *recvBuff, int clientid);
	void sendData(char *sendBuff, int &sendSize);
	void deletePlayer(int clientid);




private:
	char *recvBuffPtr;
    char *sendPreBuffPtr;
    int clientidBuff;
	int unitsFrameNum;
	int pwrPointsFrameNum;
	int starsFrameNum;
	int printObjectsSize[3];
    CreateData createData;
	std::vector<PrintData> printObjects;
	PrintData printObject;
	std::vector<UnitBox> units;
	UnitBox unit;
	std::vector<PwrPointBox> pwrPoints;
	std::vector<StarBox> stars;
	StarBox star;
	PrintStatusData printStatus;



	void createPlayer(int &sendSize);
	void movePlayer();

	void sendUnits(int &sendSize);
	void sendPwrPoints(int &sendSize);
	void sendStars(int &sendSize);
	void sendStatus(int &sendSize);
	void sendZero(int &sendSize);

	void checkPointCollision(int unitid);
	bool checkObstacle(int checkindex);
	void createStars();
	void createPlanet();

};