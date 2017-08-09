//
// The DataManager class is responsible for all the textures and fonts
// Because not many files will be loaded, all files will be loaded at once,
// we could however, load the files while a menu is booting, for efficiency's sake
//

#pragma once
#include <map>
#include <SFML\Graphics.hpp>

class DataManager
{
private:
	std::map<std::string, sf::Texture>  m_textureMap;
	std::map<std::string, sf::Font>		m_fontMap;

	DataManager();

public:
	static DataManager &getInstance();

	void loadTexture(const std::string textureName, const std::string texturePath, const bool isRepeated = false);
	void loadFont(const std::string fontName, const std::string fontPath);

	sf::Texture &getTexture(const std::string textureName);
	sf::Font &getFont(const std::string fontName);
};
