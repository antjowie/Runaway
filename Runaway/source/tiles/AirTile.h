#pragma once
#include "Tile.h"
class AirTile :
	public Tile
{
private:

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
public:
	AirTile(const float x, const float y);
};

