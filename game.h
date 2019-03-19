#pragma once

//#include "server.h"

struct sData
{
    int id;
    int uX;
    int uY;
    char uSkin;
    int command;
};

class Game
{
public:
	void initGame();
	void recvData(char *buf);
	void sendData(char *buf);





private:
	sData clientData;
    sData serverData[5];

};