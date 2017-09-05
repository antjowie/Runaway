//
// This class will handle the config file
//

#pragma once
#include <map>
#include <SFML\window.hpp>

class Item	
{
public:
	enum class ItemType
	{
		Keyboard,
		Mouse,
		Logic,
		Integer
	}itemType;
	
	union
	{
		sf::Keyboard::Key	keyboard; 
		sf::Mouse::Button	mouse; 
		bool				logic;
		int					integer;
	};

	Item(const sf::Keyboard::Key key);
	Item(const sf::Mouse::Button button);
	Item(const bool logic);
	Item(const int integer);
	Item();

	Item operator=(const Item&);
};

class Config
{
private:
	Config();
	~Config();

	std::map<std::string, Item> m_config;

	void checkConfig();

public:
	static Config &getInstance();
	
	const Item getConfig(const std::string key);
	void setConfig(const std::string key, Item value); 

	void loadConfig();
	void loadDefaultConfig();
	void saveConfig();
};