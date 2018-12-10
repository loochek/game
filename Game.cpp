#include "Game.h"

Game::Game()
{
	window.create(sf::VideoMode(1280, 720), "Frostbite 4 Tech Demo", sf::Style::Close);
	window.setFramerateLimit(60);
	ResourceManager::load();
	bool mode;
	std::cin >> mode;
	if (mode)
		pushState(new PlayState(this));
	else
		pushState(new ServerState(this));
}

int Game::run()
{
	sf::Clock clock;
	while (this->window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Time frameTime = clock.restart();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			window.close();

		if (frameTime.asMilliseconds() > 16)
			frameTime = sf::milliseconds(16);
		getCurrentState()->update(frameTime);
		window.clear(sf::Color::Blue);
		getCurrentState()->draw();
		window.display();
	}
	ResourceManager::unload();
	return 0;
}

BaseState *Game::getCurrentState()
{
	return states.top();
}

void Game::pushState(BaseState *state)
{
	states.push(state);
}

void Game::popState()
{
	auto state = states.top();
	delete state;
	states.pop();
}

Game::~Game()
{
	while (!states.empty())
		popState();
}