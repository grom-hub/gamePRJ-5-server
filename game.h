#pragma once

#include <vector>
//#include "server.h"



struct PrintObjectData
{
	char skin;
	int x;
	int y;
};

struct PrintStatusData
{
	int serverFrameNum;
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
	std::vector<PwrPointBox> pwrPoints;
	std::vector<PrintObjectData> printObjects;
	PrintObjectData printObject;
	int answerType;
	PrintStatusData printStatus;
	int serverFrameNum;


	void createPlayer(int &sendSize);
	void movePlayer();
	void sendScreen(int &sendSize);
	void checkPointCollision(int unitid);
	void addStatus(int &sendSize);
	void sendZero(int &sendSize);

};