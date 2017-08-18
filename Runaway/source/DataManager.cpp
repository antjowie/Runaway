#include "DataManager.h"

DataManager::DataManager()
{
	// Load fonts
	loadFont("pixel", "Runaway/data/fonts/Pixeled.ttf");

	// Load textures
	loadTexture("player", "Runaway/data/textures/player.png");
	loadTexture("mainMenuBackground", "Runaway/data/textures/mainMenuBackground.png");
}

DataManager & DataManager::getInstance()
{
	static DataManager instance;
	return instance;
}

void DataManager::loadTexture(const std::string textureName, const std::string texturePath, const bool isRepeated)
{
	sf::Texture texture;

	texture.loadFromFile(texturePath);
	texture.setRepeated(isRepeated);
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
