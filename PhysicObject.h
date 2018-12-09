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

	bool pushedRightWall;
	bool pushesRightWall;

	bool pushedLeftWall;
	bool pushesLeftWall;

	bool wasOnGround;
	bool onGround;

	bool wasAtCeiling;
	bool atCeiling;

	TiledMap *map;

	void updatePhysics(sf::Time delta);
	std::pair<bool, float> hasGround();
};