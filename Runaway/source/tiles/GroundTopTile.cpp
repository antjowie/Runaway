#include "GroundTopTile.h"



GroundTopTile::GroundTopTile(const float x, const float y):
	Tile(TileType::Top, x,y)
{
	m_sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
}