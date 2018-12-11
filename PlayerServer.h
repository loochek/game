#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "PhysicObject.h"
#include "TiledMap.h"

class PlayerServer : PhysicObject
{
public:
	PlayerServer(TiledMap *map);
	~PlayerServer();

	sf::RectangleShape shape;

	float walkSpeed = 360.0f;
	float jumpSpeed = 610.0f;
	float gravity = 610.0f;

	bool pushedRightWall;
	bool pushesRightWall;

	bool pushedLeftWall;
	bool pushesLeftWall;

	bool wasOnGround;
	bool onGround;

	bool wasAtCeiling;
	bool atCeiling;

	enum playerState { stay, walk, jump };
	playerState currentState = stay;

	bool inputs[4];
	bool prevInputs[4];

	bool keyReleased(sf::Keyboard::Key key);
	bool keyState(sf::Keyboard::Key key);
	bool keyPressed(sf::Keyboard::Key key);

	void update(sf::Time delta);
	void checkCollisions();

	std::pair<bool, float> hasGround();
	std::pair<bool, float> hasCeiling();
	std::pair<bool, float> hasLeftWall();
	std::pair<bool, float> hasRightWall();

	sf::UdpSocket *socket;
};