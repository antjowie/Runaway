#include "Level.h"
#include "rapidxml.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

void remove_char(std::string &str, char ch)
{
	// Remove all occurrences of char ch from str
	str.erase(std::remove(str.begin(), str.end(), ch), str.end());
}

bool Level::initMap()
{
	std::ifstream file(m_levelMapPath);
	rapidxml::xml_document<> map;
	rapidxml::xml_node<> *node;
	
	// Check if xml file can be opened
	if (!file.is_open()) return false; 

	// Load the xml file into a buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 
	buffer.push_back('\0');

	// Try to parse the buffer
	try
	{
		map.parse<0>(&buffer[0]);
	}
	catch (rapidxml::parse_error)
	{
		return false;
	}
		
	// Initiate iterator to the xml file
	node = map.first_node("map");

	// Convert c-strings to iterators
	std::stringstream converter;

	converter << node->first_attribute("width")->value();
	converter >> m_tileWidth;
	converter.clear();
	converter << node->first_attribute("height")->value();
	converter >> m_tileHeight;
	converter.clear();

	// Get the map from the file
	std::string tileMapString;
	
	for (rapidxml::xml_node<> *iter = node->first_node(); iter; iter = iter->next_sibling())
		if (std::string(iter->name()) == "layer")
			tileMapString = iter->first_node("data")->value();
	
	// Format the tileMapString
	remove_char(tileMapString, ',');
	remove_char(tileMapString, '\n');

	// Load the map into the vector
	m_tileMap.clear();
	m_tileMap.resize(m_tileHeight);
	
	for (int i = 0; i < m_tileHeight; ++i)
	{
		m_tileMap[i].resize(m_tileWidth);
		for (int j = 0, id = 0; j < m_tileWidth; ++j)
		{
			converter << tileMapString[j + m_tileWidth * i];
			converter >> id;
			converter.clear();

			m_tileMap[i][j] = new Tile(id, 32.0f * j, 32.0f * i);
		}
	}
	
	m_levelWidth = m_tileWidth * 32;
	m_levelHeight = m_tileHeight * 32;

	return true;
}

bool Level::initCamera(Camera & camera)
{
	if (m_cameraSize.x <= 0 || m_cameraSize.y <= 0) return false; // Invalid camera size
	camera.setViewSize(m_cameraSize);
	camera.setSpeed(1.0f);

	return true;
}

Level::Level(const std::string &levelMapPath, const std::string & title, const float cameraWidth, const float camerHeight):
	m_cameraSize(cameraWidth,camerHeight), m_title(title), m_levelMapPath(levelMapPath)
{
}

bool Level::loadLevel(Camera & camera)
{
	if (!initMap()) return false;
	if (!initCamera(camera)) return false;
	return true;
}

void Level::draw(sf::RenderWindow & window)
{
	for (int i = 0; i < m_tileHeight; ++i)
		for (int j = 0; j < m_tileWidth; ++j)
			m_tileMap[i][j]->draw(window);
}
