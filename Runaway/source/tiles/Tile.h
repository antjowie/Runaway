//
// Template class for all tiles
//

#pragma once
#include <SFML\Graphics.hpp>

enum tileType{Air, Block, Top, Light, Door};

class Tile
{
protected:
	bool m_solid = true;
	sf::Sprite m_sprite;

public:
	Tile(const float x, const float y, const std::string tilesetName, const bool isSolid = true);

	virtual void update();
	virtual void draw(sf::RenderWindow& window);

	const sf::FloatRect getHitbox() const;
	const bool isSolid() const;
};

Tile * const getTile(const int id, const float x, const float y, const std::string tilesetName);