#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class TiledMap
{
public:
	TiledMap(int w, int h);

	void draw(sf::RenderWindow &window);

	std::vector<std::vector<int>> *getMap();
	std::vector<sf::IntRect> *getTiles();
	void setTilesheet(sf::Texture *tilesheet);

private:
	int width;
	int height;

	std::vector<std::vector<int>> map;

	sf::Texture *tilesheet;
	std::vector<sf::IntRect> tiles;
};