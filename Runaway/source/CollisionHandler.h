//
// -- Instructions
// - Initialization
// To initialize the class, the following functions must be run:
// loadTilemap, setTileSize
// 
// - Usage
// The distanceTill* functions return the distance inside of a block
// relative to the m_playerHitbox value. This function will return
// zero if the m_playerHitbox does not intersect with a block
//

#pragma once
#include <SFML\Graphics.hpp>
#include <Tile.h>

class CollisionHandler
{
private:
	std::vector<std::vector<Tile*>> *m_tilemap;
	std::vector<Tile*> m_surroundingTiles;
	sf::FloatRect m_playerHitbox;
	// We need these values to map our global coords to tilemap coords
	int m_tileWidth{ 0 };
	int m_tileHeight{ 0 };
	float m_bottomDistance{ 0 },
		m_upperDistance{ 0 },
		m_leftDistance{ 0 },
		m_rightDistance{ 0 };

	bool checkLoaded();
	void loadSurroundingTiles();
	void updatePlayerHitbox(const sf::FloatRect &playerHitbox);

public:
	CollisionHandler();

	~CollisionHandler();

	void loadTilemap(std::vector<std::vector<Tile*>>* const tilemap);
	void setTileSize(const sf::Vector2i &tileSize);

	const float distanceTillBottomCollision	(const sf::FloatRect &playerHitbox);
	const float distanceTillUpperCollision	(const sf::FloatRect &playerHitbox);
	const float distanceTillLeftCollision	(const sf::FloatRect &playerHitbox);
	const float distanceTillRightCollision	(const sf::FloatRect &playerHitbox);
};