#include "DataManager.h"

DataManager::DataManager()
{
	std::string dataPath("Runaway/data/");
	
	// Load fonts
	std::string fontPath(dataPath + "fonts/");
	loadFont("pixel", fontPath + "Pixeled.ttf");

	// Load textures
	std::string texturePath(dataPath + "textures/");

	// Load entities
	std::string entityPath(texturePath + "entities/");
	loadTexture("player", entityPath + "player/playerSprite.png");
	loadTexture("checkpoint", entityPath + "checkpoint/checkpointSprite.png");
	loadTexture("switch", entityPath + "switch/switchSprite.png");

	// Load meta
	std::string metaPath(texturePath + "meta/");
	loadTexture("lightCircle", metaPath + "lightCircle/lightCircle.png");

	// Load misc
	loadTexture("mainMenuBackground", texturePath + "mainMenuBackground.png");

	// Load level backgrounds
	std::string levelPath(dataPath + "levels/");
	loadTexture("testBackground", levelPath + "test/background.png");
	loadTexture("thatBackground", levelPath + "that/background.png");

	getTexture("player").setSmooth(true);
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
