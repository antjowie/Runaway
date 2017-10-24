#include "InvalidTile.h"



InvalidTile::InvalidTile(const float x, const float y):
	Tile(TileType::Invalid, x,y)
{
	m_sprite.setTextureRect(sf::IntRect(32 * 3, 0, 32, 32));
}