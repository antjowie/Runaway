#include "CollisionHandler.h"
#include "PlayerObject.h"
#include "Level.h"

bool CollisionHandler::checkLoaded()
{
	bool loaded = true;
	if (m_tilemap == nullptr) loaded = false;
	return loaded;
}

CollisionHandler::CollisionHandler()
{
	m_tilemap = nullptr;
}

CollisionHandler::~CollisionHandler()
{
}

bool CollisionHandler::loadTilemap(std::vector<std::vector<Tile*>>* const tilemap)
{
	m_tilemap = tilemap;

	return m_tilemap != nullptr ? true : false;
}

void CollisionHandler::loadSurroundingTiles()
{
	if (!checkLoaded()) return;

	sf::Vector2i tilemapPlayerCoords{ mapWorldToTilemap(sf::Vector2f(m_playerHitbox.left,m_playerHitbox.top),m_tileWidth,m_tileHeight) };
	const int offset{ 2 };

	m_surroundingTiles.clear();
	for (int horizontal = tilemapPlayerCoords.x - offset; horizontal <= tilemapPlayerCoords.x + offset; horizontal++)
	{
		for (int vertical = tilemapPlayerCoords.y - offset; vertical <= tilemapPlayerCoords.y + offset; vertical++)
		{
			if ((unsigned int)horizontal < m_tilemap->size() && (unsigned int)vertical < (*m_tilemap)[horizontal].size())
			{
				// Order shouldn't matter. If we do map this vector to tilemap vector. Our function calls would do 6 comparisons less each
				// Our first vector is y, the subsequent vectors are x.
				m_surroundingTiles.push_back((*m_tilemap)[vertical][horizontal]);
			}
		}
	}
}

void CollisionHandler::setTileSize(const sf::Vector2i & tileSize)
{
	m_tileWidth = tileSize.x;
	m_tileHeight = tileSize.y;
}

void CollisionHandler::updatePlayerHitbox(const sf::FloatRect &playerHitbox)
{
	m_playerHitbox = playerHitbox;
}

const float CollisionHandler::distanceTillBottomCollision(const sf::FloatRect &playerHitbox)
{
	if (!checkLoaded()) return 0.0f;

	loadSurroundingTiles();
	updatePlayerHitbox(playerHitbox);

	// Get lower hitbox
	sf::FloatRect bottomHit{ m_playerHitbox };
	m_bottomDistance = bottomHit.height * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles)
	{
		float distance = abs(bottomHit.top + bottomHit.height - iter->getHitbox().top);
		if (iter->isSolid() && bottomHit.intersects(iter->getHitbox()) && distance < m_bottomDistance)
			m_bottomDistance = distance;
	}

	if (m_bottomDistance == bottomHit.height * 3)
		m_bottomDistance = 0.0f;
	return m_bottomDistance;
}

const float CollisionHandler::distanceTillUpperCollision(const sf::FloatRect &playerHitbox)
{
	if (!checkLoaded()) return 0.0f;

	loadSurroundingTiles();
	updatePlayerHitbox(playerHitbox);

	// Get upper hitbox	
	sf::FloatRect upperHit{ m_playerHitbox };
	m_upperDistance = upperHit.height * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles)
	{
		float distance = abs(upperHit.top - (iter->getHitbox().top + iter->getHitbox().height));
		if (iter->isSolid() && upperHit.intersects(iter->getHitbox()) && distance < m_upperDistance)
			m_upperDistance = distance;
	}

	if (m_upperDistance == upperHit.height * 3)
		m_upperDistance = 0.0f;
	return m_upperDistance;
}

const float CollisionHandler::distanceTillLeftCollision(const sf::FloatRect &playerHitbox)
{
	if (!checkLoaded()) return 0.0f;

	loadSurroundingTiles();
	updatePlayerHitbox(playerHitbox);

	// Get left collision
	sf::FloatRect leftHit{ m_playerHitbox };
	m_leftDistance = leftHit.width * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles)
	{
		float distance = abs(leftHit.left - (iter->getHitbox().left + iter->getHitbox().width));
		if (iter->isSolid() && leftHit.intersects(iter->getHitbox()) && distance < m_leftDistance)
			m_leftDistance = distance;
	}

	if (m_leftDistance == leftHit.width * 3)
		m_leftDistance = 0.0f;
	return m_leftDistance;
}

const float CollisionHandler::distanceTillRightCollision(const sf::FloatRect &playerHitbox)
{
	if (!checkLoaded()) return 0.0f;

	loadSurroundingTiles();
	updatePlayerHitbox(playerHitbox);

	// Get right collision
	sf::FloatRect rightHit{ m_playerHitbox };
	m_rightDistance = rightHit.width * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles)
	{
		float distance = abs((rightHit.left + rightHit.width) - iter->getHitbox().left);
		if (iter->isSolid() && rightHit.intersects(iter->getHitbox()) && distance < m_rightDistance)
			m_rightDistance = distance;
	}

	if (m_rightDistance == rightHit.width * 3)
		m_rightDistance = 0.0f;
	return m_rightDistance;
}