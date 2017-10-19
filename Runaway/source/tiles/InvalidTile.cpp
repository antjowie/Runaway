#include "InvalidTile.h"



InvalidTile::InvalidTile(const float x, const float y, const std::string tilesetName):
	Tile(TileType::Invalid, x,y,tilesetName)
{
	m_sprite.setTextureRect(sf::IntRect(32 * 3, 0, 32, 32));
}