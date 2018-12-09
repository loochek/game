#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "PhysicObject.h"
#include "TiledMap.h"

class Player : PhysicObject
{
public:
	Player(TiledMap *map);
	~Player();

	sf::RectangleShape shape;

	float walkSpeed = 360.0f;
	float jumpSpeed = 610.0f;
	float gravity = 610.0f;

	enum playerState { stay, walk, jump };
	playerState currentState = stay;

	bool inputs[4];
	bool prevInputs[4];

	bool keyReleased(sf::Keyboard::Key key);
	bool keyState(sf::Keyboard::Key key);
	bool keyPressed(sf::Keyboard::Key key);

	void update(sf::Time delta);
};