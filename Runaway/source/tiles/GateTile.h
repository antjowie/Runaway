

#pragma once
#include "Tile.h"

class GateTile :
	public Tile
{
private:

	// Used to fix texture rect from 0 to 1
	void fixTextureRect();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
public:	
	enum Part { Bottom , Top}part;
	enum State{ Open, Closed};
	GateTile(const float x, const float y);
	
	// Set initial position
	void setRect(const sf::FloatRect newRect);
	void setSize(const sf::Vector2f newSize);
	void setTextureType(const State state);
};