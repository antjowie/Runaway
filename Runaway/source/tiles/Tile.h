//
// Template class for all tiles
//

#pragma once
#include <SFML\Graphics.hpp>

enum class tileType{ Air, Block, Top, Light, Gate, Invalid}; // Has to be same order as in Tiled

class Tile
{
private:
	tileType m_tileType;

protected:
	bool m_solid = true;
	sf::Sprite m_sprite;

public:
	Tile(const tileType type, const float x, const float y, const std::string tilesetName, const bool isSolid = true);
	
	virtual void update();
	virtual void draw(sf::RenderWindow& window);

	const sf::FloatRect getHitbox() const;
	const tileType getType() const;
	const bool isSolid() const;
};

Tile * const getTile(const int id, const float x, const float y, const std::string tilesetName);