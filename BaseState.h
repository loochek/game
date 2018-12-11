#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Game;

class BaseState
{
public:
	BaseState(Game* game);
	virtual ~BaseState();

	virtual void update(sf::Time frameTime) = 0;
	virtual void draw() = 0;

protected:
	Game *game;
};