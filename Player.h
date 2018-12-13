#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "PhysicObject.h"
#include "TiledMap.h"
#include "AnimatedSprite.h"
#include "ResourceManager.h"

class Player : public PhysicObject, public AnimatedSprite
{
public:
	Player(TiledMap *map);
	Player(TiledMap *map, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key jumpKey);
	~Player();

	float walkSpeed = 100.0f;
	float jumpSpeed = 170.0f;
	float gravity = 100.0f;

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

	bool orientation = 0; // 0-right, 1-left

	bool inputs[3];
	bool prevInputs[3];

	bool keyReleased(sf::Keyboard::Key key);
	bool keyState(sf::Keyboard::Key key);
	bool keyPressed(sf::Keyboard::Key key);

	sf::Keyboard::Key leftKey;
	sf::Keyboard::Key rightKey;
	sf::Keyboard::Key jumpKey;

	void update(sf::Time delta);
	void checkCollisions();

	std::pair<bool, float> hasGround();
	std::pair<bool, float> hasCeiling();
	std::pair<bool, float> hasLeftWall();
	std::pair<bool, float> hasRightWall();
};