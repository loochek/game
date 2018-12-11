#pragma once

#include <stack>
#include <SFML\Graphics.hpp>

#include "PlayState.h"
#include "BaseState.h"
#include "ResourceManager.h"

class Game
{
public:
	Game();
	~Game();

	void pushState(BaseState *state);
	void popState();

	BaseState *getCurrentState();
	sf::RenderWindow window;

	int run();
	
private:
	std::stack <BaseState*> states;
};