#pragma once

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <SFML\Graphics.hpp>
#include "Animation.h"

class ResourceManager
{
public:
	static sf::Texture *getTexture(std::string name);
	static Animation *getAnimation(std::string name);
	static void load();
	static void unload();
	
private:
	ResourceManager() {};
	static std::map<std::string, sf::Texture*> textureBank;
	static std::map<std::string, Animation*> animationBank;
};