#include "GroundTopTile.h"



GroundTopTile::GroundTopTile(const float x, const float y, const std::string tilesetName):
	Tile(TileType::Top, x,y,tilesetName)
{
	m_sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
}