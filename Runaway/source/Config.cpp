#include "Config.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>

#include <iostream>

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
	checkConfig();
}

void Config::loadDefaultConfig()
{
	m_config.clear();

	m_config.emplace("moveUp",		sf::Keyboard::Key::W);
	m_config.emplace("moveLeft",	sf::Keyboard::Key::A);
	m_config.emplace("moveDown",	sf::Keyboard::Key::S);
	m_config.emplace("moveRight",	sf::Keyboard::Key::D);

	m_config.emplace("jump", sf::Keyboard::Key::Space);
	m_config.emplace("dash", sf::Keyboard::Key::LShift);

	m_config.emplace("frameLimit", 60);

	m_config.emplace("level", 0);

	for (int i = 0; i < 12; i++)
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

void Config::checkConfig()
{
	// This function checks if all config data is initialized. If someone didn't modify the file from within the program,
	// the file may be missing some setting and thus will load the default config
	bool corrupt = false;

	// Quite ugly
	if (!m_config.count("moveUp")) corrupt = true;
	if (!m_config.count("moveLeft")) corrupt = true;
	if (!m_config.count("moveDown")) corrupt = true;
	if (!m_config.count("moveRight")) corrupt = true;

	if (!m_config.count("jump")) corrupt = true;
	if (!m_config.count("dash")) corrupt = true;

	if (!m_config.count("frameLimit")) corrupt = true;
	if (!m_config.count("level")) corrupt = true;

	for (int i = 0; i < 12; i++)	
		if (!m_config.count(std::string("coin") + std::to_string(i))) corrupt = true;


	if (corrupt)
		loadDefaultConfig();
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