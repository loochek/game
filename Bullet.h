#pragma once

#include "PhysicObject.h"
#include "TiledMap.h"

class Bullet : public PhysicObject, public sf::RectangleShape
{
public:
	Bullet(TiledMap *map, sf::Vector2f position, sf::Vector2f speed);
	~Bullet();
	
	bool checkCollisions();

	bool hasGround();
	bool hasCeiling();
	bool hasLeftWall();
	bool hasRightWall();

	bool dead = false;

	void update(sf::Time delta);
};