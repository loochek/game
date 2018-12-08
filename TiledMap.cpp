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
				spr.setPosition(j * 32, i * 32);
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
