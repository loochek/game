#pragma once

#include "BaseState.h"
#include "ResourceManager.h"
#include "PlayerClient.h"
#include "TiledMap.h"

class PlayState : public BaseState
{
public:
	PlayState(Game *game);
	~PlayState();
	void update(sf::Time frameTime);
	void draw();
	PlayerClient player = 0;
	TiledMap *map;
};