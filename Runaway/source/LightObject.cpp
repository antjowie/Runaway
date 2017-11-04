#include "LightObject.h"

void LightObject::addTile(const Tile * const tile)
{
	m_tiles.push_back(tile);
}

void LightObject::addShape(const sf::Sprite * const sprite)
{
	m_shapes.push_back(sprite);
}

void LightObject::draw(sf::RenderWindow & window) const
{
	for (const auto &iter : m_tiles)
		iter->draw(window);
	for (const auto &iter : m_shapes)
		window.draw(*iter);
}
