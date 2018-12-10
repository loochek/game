#include "PlayerServer.h"

PlayerServer::PlayerServer(TiledMap *map) : PhysicObject(map)
{
	position = sf::Vector2f(0, 0);
	shape.setSize(sf::Vector2f(16, 32));
	halfSize = sf::Vector2f(8, 16);
	shape.setOrigin(8, 16);
	speed = sf::Vector2f(0, 0);
	onGround = false;
	socket = new sf::UdpSocket();
}

PlayerServer::~PlayerServer()
{
}

void PlayerServer::update(sf::Time delta)
{
	inputs[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	inputs[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	inputs[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	inputs[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	switch (currentState)
	{
	case stay:
	{
		speed = sf::Vector2f(0, 0);
		if (!onGround)
		{
			currentState = jump;
			break;
		}
		if (keyState(sf::Keyboard::A) != keyState(sf::Keyboard::D))
		{
			currentState = walk;
		}
		else if (keyState(sf::Keyboard::W))
		{
			speed.y = -jumpSpeed;
			currentState = jump;
		}
		break;
	}
	case walk:
	{
		if (keyState(sf::Keyboard::A) == keyState(sf::Keyboard::D))
		{
			currentState = stay;
			speed = sf::Vector2f(0, 0);
			break;
		}
		else if (keyState(sf::Keyboard::D))
		{
			speed.x = walkSpeed;
		}
		else if (keyState(sf::Keyboard::A))
		{
			speed.x = -walkSpeed;
		}
		if (keyState(sf::Keyboard::W))
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
		speed.y += gravity * delta.asSeconds();
		if (keyState(sf::Keyboard::A) == keyState(sf::Keyboard::D))
		{
			speed.x = 0;
		}
		else if (keyState(sf::Keyboard::D))
		{
			speed.x = walkSpeed;
		}
		else if (keyState(sf::Keyboard::A))
		{
			speed.x = -walkSpeed;
		}
		if (onGround)
		{
			if (keyState(sf::Keyboard::A) == keyState(sf::Keyboard::D))
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
	oldPosition = position;
	oldSpeed = speed;
	wasOnGround = onGround;
	pushedRightWall = pushesRightWall;
	pushedLeftWall = pushesLeftWall;
	wasAtCeiling = atCeiling;
	unsigned char *packet = new unsigned char[16];
	sf::IpAddress recipient = "192.168.100.7";
	encodeMessage(position.x, position.y, speed.x, speed.y, packet);
	if (socket->send(packet, 16, recipient, 12345) != sf::Socket::Done)
	{
		std::cout << 'E';
	}
	for (int i = 0; i < 4; i++)
		prevInputs[i] = inputs[i];
	shape.setPosition(position);
}

void PlayerServer::checkCollisions()
{
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

bool PlayerServer::keyReleased(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::W)
		return !inputs[0] && prevInputs[0];
	else if (key == sf::Keyboard::A)
		return !inputs[1] && prevInputs[1];
	else if (key == sf::Keyboard::S)
		return !inputs[2] && prevInputs[2];
	else if (key == sf::Keyboard::D)
		return !inputs[3] && prevInputs[3];
}

bool PlayerServer::keyState(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::W)
		return inputs[0];
	else if (key == sf::Keyboard::A)
		return inputs[1];
	else if (key == sf::Keyboard::S)
		return inputs[2];
	else if (key == sf::Keyboard::D)
		return inputs[3];
}

bool PlayerServer::keyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::W)
		return inputs[0] && !prevInputs[0];
	else if (key == sf::Keyboard::A)
		return inputs[1] && !prevInputs[1];
	else if (key == sf::Keyboard::S)
		return inputs[2] && !prevInputs[2];
	else if (key == sf::Keyboard::D)
		return inputs[3] && !prevInputs[3];
}

std::pair<bool, float> PlayerServer::hasGround()
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

std::pair<bool, float> PlayerServer::hasCeiling()
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

std::pair<bool, float> PlayerServer::hasLeftWall()
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

std::pair<bool, float> PlayerServer::hasRightWall()
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
void PlayerServer::encodeMessage(float positionX, float positionY, float speedX, float speedY, unsigned char *packet)
{
	unsigned int asInt = *((int*)&positionX);
	packet[0] = asInt >> 24;
	packet[1] = (asInt >> 16) & 255;
	packet[2] = (asInt >> 8) & 255;
	packet[3] = asInt & 255;
	asInt = *((int*)&positionY);
	packet[4] = asInt >> 24;
	packet[5] = (asInt >> 16) & 255;
	packet[6] = (asInt >> 8) & 255;
	packet[7] = asInt & 255;
	asInt = *((int*)&speedX);
	packet[8] = asInt >> 24;
	packet[9] = (asInt >> 16) & 255;
	packet[10] = (asInt >> 8) & 255;
	packet[11] = asInt & 255;
	asInt = *((int*)&speedY);
	packet[12] = asInt >> 24;
	packet[13] = (asInt >> 16) & 255;
	packet[14] = (asInt >> 8) & 255;
	packet[15] = asInt & 255;
}

void decodeMessage(unsigned char *packet, float &positionX, float &positionY, float &speedX, float &speedY)
{
	unsigned int asInt = 0;
	asInt |= (packet[0] << 24);
	asInt |= (packet[1] << 16);
	asInt |= (packet[2] << 8);
	asInt |= (packet[3]);
	positionX = *((float*)&asInt);
	asInt = 0;
	asInt |= (packet[4] << 24);
	asInt |= (packet[5] << 16);
	asInt |= (packet[6] << 8);
	asInt |= (packet[7]);
	positionY = *((float*)&asInt);
	asInt = 0;
	asInt |= (packet[8] << 24);
	asInt |= (packet[9] << 16);
	asInt |= (packet[10] << 8);
	asInt |= (packet[11]);
	speedX = *((float*)&asInt);
	asInt = 0;
	asInt |= (packet[12] << 24);
	asInt |= (packet[13] << 16);
	asInt |= (packet[14] << 8);
	asInt |= (packet[15]);
	speedY = *((float*)&asInt);
	return;
}