#include "Bullet.h"

Bullet::Bullet(TiledMap *map, sf::Vector2f position, sf::Vector2f speed) : PhysicObject(map)
{
	setSize(sf::Vector2f(5, 5));
	halfSize = sf::Vector2f(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
	setOrigin(halfSize);
	this->position = position;
	this->speed = speed;
	setPosition(position);
}

Bullet::~Bullet()
{
}

bool Bullet::checkCollisions()
{
	if (speed.x <= 0.0f && hasLeftWall())
	{
		return true;
	}
	if (speed.x >= 0.0f && hasRightWall())
	{
		return true;
	}
	if (speed.y >= 0.0f && hasGround())
	{
		return true;
	}
	if (speed.y <= 0.0f && hasCeiling())
	{
		return true;
	}
	return false;
}

bool Bullet::hasGround()
{
	sf::Vector2f bottomLeft = sf::Vector2f(position.x - halfSize.x + 2.f, position.y + halfSize.y + 2.f);
	sf::Vector2f bottomRight = sf::Vector2f(position.x + halfSize.x - 2.f, position.y + halfSize.y + 2.f);
	for (sf::Vector2f checkedTile = bottomLeft; ; checkedTile.x += 16)
	{
		checkedTile.x = std::min(checkedTile.x, bottomRight.x);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		if (map->isGround(tileIndexX, tileIndexY))
			return true;
		if (checkedTile.x >= bottomRight.x)
			break;
	}
	return false;
}

bool Bullet::hasCeiling()
{
	sf::Vector2f topLeft = sf::Vector2f(position.x - halfSize.x + 2.f, position.y - halfSize.y + 2.f);
	sf::Vector2f topRight = sf::Vector2f(position.x + halfSize.x - 2.f, position.y - halfSize.y + 2.f);
	for (sf::Vector2f checkedTile = topLeft; ; checkedTile.x += 16)
	{
		checkedTile.x = std::min(checkedTile.x, topRight.x);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		if (map->isGround(tileIndexX, tileIndexY))
			return true;
		if (checkedTile.x >= topRight.x)
			break;
	}
	return false;
}

bool Bullet::hasLeftWall()
{
	sf::Vector2f topLeft = sf::Vector2f(position.x - halfSize.x - 2.f, position.y - halfSize.y + 2.f);
	sf::Vector2f bottomLeft = sf::Vector2f(position.x - halfSize.x - 2.f, position.y + halfSize.y - 2.f);
	for (sf::Vector2f checkedTile = topLeft; ; checkedTile.y += 16)
	{
		checkedTile.y = std::min(checkedTile.y, bottomLeft.y);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		if (map->isGround(tileIndexX, tileIndexY))
			return true;
		if (checkedTile.y >= bottomLeft.y)
			break;
	}
	return false;
}

bool Bullet::hasRightWall()
{
	sf::Vector2f topRight = sf::Vector2f(position.x + halfSize.x + 2.f, position.y - halfSize.y + 2.f);
	sf::Vector2f bottomRight = sf::Vector2f(position.x + halfSize.x + 2.f, position.y + halfSize.y - 2.f);
	for (sf::Vector2f checkedTile = topRight; ; checkedTile.y += 16)
	{
		checkedTile.y = std::min(checkedTile.y, bottomRight.y);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		if (map->isGround(tileIndexX, tileIndexY))
			return true;
		if (checkedTile.y >= bottomRight.y)
			break;
	}
	return false;
}

void Bullet::update(sf::Time delta)
{
	if (!dead)
	{
		updatePhysics(delta);
		if (checkCollisions())
			dead = 1;
		setPosition(position);
	}
}