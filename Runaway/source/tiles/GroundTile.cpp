#include "GroundTile.h"



GroundTile::GroundTile(const float x, const float y, const std::string tilesetName):
	Tile(x,y,tilesetName)
{
	m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}