#include "Player.h"

Player::Player()
{
	position = sf::Vector2f(50, 100);
	shape.setSize(sf::Vector2f(10, 20));
	speed = sf::Vector2f(100, -10);
}

Player::~Player()
{
}

void Player::update(sf::Time delta)
{
	speed.y += 20.f * delta.asSeconds();
	position.y += speed.y;
	position.x += speed.x * delta.asSeconds();
	if (position.y > 500)
	{
		position.y = 500;
		speed.y = -10;
	}
	shape.setPosition(position);
	std::cout << position.x << ' ' << position.y << '\n';
}