#pragma once

#include <vector>
//#include "server.h"

struct sData
{
    int id;
    int x;
    int y;
    char skin;
    int command;
};

struct crtData
{
    int id;
    char skin;
};

struct unitBox
{
	int id;
	char skin;
	int x;
	int y;
};

class Game
{
public:
	void recvData(char *buf, int clientid);
	void sendData(char *buf, int &sSize);
	void deletePlayer(int clientid);





private:
	//sData clientData;
	std::vector<sData> serverData;

	crtData createData;
	std::vector<unitBox> units;
	unitBox unit;
	//int sdSize;
	//sData serverData[5];

};