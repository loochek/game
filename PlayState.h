#pragma once

#include "BaseState.h"
#include "ResourceManager.h"

class PlayState : public BaseState
{
public:
	PlayState(Game *game);
	~PlayState();

	void update(sf::Time frameTime);
	void draw();
};