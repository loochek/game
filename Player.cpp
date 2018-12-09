#include "Player.h"

Player::Player(TiledMap *map) : PhysicObject(map)
{
	position = sf::Vector2f(0, 0);
	shape.setSize(sf::Vector2f(16, 32));
	halfSize = sf::Vector2f(8, 16);
	shape.setOrigin(8, 16);
	speed = sf::Vector2f(0, 0);
	onGround = false;
}

Player::~Player()
{
}

void Player::update(sf::Time delta)
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
	for (int i = 0; i < 4; i++)
		prevInputs[i] = inputs[i];
	shape.setPosition(position);
}

bool Player::keyReleased(sf::Keyboard::Key key)
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

bool Player::keyState(sf::Keyboard::Key key)
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

bool Player::keyPressed(sf::Keyboard::Key key)
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
