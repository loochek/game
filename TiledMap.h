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

	sf::Vector2i getMapTileAtPoint(sf::Vector2f point);
	int getMapTileXAtPoint(float x);
	int getMapTileYAtPoint(float y);
	bool isGround(int x, int y);
	bool isEmpty(int x, int y);

private:
	int width;
	int height;

	std::vector<std::vector<int>> map;

	sf::Texture *tilesheet;
	std::vector<sf::IntRect> tiles;
};