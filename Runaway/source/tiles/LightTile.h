#pragma once
#include "Tile.h"

class LightTile:
	public Tile
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

public:
	LightTile(const float x, const float y);
};

