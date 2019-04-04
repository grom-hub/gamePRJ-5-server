#pragma once

#include <vector>
//#include "server.h"



struct PrintObjectData
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
};

struct StarBox
{
	int id;
	char skin[7];
	int x;
	int y[7];
	int pwr;
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
    char *sendBuffPtr;
    int clientidBuff;
	std::vector<UnitBox> units;
	UnitBox unit;
	std::vector<PrintObjectData> printObjects;
	PrintObjectData printObject;
	std::vector<PwrPointBox> pwrPoints;
	std::vector<StarBox> stars;
	int answerType;
	PrintStatusData printStatus;
	int serverFrameNum;


	void createPlayer(int &sendSize);
	void movePlayer();
	void sendScreen(int &sendSize);
	void addStatus(int &sendSize);
	void sendZero(int &sendSize);
	void checkPointCollision(int unitid);
	void createStars();
	bool checkObstacle(int checkindex);

};