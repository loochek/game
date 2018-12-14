#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>

#include "TiledMap.h"

struct AABB
{
	sf::Vector2f halfSize;
	sf::Vector2f offset;
	AABB(sf::Vector2f halfSize, sf::Vector2f offset)
	{
		this->halfSize = halfSize;
		this->offset = offset;
	}
	AABB()
	{
		this->halfSize = sf::Vector2f(0, 0);
		this->offset = sf::Vector2f(0, 0);
	}
};

class PhysicObject
{
public:
	PhysicObject(TiledMap *map, AABB aabb);
	~PhysicObject();

	sf::Vector2f oldPosition;
	sf::Vector2f position;

	sf::Vector2f oldSpeed;
	sf::Vector2f speed;

	AABB aabb;

	TiledMap *map;

	void updatePhysics(sf::Time delta);
};