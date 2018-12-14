#include "PlayState.h"
#include "Game.h"

PlayState::PlayState(Game* game) : BaseState(game)
{
	std::cout << "PlayState()\n";
	map = ResourceManager::getMap("test");
	player = new Player(map, AABB(sf::Vector2f(8, 16), sf::Vector2f(0, 0)));
	view.setCenter(player->position);
	view.setSize(854, 480);
}

PlayState::~PlayState()
{
	std::cout << "~PlayState()\n";
}

void PlayState::update(sf::Time frameTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		sf::Vector2i pixelPos = sf::Mouse::getPosition(game->window);
		sf::Vector2f dir = game->window.mapPixelToCoords(pixelPos) - player->position;
		float length = sqrt(dir.x * dir.x + dir.y * dir.y);
		Bullet *bul = new Bullet(map, player->position, sf::Vector2f(dir.x / length * 1000, dir.y / length * 1000));
		bullets.push_back(bul);
	}
	player->update(frameTime);
	for (Bullet *i : bullets)
		i->update(frameTime);
	while (!bullets.empty() && bullets.front()->dead)
		bullets.pop_front();
	float lerp = 3.f;
	sf::Vector2f viewPos = view.getCenter();
	viewPos.x += (player->position.x - viewPos.x) * lerp * frameTime.asSeconds();
	viewPos.y += (player->position.y - viewPos.y) * lerp * frameTime.asSeconds();
	view.setCenter(viewPos);
}

void PlayState::draw()
{
	game->window.setView(view);
	map->draw(game->window);
	game->window.draw(*player);
	for (Bullet *i : bullets)
		game->window.draw(*i);
}