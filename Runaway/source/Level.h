//
// Level class template
// 

#pragma once
#include "Level.h"
#include "Tile.h"
#include "Camera.h"

#include <SFML\Graphics.hpp>
#include <vector>

void remove_char(std::string &str, char ch);

class Level
{
private:
	std::vector<std::vector<Tile*>> m_tileMap;
	const sf::Vector2f m_cameraSize;
	const std::string m_levelMapPath;

	float m_cameraSpeed{ 0 };
	int m_levelWidth{ 0 }, m_levelHeight{ 0 };		// Not in tiles
	int m_tilemapWidth{ 0 }, m_tilemapHeight{ 0 };	// In tiles
	int m_tileWidth{ 0 }, m_tileHeight{ 0 };		// Size of a tile
	const int m_spawnX{ 0 }, m_spawnY{ 0 };
	const std::string m_title;

	bool initMap();
	bool initCamera(Camera &camera);
	
public:
	Level(const std::string &levelMapPath,const std::string &title, const float cameraWidth, const float camerHeight, const float cameraSpeed,
		const int spawnX, const int spawnY);

	bool loadLevel(Camera &camera); // This will load all the files for this level

	void draw(sf::RenderWindow &window);
};

