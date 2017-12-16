#include "CollisionHandler.h"
#include "PlayerObject.h"
#include "Level.h"

bool CollisionHandler::checkLoaded()
{
	return m_tilemap != nullptr;
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

	sf::Vector2i tilemapPlayerCoords{ mapWorldToTilemap(sf::Vector2f(m_playerHitbox.left,m_playerHitbox.top),m_tileHeight,m_tileWidth) };
	const int offset{ 5 };

	m_surroundingTiles.clear();
	for (int j= tilemapPlayerCoords.y - offset; j<= tilemapPlayerCoords.y + offset; j++)
	{
		for (int i = tilemapPlayerCoords.x - offset; i<= tilemapPlayerCoords.x + offset; i++)
		{
			if ((unsigned int)j < m_tilemap->size() && (unsigned int)i< (*m_tilemap)[j].size())
			{
				// Order shouldn't matter. If we do map this vector to tilemap vector. Our function calls would do 6 comparisons less each
				// Our first vector is y, the subsequent vectors are x.
				if ((*m_tilemap)[j][i]->getTileMeta().m_solid == true)
				m_surroundingTiles.push_back((*m_tilemap)[j][i]);
			}
		}
	}

	// Load all gates
	for (const auto &vertic : *m_tilemap)
		for (const auto &horizon : vertic)
			if (horizon->getTileMeta().m_tileType == TileType::Elevator || horizon->getTileMeta().m_tileType == TileType::Gate)
				m_surroundingTiles.push_back(horizon);
}

void CollisionHandler::setTileSize(const sf::Vector2i & tileSize)
{
	m_tileWidth = tileSize.x;
	m_tileHeight = tileSize.y;
}

const float CollisionHandler::distanceTillBottomCollision(const sf::FloatRect &playerHitbox)
{
	if (!checkLoaded()) return 0.0f;
	m_playerHitbox = playerHitbox;
	loadSurroundingTiles();

	// Get lower hitbox
	sf::FloatRect bottomHit{ m_playerHitbox };
	m_bottomDistance = bottomHit.height * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles)
	{
		float distance = abs(bottomHit.top + bottomHit.height - iter->getHitbox().top);
		if (iter->getTileMeta().m_solid && bottomHit.intersects(iter->getHitbox()) && distance < m_bottomDistance)
			m_bottomDistance = distance;
	}

	if (m_bottomDistance == bottomHit.height * 3)
		m_bottomDistance = 0.0f;

	return m_bottomDistance;
}

const float CollisionHandler::distanceTillUpperCollision(const sf::FloatRect &playerHitbox)
{
	if (!checkLoaded()) return 0.0f;

	m_playerHitbox = playerHitbox;
	loadSurroundingTiles();

	// Get upper hitbox	
	sf::FloatRect upperHit{ m_playerHitbox };
	m_upperDistance = upperHit.height * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles)
	{
		float distance = abs(upperHit.top - (iter->getHitbox().top + iter->getHitbox().height));
		if (iter->getTileMeta().m_solid && upperHit.intersects(iter->getHitbox()) && distance < m_upperDistance)
			m_upperDistance = distance;
	}

	if (m_upperDistance == upperHit.height * 3)
		m_upperDistance = 0.0f;
	return m_upperDistance;
}

const float CollisionHandler::distanceTillLeftCollision(const sf::FloatRect &playerHitbox)
{
	if (!checkLoaded()) return 0.0f;

	m_playerHitbox = playerHitbox;
	loadSurroundingTiles();

	// Get left collision
	sf::FloatRect leftHit{ m_playerHitbox };
	m_leftDistance = leftHit.width * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles)
	{
		float distance = abs(leftHit.left - (iter->getHitbox().left + iter->getHitbox().width));
		if (iter->getTileMeta().m_solid && leftHit.intersects(iter->getHitbox()) && distance < m_leftDistance)
			m_leftDistance = distance;
	}

	if (m_leftDistance == leftHit.width * 3)
		m_leftDistance = 0.0f;
	return m_leftDistance;
}

const float CollisionHandler::distanceTillRightCollision(const sf::FloatRect &playerHitbox)
{
	if (!checkLoaded()) return 0.0f;

	m_playerHitbox = playerHitbox;
	loadSurroundingTiles();

	// Get right collision
	sf::FloatRect rightHit{ m_playerHitbox };
	m_rightDistance = rightHit.width * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles)
	{
		float distance = abs((rightHit.left + rightHit.width) - iter->getHitbox().left);
		if (iter->getTileMeta().m_solid && rightHit.intersects(iter->getHitbox()) && distance < m_rightDistance)
			m_rightDistance = distance;
	}

	if (m_rightDistance == rightHit.width * 3)
		m_rightDistance = 0.0f;
	return m_rightDistance;
}