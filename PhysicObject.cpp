#include "PhysicObject.h"

PhysicObject::PhysicObject(TiledMap *map)
{
	this->map = map;
}

PhysicObject::~PhysicObject()
{
}

void PhysicObject::updatePhysics(sf::Time delta)
{
	oldPosition = position;
	oldSpeed = speed;
	wasOnGround = onGround;
	pushedRightWall = pushesRightWall;
	pushedLeftWall = pushesLeftWall;
	wasAtCeiling = atCeiling;
	position += speed * delta.asSeconds();
	auto check = hasGround();
	if (speed.y >= 0.0f && check.first)
	{
		position.y = check.second - halfSize.y;
		speed.y = 0.0f;
		onGround = true;
	}
	else
		onGround = false;
}

std::pair<bool, float> PhysicObject::hasGround()
{
	sf::Vector2f bottomLeft = sf::Vector2f(position.x - halfSize.x + 2.f, position.y + halfSize.y + 2.f);
	sf::Vector2f bottomRight = sf::Vector2f(position.x + halfSize.x - 2.f, position.y + halfSize.y + 2.f);
	for (sf::Vector2f checkedTile = bottomLeft; ; checkedTile.x += 16)
	{
		checkedTile.x = std::min(checkedTile.x, bottomRight.x);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		float groundY = tileIndexY * 16;
		if (map->isGround(tileIndexX, tileIndexY))
			return { true, groundY };
		if (checkedTile.x >= bottomRight.x)
			break;
	}
	return { false, -1 };
}
