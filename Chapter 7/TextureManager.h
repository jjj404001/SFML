#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

//Curiously Recurring Template Pattern
class TextureManager 
	:	public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager() : ResourceManager("Textures.cfg") {}

	sf::Texture * Load(const std::string & path)
	{
		sf::Texture * texture = new sf::Texture();
		if (!texture->loadFromFile(Utils::GetWorkingDirectory() + path))
		{
			delete texture;
			texture = nullptr;
			std::cerr << "! Failed to load texture: " << path << std::endl;
		}
		return texture;
	}
};

#endif