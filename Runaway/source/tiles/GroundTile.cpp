#include "GroundTile.h"



GroundTile::GroundTile(const float x, const float y):
	Tile(TileType::Block, x,y)
{
	m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}