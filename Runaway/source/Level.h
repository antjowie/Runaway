//
// Level class template
// 
// Map is loaded from left to right
// from top to bottom
// 

#pragma once
#include "Level.h"
#include "Elevator.h"
#include "Gate.h"
#include "Tile.h"
#include "Camera.h"
#include "PlayerObject.h"
#include "Entity.h"
#include "rapidxml.hpp"
#include "GameBackground.h"
#include "Light.h"

#include <SFML\Graphics.hpp>
#include <vector>

void remove_char(std::string &str, char ch);
const sf::Vector2i mapWorldToTilemap(const sf::Vector2f &coords, sf::Vector2i &tileSize);
const sf::Vector2i mapWorldToTilemap(const sf::Vector2f &coords, const int tileWidth, const int tileHeight);
template<class T>
void converter(T &converted, const std::string &toConvert);
template<class T>
void converter(T &converted, const char &toConvert);

class Level
{
private:
	// All these data managers are getting out of hand, but that is because I haven't planed this code well enough
	// While I could fix it, I think it isn't worth it and instead invest my time into finishing the game
	std::vector<sf::Text> m_textMap;
	std::vector<std::vector<Tile*>> m_tilemap;
	std::vector<std::vector<Tile*>> m_background;
	std::vector<Entity*> m_entityMap;
	std::vector<Gate> m_gateMap;
	std::vector<Elevator> m_elevatorMap;
	std::vector<sf::FloatRect> m_darkZones;
	
	sf::Vector2f m_cameraSize;
	const std::string m_levelPath;

	float m_cameraSpeed{ 0 };
	int m_levelWidth{ 0 }, m_levelHeight{ 0 };		// Not in tiles
	int m_tilemapWidth{ 0 }, m_tilemapHeight{ 0 };	// In tiles
	int m_tileWidth{ 0 }, m_tileHeight{ 0 };		// Size of a tile
	float m_spawnX{ 0 }, m_spawnY{ 0 };
	float m_origSpawnX{ 0 }, m_origSpawnY{ 0 };

	const std::string m_title;
	const std::string m_tilesetName;

	bool initMap(SoundManager & soundManager);
	bool initCamera(Camera &camera);
	bool initPlayer(PlayerObject *const player);
	bool initBackground(GameBackground &background);
	bool initLight(Light &light, const PlayerObject * const player);

	bool loadTileLayer(std::vector<char> tilemap, SoundManager & soundManager);
	void loadTilemap(std::vector<std::vector<Tile*>> &tilemap, const std::string tilemapString);
	
	bool loadEntities(const rapidxml::xml_document<> &doc);
	bool loadGates(const rapidxml::xml_document<> &xmlDox, SoundManager & soundManager);
	bool loadDarkZones(const rapidxml::xml_document<> &doc);

public:
	Level(const std::string &levelMapPath,const std::string &title, const float cameraSpeed, const std::string tilesetName);
	~Level();

	void update(const float elapsedTime);
	void draw(sf::RenderTarget &window,const Camera &camera) const;

	bool inLevelBounds(const sf::Vector2f &point);
	bool inDarkZone(sf::FloatRect hitbox);

	void toggleGate(const int id);
	void setSpawn(const sf::Vector2f &pos);
	
	bool loadLevel(Camera &camera, PlayerObject * const player,GameBackground &background, Light &light, SoundManager & soundManager); // This will load all the files for this level
	
	const sf::Vector2f getSpawn() const;
	const sf::Vector2f getOriginalSpawn() const;
	const std::string &getTitle() const;
	const std::vector<std::vector<Tile*>> &getTilemap() const;
	const std::vector<Entity*> &getEntityMap() const;
};