#include "ResourceManager.h"

std::map<std::string, sf::Texture*> ResourceManager::textureBank = { { "init", nullptr } };
std::map<std::string, Animation*> ResourceManager::animationBank = { { "init", nullptr } };

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
			int x, y, w, h;
			animFile >> x >> y >> w >> h;
			anim->addFrame(sf::IntRect(x, y, w, h));
		}
		animationBank[animName] = anim;
		animFile.close();
	}
	animIndex.close();
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
	std::cout << "OK\n";
}

sf::Texture* ResourceManager::getTexture(std::string name)
{
	return textureBank[name];
}

Animation * ResourceManager::getAnimation(std::string name)
{
	return animationBank[name];
}