#include "ServerState.h"
#include "Game.h"

ServerState::ServerState(Game* game) : BaseState(game)
{
	std::cout << "ServerState()\n";
	map = ResourceManager::getMap("test");
	player = PlayerServer(map);
}

ServerState::~ServerState()
{
	std::cout << "~ServerState()\n";
}

void ServerState::update(sf::Time frameTime)
{
	player.update(frameTime);
}

void ServerState::draw()
{
	map->draw(game->window);
	game->window.draw(player.shape);
}