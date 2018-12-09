#pragma once

#include <SFML/Graphics.hpp>
#include "TiledMap.h"
#include <algorithm>

class PhysicObject
{
public:
	PhysicObject(TiledMap *map);
	~PhysicObject();

	sf::Vector2f oldPosition;
	sf::Vector2f position;
	sf::Vector2f halfSize;

	sf::Vector2f oldSpeed;
	sf::Vector2f speed;

	TiledMap *map;

	void updatePhysics(sf::Time delta);
};