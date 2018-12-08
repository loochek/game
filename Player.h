#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Player
{
public:
	Player();
	~Player();

	sf::Vector2f speed;
	sf::Vector2f position;
	sf::RectangleShape shape;

	void update(sf::Time delta);
};