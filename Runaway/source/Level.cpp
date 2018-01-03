#include "Level.h"
#include "Checkpoint.h"
#include "Switch.h"
#include "Finish.h"
#include "DataManager.h"
#include "LightTrail.h"
#include "LightTile.h"
#include "Coin.h"

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

bool Level::initMap(SoundManager & soundManager)
{
	std::ifstream file(m_levelPath + "tilemap.tmx");

	// Check if xml file can be opened
	if (!file.is_open()) return false; 

	// Load the xml file into a buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 
	buffer.push_back('\0');
	file.close();

	// Load all files
	if (!loadTileLayer(buffer,soundManager)) return false;
	return true;
}

bool Level::initCamera(Camera & camera)
{
	if (m_cameraSize.x <= 0 || m_cameraSize.y <= 0) return false; // Camera size too small
	if (m_cameraSize.x > m_levelWidth || m_cameraSize.y > m_levelHeight) return false; // Camera size too big
	camera = Camera(
		sf::FloatRect(m_spawnX, m_spawnY,m_cameraSize.x,m_cameraSize.y), 
		sf::Vector2f(static_cast<float>(m_levelWidth), static_cast<float>(m_levelHeight)),m_cameraSpeed);
	camera.update(0); // This will correct the bounds
	
	return true;
}

bool Level::initPlayer(PlayerObject * const player)
{
	// Sprite
	player->m_sprite.setPos(sf::Vector2f((float)m_spawnX, (float)m_spawnY));
	
	// Collision
	player->m_collisionHandler.setTileSize(sf::Vector2i(m_tileWidth, m_tileHeight));
	if (!player->m_collisionHandler.loadTilemap(&m_tilemap)) return false;

	// Launcher
	player->m_launcher.loadTilemap(&m_tilemap);
	return true;
}

bool Level::initBackground(GameBackground & background)
{
	background = GameBackground(m_levelPath, 4);
	background.init(sf::Vector2f(m_levelWidth, m_levelHeight));
	return true;
}

bool Level::initLight(Light & light, const PlayerObject * const player)
{
	light.setFadeTime(3);
	light.setSize(m_levelWidth, m_levelHeight);
	light.setBrightness(255);

	// Add static light tiles
	for(int i = 0; i < m_tilemapHeight; ++i)
		for (int j = 0; j < m_tilemapWidth; ++j)
		{
			if (!m_background.empty() && m_background[i][j]->getTileMeta().m_light != 0)
				light.addDrawable(m_background[i][j]);
			if (m_tilemap[i][j]->getTileMeta().m_tileType == TileType::Light)
			{
				light.addDrawable(m_tilemap[i][j]);
				light.addDrawable(&static_cast<LightTile*>(m_tilemap[i][j])->getLightPool());
			}
		}

	for (const auto &iter : m_gateMap)
	{
		light.addDrawable(iter.getTopTile());
		light.addDrawable(iter.getBottomTile());
	}

	for (const auto &iter : m_entityMap)
		if (iter->getType() == EntityType::Finish)
			light.addDrawable(iter);

	light.addDrawable(&player->m_trail);
	light.addDrawable(&player->m_lightPool);
	light.addDrawable(&player->m_launcher);
	return true;
}

void Level::loadTilemap(std::vector<std::vector<Tile*>>& tilemap, const std::string tilemapString)
{
	// Load the map into the vector
	tilemap.clear();
	tilemap.resize(m_tilemapHeight);

	// Because the tiled editor edits tiles from left to right, we have to use j as first value
	for (int j = 0; j < m_tilemapHeight; j++)
	{
		tilemap[j].resize(m_tilemapWidth);
		for (int i = 0, id = 0; i < m_tilemapWidth; i++)
		{
			converter(id, tilemapString[j * m_tilemapWidth + i]);

			// While j represent y, it's value is used to evaluate the x axis.
			tilemap[j][i] = getTile(id, static_cast<float>(m_tileWidth* i), static_cast<float>(m_tileHeight* j));
		}
	}
}

