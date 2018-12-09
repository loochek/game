#include "TiledMap.h"

TiledMap::TiledMap(int w, int h)
{
	width = w;
	height = h;
}

void TiledMap::draw(sf::RenderWindow & window)
{
	sf::Sprite spr;
	spr.setTexture(*tilesheet);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (map[i][j] != 0)
			{
				spr.setPosition(j * 16, i * 16);
				spr.setTextureRect(tiles[map[i][j] - 1]);
				window.draw(spr);
			}
}

std::vector<std::vector<int>>* TiledMap::getMap()
{
	return &map;
}

std::vector<sf::IntRect>* TiledMap::getTiles()
{
	return &tiles;
}

void TiledMap::setTilesheet(sf::Texture * tilesheet)
{
	this->tilesheet = tilesheet;
}

sf::Vector2i TiledMap::getMapTileAtPoint(sf::Vector2f point)
{
	return sf::Vector2i(point.x / 16, point.y / 16);
}

int TiledMap::getMapTileXAtPoint(float x)
{
	return x / 16;
}

int TiledMap::getMapTileYAtPoint(float y)
{
	return y / 16;
}

bool TiledMap::isGround(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return true;
	return map[y][x] != 0;
}

bool TiledMap::isEmpty(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return true;
	return map[y][x] == 0;
}
