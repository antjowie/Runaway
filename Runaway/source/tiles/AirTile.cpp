#include "AirTile.h"



void AirTile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// Makes it so that air tiles aren't drawn
}

AirTile::AirTile(const float x, const float y):
	Tile(TileType::Air, x,y,false)
{
	m_sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
	m_sprite.setColor(sf::Color::Transparent);
}