bool Level::loadTileLayer(std::vector<char> tilemap, SoundManager & soundManager)
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
	
	// Load camera properties
	for (rapidxml::xml_node<>* iter = node->first_node("properties")->first_node("property"); iter; iter = iter->next_sibling())
	{
		if (std::string(iter->first_attribute("name")->value()) == "cameraWidth")
			converter(m_cameraSize.x, iter->first_attribute("value")->value());
		else if (std::string(iter->first_attribute("name")->value()) == "cameraHeight")
			converter(m_cameraSize.y, iter->first_attribute("value")->value());
	}

	m_cameraSize.x *= static_cast<float>(m_tileWidth);
	m_cameraSize.y *= static_cast<float>(m_tileHeight);

	// Calculate aspect ratios if desired (assumes 16:9)
	if (m_cameraSize.x == 0.0f)
		m_cameraSize.x = 1920.f / 1080.f * m_cameraSize.y;
	else if (m_cameraSize.y == 0.0f)
		m_cameraSize.y = 1080.f / 1920.f * m_cameraSize.x;

	

	// Load level attributes
	node = node->first_node("layer");
	converter(m_tilemapWidth,  node->first_attribute("width")->value());
	converter(m_tilemapHeight, node->first_attribute("height")->value());

	for (rapidxml::xml_node<> * tilemap = node; tilemap; tilemap= tilemap->next_sibling())
	{
		if (std::string(tilemap->name()) != "layer") continue;
		// Get the map from the file
		std::string tilemapString;
	
		tilemapString = tilemap->first_node("data")->value();

		// Format the tilemapString
		remove_char(tilemapString, ',');
		remove_char(tilemapString, '\n');

		if (std::string(tilemap->first_attribute("name")->value()) == "main")
		{
			loadTilemap(m_tilemap, tilemapString);
		}
		else if (std::string(tilemap->first_attribute("name")->value()) == "background")
		{
			loadTilemap(m_background, tilemapString);
			for (const auto &i : m_background)
				for (const auto &j : i)
				{
					j->setSolid(false);
					j->setBrightness(110); // Could use some fine tweaking
				}
		}
	}

	m_levelWidth = m_tilemapWidth * m_tileWidth;
	m_levelHeight = m_tilemapHeight * m_tileHeight;

	// Load other tilemap stuff
	if (!loadEntities(map)) return false;
	if (!loadGates(map,soundManager)) return false;
	if (!loadDarkZones(map)) return false;

	return true;
}

bool Level::loadEntities(const rapidxml::xml_document<> &doc)
{
	rapidxml::xml_node<> *node;

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
		converter(name, entity->first_attribute("type")->value());
		if (name == "spawn")
		{
			converter(m_spawnX, entity->first_attribute("x")->value());
			m_spawnX += 16;
			converter(m_spawnY, entity->first_attribute("y")->value());
			m_spawnY += 16;
			m_origSpawnX = m_spawnX;
			m_origSpawnY = m_spawnY;
		}
		else
		{
			// All enities are loaded here
			// Check entity type
			sf::Vector2f spawn;
			converter(spawn.x, entity->first_attribute("x")->value());
			converter(spawn.y, entity->first_attribute("y")->value());
			EntityAction action;
			
			if (name == "checkpoint")
			{
				spawn.x += 16;
				spawn.y += 16;
				action.pos = spawn;
				m_entityMap.push_back(new Checkpoint(action, action.pos));
			}
			else if (name == "coin")
			{
				converter(action.value, entity->first_node()->first_node()->first_attribute("value")->value());
				m_entityMap.push_back(new Coin(action, spawn));
			}

			else if (name == "finish")
			{
				m_entityMap.push_back(new Finish(action, spawn));
			}
			else if (name == "text")
			{
				std::string string{ entity->first_node()->value() };
				std::transform(string.begin(),string.end(),string.begin(),::toupper);
				sf::Text text(string,DataManager::getInstance().getFont("pixel"));
				text.setPosition(spawn);
				text.setCharacterSize(8);
				m_textMap.push_back(text);
			}
		}
	}
	return true;
}

