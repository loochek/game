#include "PlayState.h"
#include "Game.h"

PlayState::PlayState(Game* game) : BaseState(game)
{
	std::cout << "PlayState()\n";
}

PlayState::~PlayState()
{
	std::cout << "~PlayState()\n";
}

void PlayState::update(sf::Time frameTime)
{
}

void PlayState::draw()
{
}