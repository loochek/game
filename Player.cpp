#include "Player.h"

Player::Player(TiledMap *map, AABB aabb) : PhysicObject(map, aabb)
{
	scale(-1, 1);
	setAnimation(*ResourceManager::getAnimation("playerWalk"));
	position = sf::Vector2f(17, 17);
	setFrameTime(sf::seconds(0.05));
	setOrigin(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
	speed = sf::Vector2f(0, 0);
	onGround = false;
	leftKey = sf::Keyboard::A;
	rightKey = sf::Keyboard::D;
	jumpKey = sf::Keyboard::W;
}

Player::Player(TiledMap * map, sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key jump) : PhysicObject(map, aabb)
{
	scale(-1, 1);
	setAnimation(*ResourceManager::getAnimation("playerWalk"));
	position = sf::Vector2f(17, 17);
	setFrameTime(sf::seconds(0.05));
	setOrigin(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
	speed = sf::Vector2f(0, 0);
	onGround = false;
	leftKey = left;
	rightKey = right;
	jumpKey = jump;
}

Player::~Player()
{
}

void Player::update(sf::Time delta)
{
	oldPosition = position;
	oldSpeed = speed;
	wasOnGround = onGround;
	pushedRightWall = pushesRightWall;
	pushedLeftWall = pushesLeftWall;
	wasAtCeiling = atCeiling;
	inputs[0] = sf::Keyboard::isKeyPressed(jumpKey);
	inputs[1] = sf::Keyboard::isKeyPressed(leftKey);
	inputs[2] = sf::Keyboard::isKeyPressed(rightKey);
	switch (currentState)
	{
	case stay:
	{
		play(*ResourceManager::getAnimation("playerStay"));
		speed = sf::Vector2f(0, 0);
		if (!onGround)
		{
			currentState = jump;
			break;
		}
		if (keyState(leftKey) != keyState(rightKey))
		{
			if (keyState(leftKey))
			{
				if (!orientation)
					scale(-1, 1);
				orientation = 1;
			}
			else if (keyState(rightKey))
			{
				if (orientation)
					scale(-1, 1);
				orientation = 0;
			}	
			currentState = walk;
		}
		else if (keyState(jumpKey))
		{
			speed.y = -jumpSpeed;
			currentState = jump;
		}
		break;
	}
	case walk:
	{
		play(*ResourceManager::getAnimation("playerWalk"));
		if (keyState(leftKey) == keyState(rightKey))
		{
			currentState = stay;
			speed = sf::Vector2f(0, 0);
			break;
		}
		else if (keyState(rightKey))
		{
			if (orientation)
				scale(-1, 1);
			orientation = 0;
			speed.x = walkSpeed;
		}
		else if (keyState(leftKey))
		{
			if (!orientation)
				scale(-1, 1);
			orientation = 1;
			speed.x = -walkSpeed;
		}
		if (keyState(jumpKey))
		{
			speed.y = -jumpSpeed;
			currentState = jump;
			break;
		}
		else if (!onGround)
		{
			currentState = jump;
			break;
		}
		break;
	}
	case jump:
	{
		play(*ResourceManager::getAnimation("playerJump"));
		speed.y += gravity * delta.asSeconds();
		if (keyState(leftKey) == keyState(rightKey))
		{
			speed.x = 0;
		}
		else if (keyState(rightKey))
		{
			if (orientation)
				scale(-1, 1);
			orientation = 0;
			speed.x = walkSpeed;
		}
		else if (keyState(leftKey))
		{
			if (!orientation)
				scale(-1, 1);
			orientation = 1;
			speed.x = -walkSpeed;
		}
		if (onGround)
		{
			if (keyState(leftKey) == keyState(rightKey))
			{
				currentState = stay;
				speed = sf::Vector2f(0, 0);
			}
			else
			{
				currentState = walk;
				speed.y = 0;
			}
		}
		break;
	}
	};
	updatePhysics(delta);
	checkCollisions();
	for (int i = 0; i < 3; i++)
		prevInputs[i] = inputs[i];
	setPosition(position);
	updateAnimation(delta);
}

void Player::checkCollisions()
{
	auto check3 = hasLeftWall();
	if (speed.x <= 0.0f && check3.first)
	{
		if (oldPosition.x - aabb.halfSize.x + aabb.offset.x >= check3.second)
		{
			position.x = check3.second + aabb.halfSize.x - aabb.offset.x;
			pushesLeftWall = true;
		}
		speed.x = std::max(speed.x, 0.0f);
	}
	auto check4 = hasRightWall();
	if (speed.x >= 0.0f && check4.first)
	{
		if (oldPosition.x + aabb.halfSize.x + aabb.offset.x <= check4.second)
		{
			position.x = check4.second - aabb.halfSize.x - aabb.offset.x;
			pushesRightWall = true;
		}
		speed.x = std::min(speed.x, 0.0f);
	}
	auto check = hasGround();
	if (speed.y >= 0.0f && check.first)
	{
		position.y = check.second - aabb.halfSize.y - aabb.offset.y;
		speed.y = 0.0f;
		onGround = true;
	}
	else
		onGround = false;
	auto check2 = hasCeiling();
	if (speed.y <= 0.0f && check2.first)
	{
		position.y = check2.second + aabb.halfSize.y - aabb.offset.y;
		speed.y = 0.0f;
		atCeiling = true;
	}
	else
		atCeiling = false;
}

bool Player::keyReleased(sf::Keyboard::Key key)
{
	if (key == jumpKey)
		return !inputs[0] && prevInputs[0];
	else if (key == leftKey)
		return !inputs[1] && prevInputs[1];
	else if (key == rightKey)
		return !inputs[2] && prevInputs[2];
	else
		return 0;
}

bool Player::keyState(sf::Keyboard::Key key)
{
	if (key == jumpKey)
		return inputs[0];
	else if (key == leftKey)
		return inputs[1];
	else if (key == rightKey)
		return inputs[2];
	else
		return 0;
}

bool Player::keyPressed(sf::Keyboard::Key key)
{
	if (key == jumpKey)
		return inputs[0] && !prevInputs[0];
	else if (key == leftKey)
		return inputs[1] && !prevInputs[1];
	else if (key == rightKey)
		return inputs[2] && !prevInputs[2];
	else
		return 0;
}

std::pair<bool, float> Player::hasGround()
{
	sf::Vector2f center = position + aabb.offset;
	sf::Vector2f bottomLeft = sf::Vector2f(center.x - aabb.halfSize.x + 2.f, center.y + aabb.halfSize.y + 2.f);
	sf::Vector2f bottomRight = sf::Vector2f(center.x + aabb.halfSize.x - 2.f, center.y + aabb.halfSize.y + 2.f);
	for (sf::Vector2f checkedTile = bottomLeft; ; checkedTile.x += 16)
	{
		checkedTile.x = std::min(checkedTile.x, bottomRight.x);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		float groundY = tileIndexY * 16.f;
		if (map->isGround(tileIndexX, tileIndexY))
			return { true, groundY };
		if (checkedTile.x >= bottomRight.x)
			break;
	}
	return { false, -1.f };
}

std::pair<bool, float> Player::hasCeiling()
{
	sf::Vector2f center = position + aabb.offset;
	sf::Vector2f topLeft = sf::Vector2f(center.x - aabb.halfSize.x + 2.f, center.y - aabb.halfSize.y + 2.f);
	sf::Vector2f topRight = sf::Vector2f(center.x + aabb.halfSize.x - 2.f, center.y - aabb.halfSize.y + 2.f);
	for (sf::Vector2f checkedTile = topLeft; ; checkedTile.x += 16)
	{
		checkedTile.x = std::min(checkedTile.x, topRight.x);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		float ceilingY = tileIndexY * 16.f + 16.f;
		if (map->isGround(tileIndexX, tileIndexY))
			return { true, ceilingY };
		if (checkedTile.x >= topRight.x)
			break;
	}
	return { false, -1.f };
}

std::pair<bool, float> Player::hasLeftWall()
{
	sf::Vector2f center = position + aabb.offset;
	sf::Vector2f topLeft = sf::Vector2f(center.x - aabb.halfSize.x - 2.f, center.y - aabb.halfSize.y + 2.f);
	sf::Vector2f bottomLeft = sf::Vector2f(center.y - aabb.halfSize.y - 2.f, center.y + aabb.halfSize.y - 2.f);
	for (sf::Vector2f checkedTile = topLeft; ; checkedTile.y += 16)
	{
		checkedTile.y = std::min(checkedTile.y, bottomLeft.y);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		float wallX = tileIndexX * 16.f + 16.f;
		if (map->isGround(tileIndexX, tileIndexY))
			return { true, wallX };
		if (checkedTile.y >= bottomLeft.y)
			break;
	}
	return { false, -1.f };
}

std::pair<bool, float> Player::hasRightWall()
{
	sf::Vector2f center = position + aabb.offset;
	sf::Vector2f topRight = sf::Vector2f(center.x + aabb.halfSize.x + 2.f, center.y - aabb.halfSize.y + 2.f);
	sf::Vector2f bottomRight = sf::Vector2f(center.x + aabb.halfSize.x + 2.f, center.y + aabb.halfSize.y - 2.f);
	for (sf::Vector2f checkedTile = topRight; ; checkedTile.y += 16)
	{
		checkedTile.y = std::min(checkedTile.y, bottomRight.y);
		int tileIndexX = map->getMapTileXAtPoint(checkedTile.x);
		int tileIndexY = map->getMapTileYAtPoint(checkedTile.y);
		float wallX = tileIndexX * 16.f;
		if (map->isGround(tileIndexX, tileIndexY))
			return { true, wallX };
		if (checkedTile.y >= bottomRight.y)
			break;
	}
	return { false, -1.f };
}