bool Level::loadGates(const rapidxml::xml_document<> &xmlDoc, SoundManager & soundManager)
{
	struct GateNode
	{
		sf::FloatRect m_pos;
		float m_speed;
		int m_id;
		float m_height{ -1 };
		bool m_inverted;
	};

	rapidxml::xml_node<> *xmlNode;
	std::vector<GateNode> gateNodes;

	// Find objectgroup
	xmlNode = xmlDoc.first_node("map")->first_node("objectgroup");
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
			
		if (std::string(gate->first_attribute("type")->value()) == "switch")
		{
			EntityAction action;
			sf::Vector2f pos;
			converter(pos.x, gate->first_attribute("x")->value());
			converter(pos.y, gate->first_attribute("y")->value());

			converter(action.value, prop->first_attribute("value")->value());

			m_entityMap.push_back(new Switch(action, pos));
		}
		else
		{
			GateNode gateNode;
			converter(gateNode.m_pos.left, gate->first_attribute("x")->value());
			converter(gateNode.m_pos.top, gate->first_attribute("y")->value());
			converter(gateNode.m_pos.width, gate->first_attribute("width")->value());
			converter(gateNode.m_pos.height, gate->first_attribute("height")->value());

			while (prop)
			{
				if (std::string(prop->first_attribute("name")->value()) == "id")
					converter(gateNode.m_id, prop->first_attribute("value")->value());
				else if (std::string(prop->first_attribute("name")->value()) == "speed")
					converter(gateNode.m_speed, prop->first_attribute("value")->value());
				else if (std::string(prop->first_attribute("name")->value()) == "height")
					converter(gateNode.m_height, prop->first_attribute("value")->value());
				else if (std::string(prop->first_attribute("name")->value()) == "inverted")
				{
					std::string temp;
					converter(temp, prop->first_attribute("value")->value());
					if (temp == "true")
						gateNode.m_inverted = true;
					else
						gateNode.m_inverted = false;
				}
				prop = prop->next_sibling();
			}
			gateNodes.push_back(gateNode);
		}
	}

	// Link gate tiles to their gate
	for (const auto &iter : gateNodes)
	{
		if (iter.m_height == -1)
		{
			Gate gate(iter.m_id, iter.m_speed,iter.m_inverted);
			std::vector<GateTile*> gateTiles;
			for (sf::Vector2i pos = mapWorldToTilemap(sf::Vector2f(iter.m_pos.left, iter.m_pos.top), static_cast<int>(iter.m_pos.width), static_cast<int>(iter.m_pos.height))
				; pos.y >= 0 && m_tilemap[pos.y][pos.x]->getTileMeta().m_tileType == TileType::Gate; pos.y--)
			{
				GateTile * tile = static_cast<GateTile*>(m_tilemap[pos.y][pos.x]);
				gateTiles.push_back(tile);
			}
			gate.loadBottomTile(gateTiles.front());
			gate.loadTopTile(gateTiles.back());
			gate.loadGate();
			for (int i = 1; i < gateTiles.size() - 1;++i)
				gateTiles[i]->setSolid(false);

			gate.initSound(soundManager);
			m_gateMap.push_back(std::move(gate));
		}
		else
		{
			Elevator elevator(iter.m_id, iter.m_height, iter.m_speed,iter.m_inverted);
			std::vector<ElevatorTile*> elevatorTiles;
			for (sf::Vector2i pos = mapWorldToTilemap(sf::Vector2f(iter.m_pos.left, iter.m_pos.top), static_cast<int>(iter.m_pos.width), static_cast<int>(iter.m_pos.height))
				; pos.y >= 0 && m_tilemap[pos.y][pos.x]->getTileMeta().m_tileType == TileType::Gate; pos.y--)
			{
				ElevatorTile * tile = static_cast<ElevatorTile*>(m_tilemap[pos.y][pos.x]);
				elevatorTiles.push_back(tile);
			}
			if (elevatorTiles.size() == 0)
			{
				sf::Vector2i pos = mapWorldToTilemap(sf::Vector2f(iter.m_pos.left, iter.m_pos.top), static_cast<int>(iter.m_pos.width), static_cast<int>(iter.m_pos.height));
				m_tilemap[pos.y][pos.x] = getTile(static_cast<int>(TileType::Gate), iter.m_pos.left,iter.m_pos.top);
				ElevatorTile * tile = static_cast<ElevatorTile*>(m_tilemap[pos.y][pos.x]);
				tile->setSize(sf::Vector2f(tile->getHitbox().width, 0));
				elevatorTiles.push_back(tile);
			}
				elevator.loadBottomTile(elevatorTiles.front());
				elevator.loadTopTile(elevatorTiles.back());
				elevator.loadElevator();
				for (std::size_t i = 1; i < elevatorTiles.size(); ++i)
					elevatorTiles[i]->setSolid(false);
				
				elevator.initSound(soundManager);
				m_elevatorMap.push_back(std::move(elevator));
		}
	}

	return true;
}

