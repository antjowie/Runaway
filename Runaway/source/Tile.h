//
// Template class for all tiles
//

#pragma once
#include <SFML\Graphics.hpp>

enum tileType{Air, Block, Top, Light};

class Tile
{
private:
	sf::Sprite m_sprite;
	bool m_solid = true;

public:
	Tile(const int id, const float x, const float y);

	//void update();
	void draw(sf::RenderWindow& window);
};