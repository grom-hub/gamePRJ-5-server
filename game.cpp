#include "game.h"




Game::Game()
{
	uX = 5;
	uY = 5;
}

// Game::~Game()
// {

// }


void Game::addCommand(int command)
{
	if (command == 1) uX++;
	if (command == 2) uX--;
	if (command == 3) uY++;
	if (command == 4) uY--;

}

int Game::getX()
{
	return uX;
}

int Game::getY()
{
	return uY;
}

