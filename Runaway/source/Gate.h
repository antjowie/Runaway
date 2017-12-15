//
// The Gate class will make the outer gate parts become two gate componenets
// The gate tiles inbetween will be invisible
// Gate is not responsible for drawing tiles, it only modifies them
//

#pragma once
#include <vector>
#include <SFML\Graphics\RectangleShape.hpp>
#include "GateTile.h"

class GatePart
{
private:
	GateTile* m_top;
	GateTile* m_bottom;
	float m_originalHeight{ 0 };

public:
	
	// Must be between 0-1
	void update(const float percentage);

	void loadTopPart(GateTile* const topGateTile);
	void loadBottomPart(GateTile* const bottomGatePart);
	void loadGate();

	const GateTile* const getTopTile() const;
	const GateTile* const getBottomTile() const;

	void setState(const GateTile::State state);
};

class Gate
{
private:
	GatePart m_gate;
	
	float m_speed; // The time it takes to open and close the door in seconds
	float m_timeline{ 0 }; // Local timeline used to calculate position of gate. When 0 is fully closed.

	const bool m_inverted;

	int m_id; // The id value is a value that to connect with switches
public:
	bool m_isOpen; // Decides wheter gate should be opening or not

	Gate(const int id, const float speed, const bool inverted = false);
	
	// Init phase
	void loadTopTile(GateTile* const topGateTile);
	void loadBottomTile(GateTile* const bottomGateTile);
	void loadGate(); // Call after top and bottomTile are loaded

	void update(const float elapsedTime);
	
	const int getId();

	const GateTile* const getTopTile() const;
	const GateTile* const getBottomTile() const;
};