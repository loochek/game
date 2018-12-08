#pragma once

#include "BaseState.h"
#include "ResourceManager.h"
#include "Player.h"
#include "TiledMap.h"

class PlayState : public BaseState
{
public:
	PlayState(Game *game);

	void update(sf::Time frameTime);
	void draw();
	Player player;
	TiledMap *map;
};