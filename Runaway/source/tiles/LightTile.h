#pragma once
#include "Tile.h"
#include "LightPool.h"

class LightTile:
	public Tile
{
private:
	LightPool m_lightPool;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	LightTile(const float x, const float y);

	const LightPool &getLightPool() const;
};