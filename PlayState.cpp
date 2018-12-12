#include "PlayState.h"
#include "Game.h"

PlayState::PlayState(Game* game) : BaseState(game)
{
	std::cout << "PlayState()\n";
	map = ResourceManager::getMap("test");
	player = Player(map);
}

PlayState::~PlayState()
{
	std::cout << "~PlayState()\n";
}

void PlayState::update(sf::Time frameTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition(game->window)) - player.position;
		float length = sqrt(dir.x * dir.x + dir.y * dir.y);
		Bullet *bul = new Bullet(map, player.position, sf::Vector2f(dir.x / length * 1000, dir.y / length * 1000));
		bullets.push_back(bul);
	}
	player.update(frameTime);
	for (Bullet *i : bullets)
		i->update(frameTime);
	while (!bullets.empty() && bullets.front()->dead)
		bullets.pop_front();
}

void PlayState::draw()
{
	map->draw(game->window);
	game->window.draw(player);
	for (Bullet *i : bullets)
		game->window.draw(*i);
}