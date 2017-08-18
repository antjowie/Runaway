//
// Level class template
// 

#pragma once
#include"Level.h"

#include <SFML\Graphics.hpp>
#include <vector>

class Level
{
private:
	sf::Vector2f m_cameraSize;
	int m_levelWidth, m_levelHeight; // Not in tiles
	int m_spawnX, m_spawnY;
	std::vector<std::vector<int>> m_tileMap;

public:
	//Level(const std::string &tileMapPath, const float cameraWidth, const float camerHeight);
	~Level();

	//void loadMap(std::vector<std::vector<Tile*>> const *tileMap);
};

