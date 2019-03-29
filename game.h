#pragma once

#include <vector>
//#include "server.h"



struct CrtData
{
    int id;
    char skin;
};

struct PrintData
{
	char skin;
	int x;
	int y;
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
	void recvData(char *recvBuff, int clientid);
	void sendData(char *sendBuff, int &sSize);
	void deletePlayer(int clientid);

	void createPwrPoints();




private:
	char *recvBuffPtr;
    char *sendBuffPtr;
    int clientidBuff;
	CrtData createData;
	std::vector<UnitBox> units;
	UnitBox unit;
	std::vector<PwrPointBox> pwrPoints;
	std::vector<PrintData> printObjects;
	PrintData printObject;
	int answerType;
	bool toggle;

	void createPlayer(int &sSize);
	void movePlayer();
	void sendScreen(int &sSize);
	void checkPointCollision(int unitid);
};