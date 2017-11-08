#include "LightTile.h"



void LightTile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.blendMode = sf::BlendAdd;
	target.draw(m_sprite);
}

LightTile::LightTile(const float x, const float y):
	Tile(TileType::Light, x,y)
{
	m_sprite.setTextureRect(sf::IntRect(32 * 2 ,0, 32, 32));
}