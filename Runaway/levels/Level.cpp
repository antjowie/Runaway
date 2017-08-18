#include "Level.h"
#include "rapidxml.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

bool Level::initMap(std::vector<std::vector<Tile*>> * tileMap)
{
	std::ifstream file(m_tileMapPath);
	rapidxml::xml_document<> map;
	rapidxml::xml_node<> *node;
	int width, height;

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

	std::stringstream converter;

	// Convert c-strings to iterators
	converter << node->first_attribute("width")->value();
	converter >> width;
	converter.clear();
	converter << node->first_attribute("height")->value();
	converter >> height;

	// Get the map from the file
	std::string tileMapString;
	for (rapidxml::xml_node<> *iter = node->first_node(); iter; iter = iter->next_sibling())
		if (std::string(iter->name()) == "layer")
			tileMapString = iter->first_node("data")->value();
	
	// Load the map into the vector
	tileMap->clear();
	for (int i = 0; i < height; ++i)
	{
		tileMap->reserve(height);
		for (int j = 0; j < width; ++j)
		{
			tileMap[i].reserve(width);
			int id = tileMapString[j + width*i] - '0';
			tileMap[i][j].push_back(new Tile(id));
		}
	}
	
	return true;
}

bool Level::initCamera(Camera & camera)
{
	if (m_cameraSize.x <= 0 || m_cameraSize.y <= 0) return false; // Invalid camera size
	camera.setViewSize(m_cameraSize);

	return true;
}

Level::Level(const std::string & tileMapPath, const float cameraWidth, const float camerHeight):
	m_cameraSize(cameraWidth,camerHeight), m_tileMapPath(tileMapPath)
{
}

Level::~Level()
{
}

bool Level::loadLevel(std::vector<std::vector<Tile*>> * tileMap, Camera & camera)
{
	if (!initMap(tileMap)) return false;
	if (!initCamera(camera)) return false;
	return true;
}
