#include "AirTile.h"



AirTile::AirTile(const float x, const float y):
	Tile(TileType::Air, x,y,false)
{
	m_sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
	m_sprite.setColor(sf::Color::Transparent);
}
