#include "AirTile.h"



AirTile::AirTile(const float x, const float y, const std::string tilesetName):
	Tile(TileType::Air, x,y,tilesetName,false)
{
	m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	m_sprite.setColor(sf::Color::Transparent);
}
