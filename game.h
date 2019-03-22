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

class Game
{
public:
	void recvData(char *buf);
	void sendData(char *buf);





private:
	sData clientData;
	std::vector<sData> serverData;
	//int sdSize;
	//sData serverData[5];

};