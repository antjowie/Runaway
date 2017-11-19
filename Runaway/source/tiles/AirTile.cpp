#include "AirTile.h"



void AirTile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// Makes it so that air tiles aren't drawn
}

AirTile::AirTile(const float x, const float y):
	Tile(TileType::Air, x,y,0,false)
{
	m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	m_sprite.setColor(sf::Color::Transparent);
}
