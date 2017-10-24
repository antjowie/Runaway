#include "Level.h"
#include "rapidxml.hpp"
#include "Checkpoint.h"
#include "Switch.h"
#include "DataManager.h"

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

template<class T>
void converter(T &converted, const std::string &toConvert)
{
	std::stringstream converter;
	converter << toConvert;
	converter >> converted;
}

template<class T>
void converter(T &converted, const char &toConvert)
{
	std::stringstream converter;
	converter << toConvert;
	converter >> converted;
}

bool Level::initMap()
{
	std::ifstream file(m_levelPath + "tilemap.tmx");

	// Check if xml file can be opened
	if (!file.is_open()) return false; 

	// Load the xml file into a buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 
	buffer.push_back('\0');
	file.close();

	// Try to parse the buffer
	if (!loadTilemap(buffer)) return false;
	if (!loadEntities(buffer)) return false;
	if (!loadGates(buffer))return false;
	return true;
}

bool Level::initCamera(Camera & camera)
{
	if (m_cameraSize.x <= 0 || m_cameraSize.y <= 0) return false; // Camera size too small
	if (m_cameraSize.x > m_levelWidth || m_cameraSize.y > m_levelHeight) return false; // Camera size too big
	camera = Camera(
		sf::FloatRect(static_cast<float>(m_spawnX - m_cameraSize.x/2), static_cast<float>(m_spawnY- m_cameraSize.y/2),m_cameraSize.x,m_cameraSize.y), 
		sf::Vector2f(static_cast<float>(m_levelWidth), static_cast<float>(m_levelHeight)),m_cameraSpeed);
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

bool Level::loadTilemap(std::vector<char> tilemap)
{
	rapidxml::xml_document<> map;
	rapidxml::xml_node<> *node;

	try
	{
		map.parse<0>(&tilemap[0]);
	}
	catch (rapidxml::parse_error)
	{
		return false;
	}

	
	// Initiate iterator to the xml file
	node = map.first_node("map");
	converter(m_tileWidth, node->first_attribute("tilewidth")->value());
	converter(m_tileHeight, node->first_attribute("tileheight")->value());
	
	node = node->first_node("layer");
	converter(m_tilemapWidth,  node->first_attribute("width")->value());
	converter(m_tilemapHeight, node->first_attribute("height")->value());

	// Get the map from the file
	std::string tilemapString;
	
	tilemapString = node->first_node("data")->value();

	// Format the tilemapString
	remove_char(tilemapString, ',');
	remove_char(tilemapString, '\n');

	// Load the map into the vector
	m_tilemap.clear();
	m_tilemap.resize(m_tilemapHeight);

	// Because the tiled editor edits tiles from left to right, we have to use j as first value
	for (int j = 0; j < m_tilemapHeight; j++)
	{
		m_tilemap[j].resize(m_tilemapWidth);
		for (int i = 0, id = 0; i < m_tilemapWidth; i++)
		{
			converter(id, tilemapString[j * m_tilemapWidth + i]);

			// While j represent y, it's value is used to evaluate the x axis.
			m_tilemap[j][i] = getTile(id, static_cast<float>(m_tileWidth* i), static_cast<float>(m_tileHeight* j));
		}
	}

	m_levelWidth = m_tilemapWidth * m_tileWidth;
	m_levelHeight = m_tilemapHeight * m_tileHeight;

	// Load camera properties
	for (rapidxml::xml_node<>* iter = node->first_node("properties")->first_node("property"); iter; iter = iter->next_sibling())
	{
	if (std::string(iter->first_attribute("name")->value()) == "cameraWidth")
		converter(m_cameraSize.x, iter->first_attribute("value")->value());
	else if (std::string(iter->first_attribute("name")->value()) == "cameraHeight")
		converter(m_cameraSize.y, iter->first_attribute("value")->value());
	}
	
	m_cameraSize.x *= m_tileWidth;
	m_cameraSize.y *= m_tileHeight;

	// Calculate aspect ratios if desired (assumes 16:9)
	if (m_cameraSize.x == 0)
		m_cameraSize.x = 1920.f / 1080.f * m_cameraSize.y;
	else if (m_cameraSize.y == 0)
		m_cameraSize.y = 1080.f / 1920.f * m_cameraSize.x;

	return true;
}

bool Level::loadEntities(std::vector<char> tilemap)
{
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *node;

	try {
		doc.parse<0>(&tilemap[0]);
	}
	catch (rapidxml::parse_error)
	{
		return false;
	}

	// Access root
	node = doc.first_node("map")->first_node("objectgroup");
	if (!node) return true;

	// Access entitygroup
	while (std::string(node->first_attribute("name")->value()) != "meta")
	{
		if (!node->next_sibling()) return true;
		node = node->next_sibling();
	}

	// Get all the attributes from all entity
	std::string name;
	for (rapidxml::xml_node<> *entity = node->first_node(); entity; entity = entity->next_sibling())
	{
		// Some specific type checking
		converter(name, entity->first_attribute("name")->value());
		if (name == "spawn")
		{
			converter(m_spawnX, entity->first_attribute("x")->value());
			converter(m_spawnY, entity->first_attribute("y")->value());
		}
		else
		{
			// Check entity type
			if (name == "checkpoint")
			{
				sf::Vector2f spawn;
				converter(spawn.x, entity->first_attribute("x")->value());
				converter(spawn.y, entity->first_attribute("y")->value());
				EntityAction action;
				action.pos = spawn;
				m_entityMap.push_back(new Checkpoint(action , action.pos));
			}
			else if (name == "coin")
			{
				EntityAction action;
				converter(action.value, entity->first_attribute("value")->value());
				sf::Vector2f spawn;
				converter(spawn.x, entity->first_attribute("x")->value());
				converter(spawn.y, entity->first_attribute("y")->value());
				//m_entityMap.push_back(new Coin(entityAction, spawn));
			}
		}
	}
	return true;
}

bool Level::loadGates(std::vector<char> tilemap)
{
	struct GateNode
	{
		sf::FloatRect m_pos;
		float m_speed;
		int m_id;
	};

	rapidxml::xml_document<> xmlDoc;
	rapidxml::xml_node<> *xmlNode;
	std::vector<GateNode> gateNodes;

	try
	{
		xmlDoc.parse<0>(&tilemap[0]);
	}
	catch (const rapidxml::parse_error)
	{
		return false;
	}

	// Find objectgroup
	xmlNode=  xmlDoc.first_node("map")->first_node("objectgroup");
	if (!xmlNode) return true;

	// Iterate till objectgroup gateNetwork
	while (std::string(xmlNode->first_attribute("name")->value()) != "gateNetwork")
	{
		if (!xmlNode->next_sibling()) return true;
		xmlNode = xmlNode->next_sibling();
	}

	// Load all values for gateNetwork
	for (rapidxml::xml_node<> *gate{ xmlNode->first_node() }; gate; gate = gate->next_sibling())
	{
		rapidxml::xml_node<> *prop = gate->first_node("properties")->first_node("property");

		if (std::string(gate->first_attribute("name")->value()) == "switch")
		{
			EntityAction action;
			sf::Vector2f pos;
			converter(pos.x, gate->first_attribute("x")->value());
			converter(pos.y, gate->first_attribute("y")->value());


			converter(action.value, prop->first_attribute("value")->value());
			
			m_entityMap.push_back(new Switch(action,pos));
		}
		else
		{
			GateNode gateNode;
			converter(gateNode.m_pos.left,	gate->first_attribute("x")		->value());
			converter(gateNode.m_pos.top,	gate->first_attribute("y")		->value());
			converter(gateNode.m_pos.width, gate->first_attribute("width")	->value());
			converter(gateNode.m_pos.height,gate->first_attribute("height")	->value());

			while (prop)
			{
				if(std::string(prop->first_attribute("name")->value()) == "id")
					converter(gateNode.m_id, prop->first_attribute("value")->value());
				else if (std::string(prop->first_attribute("name")->value()) == "speed")
					converter(gateNode.m_speed, prop->first_attribute("value")->value());
				prop = prop->next_sibling();
			}
			gateNodes.push_back(gateNode);
		}
		
	}

	// Link gate tiles to their gate
 	for (const auto &iter : gateNodes)
	{
		Gate gate(iter.m_id,iter.m_speed);
		for (sf::Vector2i pos = mapWorldToTilemap(sf::Vector2f(iter.m_pos.left, iter.m_pos.top), static_cast<int>(iter.m_pos.width), static_cast<int>(iter.m_pos.height))
			; pos.y >= 0 && m_tilemap[pos.y][pos.x]->getType() == TileType::Gate; pos.y--)
		{
			GateTile * tile = static_cast<GateTile*>(m_tilemap[pos.y][pos.x]);
			gate.addTile(tile);

		}
		m_gateMap.push_back(std::move(gate));
	}

	// Update gateTile texture
	for (auto &iter : m_gateMap)
		iter.updateTextureRect();

	return true;
}

Level::Level(const std::string &levelPath, const std::string &title, const float cameraSpeed, const std::string tilesetName): 
	m_title(title), m_levelPath(levelPath), m_cameraSpeed(cameraSpeed), m_tilesetName(tilesetName)
{
	std::string tilesetPath("Runaway/data/textures/tilesets/" + tilesetName);
	DataManager::getInstance().loadTexture("tileset", tilesetPath + "/tileset.png");
	DataManager::getInstance().loadTexture("gate", tilesetPath + "/gateSprite.png");
}

Level::~Level()
{
	// Does this even work?
	for (auto &iter : m_tilemap)
		for (auto &deleter : iter)
			delete deleter;
	m_tilemap.clear();

	for (auto &deleter : m_entityMap)
		delete deleter;
	m_entityMap.clear();
}

bool Level::loadLevel(Camera & camera, PlayerObject * const player, GameBackground &background)
{
	if (!initMap()) return false;
	if (!initPlayer(player)) return false;
	if (!initCamera(camera)) return false;
	background = GameBackground(m_levelPath,10);
	return true;
}

void Level::update(const float elapsedTime)
{
	for (const auto &iter : m_entityMap)
	{
		iter->logic(elapsedTime);
	}
	for (auto &iter : m_gateMap)
	{
		iter.update(elapsedTime);
	}
}

void Level::draw(sf::RenderWindow & window, const Camera &camera)
{
	sf::IntRect tileBounds = camera.getTileBounds(m_tileWidth, m_tileHeight);
	
	// So that gate wont be rendered above tiles
	for (int i = tileBounds.top; i < tileBounds.height + tileBounds.top; ++i)
		for (int j = tileBounds.left; j < tileBounds.width + tileBounds.left; ++j)
			if(m_tilemap[i][j]->getType() == TileType::Gate)
				m_tilemap[i][j]->draw(window);

	for (const auto &iter : m_entityMap)
	{
		iter->draw(window);
	}
	

	for(int i = tileBounds.top;i < tileBounds.height + tileBounds.top;++i)
		for(int j = tileBounds.left;j < tileBounds.width + tileBounds.left; ++j)
			if (m_tilemap[i][j]->getType() != TileType::Gate)
			m_tilemap[i][j]->draw(window);
}

bool Level::inLevelBounds(const sf::Vector2f & point)
{
	return point.y > m_levelHeight;
}

void Level::toggleGate(const int id)
{
	for (auto &iter : m_gateMap)
		if (iter.m_id == id)
			iter.m_isOpen = iter.m_isOpen ? false : true;
}

void Level::setSpawn(const sf::Vector2f & pos)
{
	m_spawnX = pos.x;
	m_spawnY = pos.y;
}

const std::vector<std::vector<Tile*>> &Level::getTilemap() const
{
	return m_tilemap;
}

const sf::Vector2f Level::getSpawn() const
{
	return sf::Vector2f(m_spawnX, m_spawnY);
}

const std::string & Level::getTitle() const
{
	return m_title;
}

const std::vector<Entity*>& Level::getEntityMap() const
{
	return m_entityMap;
}