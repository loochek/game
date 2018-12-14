#pragma once

#include <queue>

#include "BaseState.h"
#include "ResourceManager.h"
#include "Player.h"
#include "TiledMap.h"
#include "Bullet.h"

class PlayState : public BaseState
{
public:
	PlayState(Game *game);
	~PlayState();

	void update(sf::Time frameTime);
	void draw();

	std::deque<Bullet*> bullets;

	sf::View view;

	Player *player;
	TiledMap *map;
};