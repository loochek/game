#include "BaseState.h"
#include "Game.h"

BaseState::BaseState(Game* game)
{
	this->game = game;
	std::cout << "BaseState()\n";
}

BaseState::~BaseState()
{
	std::cout << "~BaseState()\n";
}