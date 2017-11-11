//
// The gate class will handle gate tiles as one object, so that they
// can be moved all at once to represent a real gate
//

#pragma once
#include <vector>
#include "GateTile.h"

class Gate
{
private:
	std::vector<GateTile*> m_tiles;
	std::vector<GateTile> m_originalTiles; // Original pos
	
	float m_speed ; // The time it takes to open and close the door in seconds

	bool isClosed();

public:
	bool m_isOpen;
	int m_id; // The id value is a value that to connect with switches

	Gate();
	Gate(const int id, const float speed);
	
	// Init phase
	void addTile(GateTile* const tile);
	void addTile(std::vector<GateTile*> &tiles);

	void setSpeed(const float speed);
	void updateTextureRect(); // Called to fix textures

	void update(const float elapsedTime);
	const std::vector<GateTile> &getOriginalTiles() const; // For the light function
};