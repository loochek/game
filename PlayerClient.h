#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "PhysicObject.h"
#include "TiledMap.h"

class PlayerClient : PhysicObject
{
public:
	PlayerClient(TiledMap *map);
	~PlayerClient();

	sf::RectangleShape shape;

	void update(sf::Time delta);

	void decodeMessage(unsigned char *packet, float &positionX, float &positionY, float &speedX, float &speedY);

	sf::UdpSocket *sock;
	unsigned char receivedData[16];
};