bool Level::loadDarkZones(const rapidxml::xml_document<>& doc)
{
	rapidxml::xml_node<> * area = doc.first_node("map")->first_node();
	for (;area; area = area->next_sibling())
		if (std::string(area->name()) == "objectgroup" && std::string(area->first_attribute("name")->value()) == "area")
			break;

	if (!area) return true;

	for (area = area->first_node("object"); area; area = area->next_sibling())
	{
		sf::FloatRect darkzone;
	
		converter(darkzone.left, area->first_attribute("x")->value());
		converter(darkzone.top,	area->first_attribute("y")->value());
		converter(darkzone.width, area->first_attribute("width")->value());
		converter(darkzone.height, area->first_attribute("height")->value());
	
		m_darkZones.push_back(darkzone);
	}

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

bool Level::loadLevel(Camera & camera, PlayerObject * const player, GameBackground &background, Light &light, SoundManager & soundManager)
{
	if (!initMap(soundManager)) return false;
	if (!initPlayer(player)) return false;
	if (!initCamera(camera)) return false;
	if (!initBackground(background)) return false;
	if (!initLight(light,player)) return false;
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
	for (auto &iter : m_elevatorMap)
		iter.update(elapsedTime);
}

void Level::draw(sf::RenderTarget & target, const Camera &camera) const
{
	sf::IntRect tileBounds = camera.getTileBounds(m_tileWidth, m_tileHeight);

	// Draw background tiles
	if(!m_background.empty())
	for (int i = tileBounds.top; i < tileBounds.height + tileBounds.top; ++i)
		for (int j = tileBounds.left; j < tileBounds.width + tileBounds.left; ++j)
				target.draw(*m_background[i][j]);

	for (auto iter : m_darkZones)
	{
		sf::RectangleShape temp;
		temp.setSize(sf::Vector2f(iter.width, iter.height));
		temp.setPosition(iter.left, iter.top);
		temp.setFillColor(sf::Color(40,0,0,255));
		target.draw(temp, sf::BlendMultiply);
	}

	for (const auto &iter : m_textMap)
		target.draw(iter);

	for (const auto &iter : m_entityMap)
		target.draw(*iter);

	for (const auto &vertic : m_tilemap)
		for (const auto &horiz : vertic)
			if (horiz->getTileMeta().m_tileType == TileType::Gate || horiz->getTileMeta().m_tileType == TileType::Elevator)
				target.draw(*horiz);

	for (int i = tileBounds.top; i < tileBounds.height + tileBounds.top; ++i)
		for (int j = tileBounds.left; j < tileBounds.width + tileBounds.left; ++j)
			if (m_tilemap[i][j]->getTileMeta().m_tileType != TileType::Gate && m_tilemap[i][j]->getTileMeta().m_tileType != TileType::Elevator)
			target.draw(*m_tilemap[i][j]);
}

bool Level::inLevelBounds(const sf::Vector2f & point)
{
	return point.y > m_levelHeight;
}

bool Level::inDarkZone(sf::FloatRect hitbox)
{
	for (auto iter : m_darkZones)
		if (iter.intersects(hitbox))
			return true;
	return false;
}

void Level::toggleGate(const int id)
{
	for (auto &iter : m_gateMap)
		if(iter.getId() == id)
			iter.m_isOpen = iter.m_isOpen ? false : true;
	for (auto &iter : m_elevatorMap)
		if (iter.getId() == id)
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

const sf::Vector2f Level::getOriginalSpawn() const
{
	return sf::Vector2f(m_origSpawnX,m_origSpawnY);
}

const std::string & Level::getTitle() const
{
	return m_title;
}

const std::vector<Entity*>& Level::getEntityMap() const
{
	return m_entityMap;
}