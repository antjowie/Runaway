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

const sf::Vector2i mapWorldToTilemap(const sf::Vector2f &coords, sf::Vector2i &tileSize)
{
	sf::Vector2i tempVec{ coords };

	// Value mapped to int will always be correct. Since int will round down
	tempVec.x /= tileSize.x;
	tempVec.y /= tileSize.y;
	return tempVec;
}

const sf::Vector2i mapWorldToTilemap(const sf::Vector2f & coords, const int tileWidth, const int tileHeight)
{
	return mapWorldToTilemap(coords, sf::Vector2i(tileWidth, tileHeight));
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
	file.close();

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
	converter >> m_tilemapWidth;
	converter.clear();
	converter << node->first_attribute("height")->value();
	converter >> m_tilemapHeight;
	converter.clear();
	converter << node->first_attribute("tilewidth")->value();
	converter >> m_tileWidth;
	converter.clear();
	converter << node->first_attribute("tileheight")->value();
	converter >> m_tileHeight;
	
	// Get the map from the file
	std::string tilemapString;
	
	for (rapidxml::xml_node<> *iter = node->first_node(); iter; iter = iter->next_sibling())
		if (std::string(iter->name()) == "layer")
			tilemapString = iter->first_node("data")->value();
	
	// Format the tilemapString
	remove_char(tilemapString, ',');
	remove_char(tilemapString, '\n');

	// Load the map into the vector
	m_tilemap.clear();
	m_tilemap.resize(m_tilemapWidth);
	
	for (int i = 0; i < m_tilemapWidth; i++)
	{
		m_tilemap[i].resize(m_tilemapHeight);
		for (int j = 0, id = 0; j < m_tilemapHeight; j++)
		{
			converter << tilemapString[i * m_tilemapHeight + j];
			converter >> id;
			converter.clear();

			// While j represent y, it's value is used to evaluate the x axis.
			m_tilemap[i][j] = new Tile(id, static_cast<float>(m_tileWidth* j), static_cast<float>(m_tileHeight* i));
		}
	}
	
	m_levelWidth = m_tilemapWidth * m_tileWidth;
	m_levelHeight = m_tilemapHeight * m_tileHeight;

	return true;
}

bool Level::initCamera(Camera & camera)
{
	if (m_cameraSize.x <= 0 || m_cameraSize.y <= 0) return false; // Camera size too small
	if (m_cameraSize.x > m_levelWidth || m_cameraSize.y > m_levelHeight) return false; // Camera size too big
	camera = Camera::Camera(
		sf::FloatRect(static_cast<float>(m_spawnX), static_cast<float>(m_spawnY),m_cameraSize.x,m_cameraSize.y), 
		sf::Vector2f(static_cast<float>(m_levelWidth), static_cast<float>(m_levelHeight)),
		m_cameraSpeed);

	return true;
}

bool Level::initPlayer(PlayerObject * const player)
{
	// Sprite
	player->m_sprite.setPos(sf::Vector2f((float)m_spawnX, (float)m_spawnY));
	
	// Collision
	player->m_collisionHandler.setTileSize(sf::Vector2i(m_tileWidth, m_tileHeight));
	if (!player->m_collisionHandler.loadTilemap(&m_tilemap)) return false;
	return true;
}

Level::Level(const std::string &levelMapPath, const std::string &title, 
	const float cameraWidth, const float camerHeight, 
	const float cameraSpeed, const int spawnX, const int spawnY):
	m_cameraSize(cameraWidth,camerHeight), m_title(title), 
	m_levelMapPath(levelMapPath), m_cameraSpeed(cameraSpeed), 
	m_spawnX(spawnX), m_spawnY(spawnY)
{
}

bool Level::loadLevel(Camera & camera, PlayerObject * const player)
{
	if (!initMap()) return false;
	if (!initPlayer(player)) return false;
	if (!initCamera(camera)) return false;
	return true;
}

void Level::draw(sf::RenderWindow & window, const Camera &camera)
{
	sf::IntRect tileBounds = camera.getTileBounds(m_tileWidth, m_tileHeight);

	for(int i = tileBounds.top;i < tileBounds.height + tileBounds.top;++i)
		for(int j = tileBounds.left;j < tileBounds.width + tileBounds.left; ++j)
			m_tilemap[i][j]->draw(window);
}

const std::vector<std::vector<Tile*>> &Level::getTileMap() const
{
	return m_tilemap;
}

const sf::Vector2i Level::getSpawn() const
{
	return sf::Vector2i(m_spawnX, m_spawnY);
}

const std::string & Level::getTitle() const
{
	return m_title;
}
