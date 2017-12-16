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
	loadTexture("playerLight", entityPath + "player/playerLightSprite.png");

	loadTexture("checkpoint", entityPath + "checkpoint/checkpointSprite.png");
	loadTexture("switch", entityPath + "switch/switchSprite.png");
	loadTexture("finish", entityPath + "finish/finishSprite.png");
	loadTexture("coin", entityPath + "coin/coinSprite.png");

	// Load meta
	std::string metaPath(texturePath + "meta/");
	loadTexture("lightCircle", metaPath + "lightCircle/lightCircle.png");

	// Load misc
	loadTexture("mainMenuBackground", texturePath + "mainMenuBackground.png");

	// Load level backgrounds
	std::string levelPath(dataPath + "levels/");
	loadTexture("wake upBackground", levelPath + "wake up/background.png");
	loadTexture("the gateBackground", levelPath + "the gate/background.png");
	
	getTexture("player").setSmooth(true);

	// Load sounds
	std::string soundPath(dataPath + "sounds/");
	loadSound("lightPool", soundPath + "lightPool.wav");
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

void DataManager::loadSound(const std::string soundName, const std::string soundPath)
{
	sf::SoundBuffer sound;
	sound.loadFromFile(soundPath);
	m_soundMap[soundName] = sound;
}

sf::Texture & DataManager::getTexture(const std::string textureName)
{
	return m_textureMap[textureName];
}

sf::Font & DataManager::getFont(const std::string fontName)
{
	return m_fontMap[fontName];
}

sf::SoundBuffer & DataManager::getSound(const std::string soundName)
{
	return m_soundMap[soundName];
}
