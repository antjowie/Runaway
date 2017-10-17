//
// Level class template
// 
// Map is loaded from left to right
// from top to bottom
// 

#pragma once
#include "Level.h"
#include "Tile.h"
#include "Camera.h"
#include "PlayerObject.h"
#include "Entity.h"

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
	std::vector<std::vector<Tile*>> m_tilemap;
	std::vector<Entity*> m_entityMap;
	
	const sf::Vector2f m_cameraSize;
	const std::string m_levelMapPath;

	float m_cameraSpeed{ 0 };
	int m_levelWidth{ 0 }, m_levelHeight{ 0 };		// Not in tiles
	int m_tilemapWidth{ 0 }, m_tilemapHeight{ 0 };	// In tiles
	int m_tileWidth{ 0 }, m_tileHeight{ 0 };		// Size of a tile
	float m_spawnX{ 0 }, m_spawnY{ 0 };

	const std::string m_title;
	const std::string m_tilesetName;

	bool initMap();
	bool initCamera(Camera &camera);
	bool initPlayer(PlayerObject *const player);

	bool loadTilemap(std::vector<char> tilemap);
	bool loadEntities(std::vector<char> tilemap);

public:
	Level(const std::string &levelMapPath,const std::string &title, const float cameraWidth, const float camerHeight, const float cameraSpeed, const std::string tilesetName);

	void update(const float elapsedTime);
	void draw(sf::RenderWindow &window,const Camera &camera);

	bool inLevelBounds(const sf::Vector2f &point);

	void setSpawn(const sf::Vector2f &pos);
	
	bool loadLevel(Camera &camera, PlayerObject * const player); // This will load all the files for this level
	const sf::Vector2f getSpawn() const;
	const std::string &getTitle() const;
	const std::vector<std::vector<Tile*>> &getTilemap() const;
	const std::vector<Entity*> &getEntityMap() const;
};