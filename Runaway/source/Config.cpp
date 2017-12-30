#include "Config.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "menus\GameMenu.h"

#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>

Config::Config()
{
}

void Config::loadConfig()
{
	// Load the file
	std::ifstream configFile("config.xml");
	if (!configFile.is_open())
	{
		loadDefaultConfig();
		return;
	}
	
	// Format the file
	std::vector<char> buffer((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0'); 
	
	// Parse the buffer
	rapidxml::xml_document<> config;
	config.parse<0>(&buffer[0]);
	// Read necessary attributes
	std::stringstream converter;
	for (rapidxml::xml_node<> *node = config.first_node("config")->first_node(); node; node = node->next_sibling())
	{
		int temp;
		for (rapidxml::xml_attribute<> * attr = node->first_attribute(); attr; attr = attr->next_attribute())
		{
			converter << attr->value();
			converter >> temp;
			converter.clear();
			if (std::string(node->name()) == "keyboard")
				m_config.emplace(attr->name(), sf::Keyboard::Key(temp));
			else if (std::string(node->name()) == "mouse")
				m_config.emplace(attr->name(), sf::Mouse::Button(temp));
			else if (std::string(node->name()) == "logic")
				m_config.emplace(attr->name(), temp == 1 ? true : false);
			else if (std::string(node->name()) == "integer")
				m_config.emplace(attr->name(), temp);
		}
	}
	if (checkConfig())
		loadDefaultConfig();
}

void Config::loadDefaultConfig()
{
	m_config.clear();

	m_config.emplace("moveLeft",	sf::Keyboard::Key::A);
	m_config.emplace("moveRight",	sf::Keyboard::Key::D);
	m_config.emplace("dash",		sf::Keyboard::Key::LShift);
	m_config.emplace("jump",		sf::Keyboard::Key::Space);

	m_config.emplace("shoot",	sf::Mouse::Button::Left);
	m_config.emplace("respawn", sf::Keyboard::Key::P);

	m_config.emplace("frameLimit", 60);

	m_config.emplace("level", 0);
	
	m_config.emplace("music", 100);
	m_config.emplace("effects", 100);

	for (int i = 0; i < static_cast<int>(LevelName::Blank) - 1; i++)
		m_config.emplace(std::string("coin") + std::to_string(i), false);
}

void Config::saveConfig()
{ 
	std::ofstream config("config.xml", std::ios::trunc);
	rapidxml::xml_document<> doc;
	
	// Add declaration node
	rapidxml::xml_node<> *decl = doc.allocate_node(rapidxml::node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "UTF-8"));
	doc.append_node(decl);

	// Add root node
	rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_element, "config");
	doc.append_node(root);

	// Add keyboard node
	rapidxml::memory_pool<> pool;
	rapidxml::xml_node<> *keyboard = doc.allocate_node(rapidxml::node_element, "keyboard");
	for (auto iter : m_config)
		if (iter.second.itemType == Item::ItemType::Keyboard)
			keyboard->append_attribute(doc.allocate_attribute(pool.allocate_string(iter.first.c_str()) , pool.allocate_string(std::to_string(iter.second.keyboard).c_str())));
	root->append_node(keyboard);

	// Add mouse node
	rapidxml::xml_node<> *mouse = doc.allocate_node(rapidxml::node_element, "mouse");
	for (auto iter : m_config)
		if (iter.second.itemType == Item::ItemType::Mouse)
			mouse->append_attribute(doc.allocate_attribute(pool.allocate_string(iter.first.c_str()), pool.allocate_string(std::to_string(iter.second.mouse).c_str())));
	root->append_node(mouse);

	// Add logic node
	rapidxml::xml_node<> *logic = doc.allocate_node(rapidxml::node_element, "logic");
	for (auto iter : m_config)
		if (iter.second.itemType == Item::ItemType::Logic)
			logic->append_attribute(doc.allocate_attribute(pool.allocate_string(iter.first.c_str()), pool.allocate_string(std::to_string(iter.second.logic).c_str())));
	root->append_node(logic);

	// Add int node
	rapidxml::xml_node<> *integer = doc.allocate_node(rapidxml::node_element, "integer");
	for (auto iter : m_config)
		if (iter.second.itemType == Item::ItemType::Integer)
			integer->append_attribute(doc.allocate_attribute(pool.allocate_string(iter.first.c_str()), pool.allocate_string(std::to_string(iter.second.integer).c_str())));
	root->append_node(integer);

	config << doc;
	config.close();
}

Config::~Config()
{
	saveConfig();
}

bool Config::checkConfig()
{
	// This function checks if all config data is initialized. If someone didn't modify the file from within the program,
	// the file may be missing some setting and thus will load the default config
	bool corrupt = false;

	// Quite ugly
	if (!m_config.count("moveLeft")) corrupt = true;
	if (!m_config.count("moveRight")) corrupt = true;
	if (!m_config.count("dash")) corrupt = true;
	if (!m_config.count("jump")) corrupt = true;
	
	if (!m_config.count("shoot")) corrupt = true;
	if (!m_config.count("respawn")) corrupt = true;

	if (!m_config.count("frameLimit")) corrupt = true;
	if (!m_config.count("level")) corrupt = true;

	if (!m_config.count("music")) corrupt = true;
	if (!m_config.count("effects")) corrupt = true;
	
	for (int i = 0; i < static_cast<int>(static_cast<int>(LevelName::Blank) - 1); i++)
		if (!m_config.count(std::string("coin") + std::to_string(i))) corrupt = true;

	return corrupt;
}

Config & Config::getInstance()
{
	static Config instance;
	return instance;
}

const Item Config::getConfig(const std::string key)
{
	return m_config[key];
}

void Config::setConfig(const std::string key, Item value)
{
	// It doesn't want to overwrite???
	m_config.erase(key);
	m_config.emplace(key, value);
}

Item::Item(const sf::Keyboard::Key key):
	itemType(ItemType::Keyboard), keyboard(key)
{
}

Item::Item(const sf::Mouse::Button button):
	itemType(ItemType::Mouse), mouse(button)
{
}

Item::Item(const bool logic):
	itemType(ItemType::Logic), logic(logic)
{
}

Item::Item(const int integer) :
	itemType(ItemType::Integer), integer(integer)
{
}

Item::Item()
{
}

Item Item::operator=(const Item &)
{
	return *this;
}