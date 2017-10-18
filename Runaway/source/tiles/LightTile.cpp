#include "LightTile.h"



LightTile::LightTile(const float x, const float y, const std::string tilesetName):
	Tile(tileType::Light, x,y,tilesetName)
{
	m_sprite.setTextureRect(sf::IntRect(32 * 2 ,0, 32, 32));
}