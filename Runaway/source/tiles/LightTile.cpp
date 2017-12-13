#include "LightTile.h"

#include <SFML\Graphics\RenderTarget.hpp>

void LightTile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

LightTile::LightTile(const float x, const float y):
	Tile(TileType::Light, x,y,1.0f)
{
	m_sprite.setTextureRect(sf::IntRect(32 * 2 ,0, 32, 32));
	m_lightPool.setPos(m_sprite.getPosition() + sf::Vector2f(16, 16));
	m_lightPool.setSize(32 * 4);
}

const LightPool & LightTile::getLightPool() const
{
	return m_lightPool;
}
