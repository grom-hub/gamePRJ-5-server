#pragma once

#include <vector>
//#include "server.h"



struct crtData
{
    int id;
    char skin;
};

struct printData
{
	char skin;
	int x;
	int y;
};

struct unitBox
{
	int id;
	char skin;
	int x;
	int y;
};

struct pwrPointBox
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
	crtData createData;
	std::vector<unitBox> units;
	unitBox unit;
	std::vector<pwrPointBox> pwrPoints;
	std::vector<printData> printObjects;
	printData printObject;
	int answerType;

	void createPlayer(int &sSize);
	void movePlayer();
	void sendScreen(int &sSize);
};