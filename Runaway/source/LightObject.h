//
// This class will hold all the shapes which emit light
// Those will be rendered above the dark overlay
//

#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "Tile.h"

class LightObject
{
	std::vector<const Tile*> m_tiles;
	std::vector<const sf::Sprite*> m_shapes;

public:
	void addTile(const Tile* const tile);
	void addShape(const sf::Sprite* const sprite);

	void draw(sf::RenderWindow &window) const;
};