#include "DataManager.h"

#include <iostream>


DataManager::DataManager()
{

}

DataManager & DataManager::getInstance()
{
	std::cout << "DataManager object created!\n";
	static DataManager instance;
	return instance;
}

void DataManager::loadTexture(const std::string textureName, const std::string texturePath, const bool isRepeated = false)
{
	sf::Texture texture;

	texture.loadFromFile(texturePath);
	texture.isRepeated = isRepeated;
	m_textureMap[textureName] = texture;
}

void DataManager::loadFont(const std::string fontName, const std::string fontPath)
{
	sf::Font font;

	font.loadFromFile(fontPath);
	m_fontMap[fontName] = font;
}

sf::Texture & DataManager::getTexture(const std::string textureName)
{
	return m_textureMap[textureName];
}

sf::Font & DataManager::getFont(const std::string fontName)
{
	return m_fontMap[fontName];
}
