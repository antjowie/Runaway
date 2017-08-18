//
// Level class template
// 

#pragma once
#include "Level.h"
#include "Tile.h"
#include "Camera.h"

#include <SFML\Graphics.hpp>
#include <vector>

class Level
{
private:
	sf::Vector2f m_cameraSize;

	const int m_levelWidth{ 0 }, m_levelHeight{ 0 }; // Not in tiles 
	const int m_spawnX{ 0 }, m_spawnY{ 0 };
	const std::string m_tileMapPath, m_title;

	bool initMap(std::vector<std::vector<Tile*>> *tileMap);
	bool initCamera(Camera &camera);
	
public:
	Level(const std::string &tileMapPath, const float cameraWidth, const float camerHeight);
	~Level();

	bool loadLevel(std::vector<std::vector<Tile*>> *tileMap, Camera &camera); // This will load all the files for this level
};

