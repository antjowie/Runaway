//
// Elevator is a variation of the gate class. It is the same as a gate, but doesn't have a top part.
//

#pragma once
#include "Tile.h"


class ElevatorTile :
	public Tile
{
private:
	// Used to fix texture rect from 0 to 1
	void fixTextureRect();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	enum State { Open, Closed };
	ElevatorTile(const float x, const float y);
	
	// Set initial position
	void setSize(const sf::Vector2f newSize);
	void setTextureType(const State state);
};