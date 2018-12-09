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
	auto check3 = hasLeftWall();
	if (speed.x <= 0.0f && check3.first) 
	{
		if (oldPosition.x - halfSize.x >= check3.second)
		{
			position.x = check3.second + halfSize.x;
			pushesLeftWall = true;
		}
		speed.x = std::max(speed.x, 0.0f);
	}
	auto check4 = hasRightWall();
	if (speed.x >= 0.0f && check4.first)
	{
		if (oldPosition.x + halfSize.x <= check4.second)
		{
			position.x = check4.second - halfSize.x;
			pushesRightWall = true;
		}
		speed.x = std::min(speed.x, 0.0f);
	}
	auto check = hasGround();
	if (speed.y >= 0.0f && check.first)
	{
		position.y = check.second - halfSize.y;
		speed.y = 0.0f;
		onGround = true;
	}
	else
		onGround = false;
	auto check2 = hasCeiling();
	if (speed.y <= 0.0f && check2.first)
	{
		position.y = check2.second + halfSize.y;
		speed.y = 0.0f;
		atCeiling = true;
	}
	else
		atCeiling = false;
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

std::pair<bool, float> PhysicObject::hasCeiling()
{
	sf::Vector2f topLeft = sf::Vector2f(position.x - halfSize.x + 2.f, position.y - halfSize.y + 2.f);
	sf::Vector2f topRight = sf::Vector2f(position.x + halfSize.x - 2.f, position.y - halfSize.y + 2.f);
	for (sf::Vector2f checkedTile = topLeft; ; checkedTile.x += 16)
	{
		checkedTile.x = std::min(checkedTile.x, topRight.x);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		float ceilingY = tileIndexY * 16 + 16;
		if (map->isGround(tileIndexX, tileIndexY))
			return { true, ceilingY };
		if (checkedTile.x >= topRight.x)
			break;
	}
	return { false, -1 };
}

std::pair<bool, float> PhysicObject::hasLeftWall()
{
	sf::Vector2f topLeft = sf::Vector2f(position.x - halfSize.x - 2.f, position.y - halfSize.y + 2.f);
	sf::Vector2f bottomLeft = sf::Vector2f(position.x - halfSize.x - 2.f, position.y + halfSize.y + 2.f);
	for (sf::Vector2f checkedTile = topLeft; ; checkedTile.y += 16)
	{
		checkedTile.y = std::min(checkedTile.y, topLeft.y);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		float wallX = tileIndexX * 16 + 16;
		if (map->isGround(tileIndexX, tileIndexY))
			return { true, wallX };
		if (checkedTile.y >= topLeft.y)
			break;
	}
	return { false, -1 };
}

std::pair<bool, float> PhysicObject::hasRightWall()
{
	sf::Vector2f topRight = sf::Vector2f(position.x + halfSize.x + 2.f, position.y - halfSize.y + 2.f);
	sf::Vector2f bottomRight = sf::Vector2f(position.x + halfSize.x + 2.f, position.y + halfSize.y + 2.f);
	for (sf::Vector2f checkedTile = topRight; ; checkedTile.y += 16)
	{
		checkedTile.y = std::min(checkedTile.y, topRight.y);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		float wallX = tileIndexX * 16;
		if (map->isGround(tileIndexX, tileIndexY))
			return { true, wallX };
		if (checkedTile.y >= topRight.y)
			break;
	}
	return { false, -1 };
}