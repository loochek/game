#include "PlayState.h"
#include "Game.h"

PlayState::PlayState(Game* game) : BaseState(game)
{
	std::cout << "PlayState()\n";
	map = ResourceManager::getMap("test");
	player = Player(map);
}

PlayState::~PlayState()
{
	std::cout << "~PlayState()\n";
}

void PlayState::update(sf::Time frameTime)
{
	player.update(frameTime);
}

void PlayState::draw()
{
	map->draw(game->window);
	game->window.draw(player.shape);
}