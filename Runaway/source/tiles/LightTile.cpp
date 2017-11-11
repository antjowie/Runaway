#include "LightTile.h"



LightTile::LightTile(const float x, const float y):
	Tile(TileType::Light, x,y,1.0f)
{
	m_sprite.setTextureRect(sf::IntRect(32 * 2 ,0, 32, 32));
}