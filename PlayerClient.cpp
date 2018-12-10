#include "PlayerClient.h"

PlayerClient::PlayerClient(TiledMap *map) : PhysicObject(map)
{
	position = sf::Vector2f(0, 0);
	shape.setSize(sf::Vector2f(16, 32));
	halfSize = sf::Vector2f(8, 16);
	shape.setOrigin(8, 16);
	speed = sf::Vector2f(0, 0);
	sock = new sf::UdpSocket();
}

PlayerClient::~PlayerClient()
{
}

void PlayerClient::update(sf::Time delta)
{
	std::size_t received;
	sf::IpAddress sender;
	unsigned short port;
	sock->bind(12345);
	sock->receive(receivedData, 16, received, sender, port);
	//std::cout << "Received " << received << " bytes from " << sender << " on port " << port << std::endl;
	decodeMessage(receivedData, position.x, position.y, speed.x, speed.y);
	std::cout << position.x << ' ' << position.y << ' ' << speed.x << ' ' << speed.y;
	updatePhysics(delta);
	shape.setPosition(position);
}

void PlayerClient::decodeMessage(unsigned char *packet, float &positionX, float &positionY, float &speedX, float &speedY)
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