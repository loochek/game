#pragma once

#include "BaseState.h"
#include "ResourceManager.h"
#include "PlayerServer.h"
#include "TiledMap.h"

class ServerState : public BaseState
{
public:
	ServerState(Game *game);
	~ServerState();
	void update(sf::Time frameTime);
	void draw();
	PlayerServer player = 0;
	TiledMap *map;
};