#include "ResourceManager.h"

std::map<std::string, sf::Texture*> ResourceManager::textureBank = { { "init", nullptr } };
std::map<std::string, Animation*> ResourceManager::animationBank = { { "init", nullptr } };
std::map<std::string, TiledMap*> ResourceManager::mapBank = { { "init", nullptr } };

void ResourceManager::load()
{
	std::cout << "Loading resources...\n";
	std::ifstream textureIndex("resources/textures.idx");
	std::string textureName;
	while (textureIndex >> textureName)
	{
		std::cout << "textures/" + textureName << "\n";
		sf::Image img;
		sf::Texture *tx = new sf::Texture();
		img.loadFromFile("resources/textures/" + textureName + ".png");
		img.createMaskFromColor(sf::Color(233, 54, 224));
		tx->loadFromImage(img);
		textureBank[textureName] = tx;
	}
	textureIndex.close();
	std::ifstream animIndex("resources/animations.idx");
	std::string animName;
	while (animIndex >> animName)
	{
		std::cout << "animations/" + animName << "\n";
		std::ifstream animFile("resources/animations/" + animName + ".anim");
		int framesCount;
		std::string spritesheetName;
		animFile >> framesCount >> spritesheetName;
		if (textureBank.find(spritesheetName) == textureBank.end())
		{
			std::cout << "Warning! Texture " << spritesheetName << " not found\n";
		}
		Animation *anim = new Animation();
		anim->setSpriteSheet(*textureBank[spritesheetName]);
		for (int i = 0; i < framesCount; i++)
		{
			int x, y, width, height;
			animFile >> x >> y >> width >> height;
			anim->addFrame(sf::IntRect(x, y, width, height));
		}
		animationBank[animName] = anim;
		animFile.close();
	}
	animIndex.close();
	std::ifstream mapIndex("resources/maps.idx");
	std::string mapName;
	while (mapIndex >> mapName)
	{
		std::cout << "maps/" + mapName << "\n";
		std::ifstream mapFile("resources/maps/" + mapName + ".map");
		int width, height, tilesCount;
		std::string tilesheetName;
		mapFile >> width >> height >> tilesheetName >> tilesCount;
		TiledMap *map = new TiledMap(width, height);
		if (textureBank.find(tilesheetName) == textureBank.end())
		{
			std::cout << "Warning! Texture " << tilesheetName << " not found\n";
		}
		else
		{
			map->setTilesheet(getTexture(tilesheetName));
		}
		std::vector<std::vector<int>> *mapArray = map->getMap();
		mapArray->resize(height);
		for (int i = 0; i < height; i++)
		{
			(*mapArray)[i].resize(width);
			for (int j = 0; j < width; j++)
			{
				mapFile >> (*mapArray)[i][j];
			}
		}
		std::vector<sf::IntRect> *tilesArray = map->getTiles();
		tilesArray->resize(tilesCount);
		for (int i = 0; i < tilesCount; i++)
		{
			int tx, ty, tw, th;
			mapFile >> tx >> ty >> tw >> th;
			(*tilesArray)[i] = sf::IntRect(tx, ty, tw, th);
		}
		mapBank[mapName] = map;
		mapFile.close();
	}
	mapIndex.close();
	std::cout << "Resources loading complete\n";
}

void ResourceManager::unload()
{
	std::cout << "Unloading resources...";
	for (auto i : textureBank)
	{
		delete i.second;
	}
	for (auto i : animationBank)
	{
		delete i.second;
	}
	for (auto i : mapBank)
	{
		delete i.second;
	}
	std::cout << "OK\n";
}

sf::Texture* ResourceManager::getTexture(std::string name)
{
	return textureBank[name];
}

Animation* ResourceManager::getAnimation(std::string name)
{
	return animationBank[name];
}

TiledMap* ResourceManager::getMap(std::string name)
{
	return mapBank[name];
}