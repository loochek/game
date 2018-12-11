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
	sf::IpAddress sender;
	unsigned short port;
	sock->bind(12345);
	sf::Packet packet;
	sock->receive(packet, sender, port);
	packet >> position.x >> position.y >> speed.x >> speed.y;
	std::cout << position.x << ' ' << position.y << ' ' << speed.x << ' ' << speed.y;
	updatePhysics(delta);
	shape.setPosition(position);
}