//
// Level class template
// 

#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class Level
{
private:
	sf::FloatRect m_cameraSize;
	std::vector<std::vector<int>> m_tileMap; // Int will be replaced with Tile

public:
	Level();
	~Level();
};

